#include "pch.h"
#include "Room.h"
#include "Player.h"
#include "GameSession.h"

Room GRoom;

Room::Room()
{
	
}

void Room::Initialize()
{
	this->_players.clear();
	this->_jobs.Clear();
	this->_roomCode = "0000";
}

bool Room::Enter(PlayerRef player)
{
	//WRITE_LOCK;
	if (_players.size() > 6)
	{
		// Todo 실패 패킷
		return false;
	}

	_players[player->uid] = player;
	player->_currentRoom = this->shared_from_this();

	// Todo 성공 패킷

	return true;
}

bool Room::Leave(PlayerRef player)
{
	//WRITE_LOCK;
	_players.erase(player->uid);
	player->_currentRoom = nullptr;

	return true;
}

void Room::BroadCast(Horang::SendBufferRef sendBuffer)
{
	//WRITE_LOCK;
	for (auto& [id, player] : _players)
	{
		player->ownerGameSession->Send(sendBuffer);
	}
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
