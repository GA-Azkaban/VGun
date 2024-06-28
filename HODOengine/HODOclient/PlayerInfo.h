﻿#pragma once
#include "../HODOengine/HODO_API.h"
#include "PlayerState.h"

enum class eHITLOC
{
	NONE = 0,
	NO_HIT = 1,
	HEAD = 2,
	BODY = 3
};

class PlayerInfo : public HDData::Script
{
public:
	PlayerInfo();
	PlayerInfo(PlayerInfo* info);

public:
	void Start() override;
	void Update() override;

public:
	void Init();

public:
	void SetServerTransform(Vector3 pos, Quaternion rot);
	Vector3& GetServerPosition();
	Quaternion& GetServerRotation();


	void SetPlayerUID(int uid);
	void SetIsHost(bool isHost);
	void SetNickName(std::string nickName);

	void SetCurrentHP(int hp);
	void SetCurrentKill(int killCount);
	void SetCurrentDeath(int deathCount);
	void SetIsDie(bool isDie);
	void SetCurrentState(ePlayerState state);

	void SetCurrentBulletCount(int Count);

	int& GetPlayerUID();
	bool& GetIsHost();
	bool& GetPlayerDie();
	std::string& GetPlayerNickName();

	int& GetPlayerCurrentHP();
	int& GetPlayerKillCount();
	int& GetPlayerDeathCount();
	int& GetCurrentBulletCount();
	int GetMaxBulletCount();
	bool& GetIsDie();

	bool GetIsStateChange();
	void SetIsStateChange(bool isChange);
	ePlayerState GetPrevPlayerState();
	ePlayerState GetPlayerState();

	void SetIsMyInfo(bool isMine);
	bool GetIsMyInfo();

	void SetIsShoot(bool isShoot);
	void SetIsJump(bool isJump);
	bool GetIsShoot();
	bool GetIsJump();

	void SetParticleObj(HDData::ParticleSystem* particle);

private:
	bool _isMyInfo = false;

	// server info
	Vector3 _serverPos;
	Quaternion _serverRot;

	// player info
	int _playerUID;
	bool _isHost = false;
	std::string _playerNickname;

	// state info
	bool _isJump;
	bool _isShoot = false;

	int _currentHP = 70;
	bool _isDie;

	int _maxBulletCount = 6;
	int _bulletCount = 6;

	bool _isStateChange = false;
	ePlayerState _prevState = ePlayerState::NONE;
	ePlayerState _state = ePlayerState::IDLE;

	HDData::ParticleSystem* _shootParticle;
	
	// count info
	int _kill;
	int _death;
};

