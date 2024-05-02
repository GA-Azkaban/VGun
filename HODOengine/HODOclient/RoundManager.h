﻿#pragma once
#include <unordered_map>

#include "../HODOengine/HODO_API.h"
#include "Weapon.h"
#include "PlayerInfo.h"

class RoundManager : public HDData::Script
{
public:
	RoundManager();
	~RoundManager() = default;

public:
	void Start() override;
	void Update() override;

public:
	void InitGame();
	void EndGame();

public:
	void InitRound();
	void UpdateRound();

public:
	//PlayerInfo* GetPlayerInfo(const int playerIndex);
	//PlayerInfo* GetPlayerInfo(const PlayerInfo& player);
	//PlayerInfo* GetPlayerInfo(const HDData::GameObject* object);

private:
	std::vector<PlayerInfo*> _playerInfo;

private:
	// 라운드 정보
	int _timer;			// 타이머

	int _R;				// 팀당 킬카운트
	int _G;
	int _B;

public:
	void InitRandomSpawn();
	void SetPlayerSpawn(int playerIndex);

private:
	std::vector<HDData::GameObject*> _randomSpawnPos;
};

