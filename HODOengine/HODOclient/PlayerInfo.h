#pragma once
#include "../HODOengine/HODO_API.h"
#include "PlayerState.h"

class PlayerInfo : public HDData::Script
{
public:
	PlayerInfo();


public:
	void Init();

public:
	struct Stats 
	{
		int playerIndex;

		int health;

		bool damageHead;
		bool damageBody;

	};

private:
	int _playerIndex;
	bool _isHost;

	int _health;

	int _kill;
	int _death;

	ePlayerState state;
};

