#include "pch.h"
#include "Room.h"
#include "Player.h"
#include "GameSession.h"
#include "ClientPacketHandler.h"

RoomRef GRoom;

Room::Room()
	: _state(Protocol::eRoomState::ROOM_STATE_NONE)
{

}

void Room::Initialize()
{
	WRITE_LOCK;
	this->_players.clear();
	this->_roomCode = "0000";
	this->_state = Protocol::eRoomState::ROOM_STATE_LOBBY;
}

bool Room::Enter(PlayerRef player)
{
	WRITE_LOCK;

	// ������
	if (_state != Protocol::eRoomState::ROOM_STATE_LOBBY)
	{
		player->ownerGameSession->SendError(ErrorCode::ROOM_PLAYING);

		return false;
	}

	// �ο� �ʰ�
	if (_players.size() > 6)
	{
		player->ownerGameSession->SendError(ErrorCode::ROOM_FULL);
		return false;
	}

	_players[player->uid] = { player };
	player->_currentRoom = this->shared_from_this();

	// ������ ���� ��
	if (_players.size() == 1)
		_players[player->uid].host = true;

	// ������ ������� �� ���� �����ֱ�
	{
		Protocol::S_ROOM_ENTER packet;
		auto roomInfo = packet.mutable_roominfo();
		this->GetRoomInfo(roomInfo);

		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(packet);
		player->ownerGameSession->Send(sendBuffer);
	}

	// ���� �� ����鿡�� ����
	{
		Protocol::S_ANOTHER_ENTER_ROOM packet;
		auto roomInfo = packet.mutable_roominfo();
		this->GetRoomInfo(roomInfo);

		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(packet);
		this->BroadCast(sendBuffer);
	}

	return true;
}

bool Room::Leave(PlayerRef player)
{
	WRITE_LOCK;

	// Todo 1�� �������� �� ����
	if (_players.size() == 1)
	{
		_players.clear();
		return true;
	}

	// ������ ��������
	if (_players[player->uid].host == true)
	{
		_players[player->uid].host = false;
		for (auto& [uid, playerData] : _players)
		{
			if (playerData.host == false)
			{
				playerData.host = true;
				break;
			}
		}
	}

	_players.erase(player->uid);
	player->_currentRoom = nullptr;

	// ������ �ٸ� ������� �˷��ֱ�
	{
		Protocol::S_ANOTHER_LEAVE_ROOM packet;
		auto roomInfo = packet.mutable_roominfo();
		this->GetRoomInfo(roomInfo);

		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(packet);
		this->BroadCast(sendBuffer);
	}

	return true;
}

void Room::BroadCast(Horang::SendBufferRef sendBuffer)
{
	WRITE_LOCK;
	for (auto& [id, playerData] : _players)
	{
		playerData.player->ownerGameSession->Send(sendBuffer);
	}
}

Protocol::RoomInfo Room::GetRoomInfo()
{
	Protocol::RoomInfo roomInfo;
	this->GetRoomInfo(roomInfo);
	return roomInfo;
}

void Room::GetRoomInfo(Protocol::RoomInfo& roomInfo)
{
	roomInfo.set_roomid(_roomID);
	roomInfo.set_roomcode(_roomCode);
	roomInfo.set_state(_state);

	for (auto& [uid, playerData] : _players)
	{
		auto userInfo = roomInfo.add_users();
		userInfo->set_id(playerData.player->id);
		userInfo->set_nickname(playerData.player->nickname);
		userInfo->set_host(playerData.host);
		userInfo->set_team(playerData.team);
	}
}

void Room::GetRoomInfo(Protocol::RoomInfo* roomInfo)
{
	this->GetRoomInfo(*roomInfo);
}

void Room::PushJob(JobRef job)
{
	WRITE_LOCK;

	_jobs.Push(job);
}

void Room::FlushJob()
{
	WRITE_LOCK;

	while (true)
	{
		JobRef job = _jobs.Pop();
		if (job == nullptr)
			break;

		job->Execute();
	}
}
