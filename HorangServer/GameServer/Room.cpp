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

	// 게임중
	if (_state != Protocol::eRoomState::ROOM_STATE_LOBBY)
	{
		player->ownerGameSession->SendError(ErrorCode::ROOM_PLAYING);

		return false;
	}

	// 인원 초과
	if (_players.size() > 6)
	{
		player->ownerGameSession->SendError(ErrorCode::ROOM_FULL);
		return false;
	}

	_players[player->uid] = { player };
	player->_currentRoom = this->shared_from_this();

	// 방장이 없을 때
	if (_players.size() == 1)
		_players[player->uid].data.set_host(true);

	// 접속한 사람에게 방 정보 보내주기
	{
		Protocol::S_ROOM_ENTER packet;
		auto roomInfo = packet.mutable_roominfo();
		this->GetRoomInfo(roomInfo);

		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(packet);
		player->ownerGameSession->Send(sendBuffer);
	}

	// 기존 방 사람들에게 보냄
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

	// Todo 1명만 남았을때 방 삭제
	if (this->_players.size() == 1)
	{
		this->_players.clear();
		this->_state = Protocol::eRoomState::ROOM_STATE_LOBBY;
		return true;
	}

	// 방장이 나갔을때
	if (this->_players[player->uid].data.host() == true)
	{
		this->_players[player->uid].data.set_host(false);
		for (auto& [uid, player] : this->_players)
		{
			if (player.data.host() == false)
			{
				player.data.set_host(true);
				break;
			}
		}
	}

	this->_players.erase(player->uid);
	player->_currentRoom = nullptr;

	// 나간거 다른 사람에게 알려주기
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

void Room::ClientUpdate(PlayerRef player, Protocol::C_PLAY_UPDATE& pkt)
{
	WRITE_LOCK;

	if (player == nullptr)
		return;

	// 방 상태가 게임중이 아닐때
	if (this->_state != Protocol::eRoomState::ROOM_STATE_PLAY)
		return;

	// 플레이어가 방에 존재하지 않을때
	if (_players.find(player->uid) == _players.end())
		return;

	// player의 정보 업데이트
	_players[player->uid].data.CopyFrom(pkt.playerdata());
}

void Room::GameStart()
{
	WRITE_LOCK;

	if (_players.size() < 2)
		return;

	this->_state = Protocol::eRoomState::ROOM_STATE_PLAY;
	_gameTime = ::GetTickCount64();

	// Todo 시작지점 설정
	// Todo 플레이어 위치 설정
	// Todo 플레이어 상태 설정
	// Todo 플레이어 정보 보내주기
	// Todo 게임 시작 정보 보내주기


	// Todo 게임 시작
	JobRef job = Horang::MakeShared<UpdateJob>(this->shared_from_this());
	this->PushJob(job);
}

void Room::Update()
{
	WRITE_LOCK;

	if (this->_state != Protocol::eRoomState::ROOM_STATE_PLAY)
		return;

	// 사람이 없을 때
	if (_players.size() == 0)
	{
		this->Initialize();
		return;
	}

	// 현재 시간과 비교해 16ms (60fps) 이상 지났을 때만 업데이트
	auto currentTime = ::GetTickCount64();

	// 16ms 이상 지났을 때만 업데이트
	if (currentTime - _gameTime < 16)
	{
		JobRef job = Horang::MakeShared<UpdateJob>(this->shared_from_this());
		this->PushJob(job);
		return;
	}

	_gameTime = currentTime;

	// 이때 방에 존재하는 player들에게 player 정보를 보내줘야함
	{
		Protocol::S_PLAY_UPDATE packet;
		this->SetUpdatePacket(packet);


		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(packet);
		this->BroadCast(sendBuffer);
	}


	// 다시 Update Job 만들어서 대기열에 넣기
	{
		JobRef job = Horang::MakeShared<UpdateJob>(this->shared_from_this());
		this->PushJob(job);
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

	for (auto& [uid, player] : _players)
	{
		auto userInfo = roomInfo.add_users();
		player.player->GetUserInfo(userInfo);
	}
}

void Room::GetRoomInfo(Protocol::RoomInfo* roomInfo)
{
	this->GetRoomInfo(*roomInfo);
}

void Room::SetUpdatePacket(Protocol::S_PLAY_UPDATE& packet)
{
	this->GetRoomInfo(packet.mutable_roominfo());
	for (auto& [uid, playerData] : _players)
	{
		auto player = packet.add_playerdata();
		player->CopyFrom(playerData.data);
	}
}

void Room::GetPlayerData(Protocol::PlayerData& playerData, int32 uid)
{
	playerData.CopyFrom(_players[uid].data);
}

void Room::PushJob(JobRef job)
{
	WRITE_LOCK;
	_jobs.Push(job);
}

void Room::FlushJob()
{
	while (true)
	{
		JobRef job = _jobs.Pop();
		if (job == nullptr)
			break;

		job->Execute();
	}
}
