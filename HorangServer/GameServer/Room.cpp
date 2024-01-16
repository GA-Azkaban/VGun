#include "pch.h"
#include "Room.h"
#include "Player.h"
#include "GameSession.h"

Room GRoom;

void Room::Enter(PlayerRef player)
{
	WRITE_LOCK;
	_players[player->playerId] = player;
}

void Room::Leave(PlayerRef player)
{
	WRITE_LOCK;
	_players.erase(player->playerId);
}

void Room::BroadCast(Horang::SendBufferRef sendBuffer)
{
	WRITE_LOCK;
	for (auto& p : _players)
	{
		p.second->ownerGameSession->Send(sendBuffer);
	}
}
