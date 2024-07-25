#pragma once
#include "../HODOengine/HODO_API.h"
#include "PlayerState.h"
#include "SoundManager.h"
#include "Timer.h"

enum class eHITLOC
{
	NONE = 0,
	NO_HIT = 1,
	HEAD = 2,
	BODY = 3
};

enum class eMeshType
{
	NONE,
	COWBOY,
	COWGIRL,
	BADGUY,
	GUNMAN,
	WORKINGGIRL,

	LAST
};

class Crosshair;
class HitEffect;
class IndicatorPool;
class DamageLog;

class PlayerInfo : public HDData::Script
{
public:
	PlayerInfo();
	PlayerInfo(PlayerInfo* info);

public:
	void Start() override;
	void Update() override;

public:
	void GetData(PlayerInfo* info);
	void Init();

public:
	void SetServerTransform(Vector3 pos, Quaternion rot);
	Vector3& GetServerPosition();
	Quaternion& GetServerRotation();
	void SetIsInterpolation(bool isInterpolation);
	bool GetIsInterpolation();

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
	void SetIsRespawn(bool isRespawn);
	bool GetIsRespawn();

	bool& GetIsStateChange();
	void SetIsStateChange(bool isChange);
	ePlayerState GetPrevPlayerState();
	ePlayerState GetPlayerState();

	void SetIsMyInfo(bool isMine);
	bool GetIsMyInfo();

	void SetIsShoot(bool isShoot);
	void SetIsJump(bool isJump);
	bool GetIsShoot();
	bool GetIsJump();

	void SetHitEffectObj(HitEffect* hitEffect);
	void SetParticleSystem(HDData::ParticleSystem* particleSystem);
	void PlayerAttacked(Vector3 targetPos);

	void AddSerialKillCount();
	void SetKillEffectImg(HDData::ImageUI* img);
	void SetDieEffectImg(HDData::ImageUI* img);
	void PlayHeadShotEffect();
	void PlayDieEffect();
	void PlayRespawnEffect();
	void SetLogUI(HDData::TextUI* txt);
	void PlayKillLog(std::string log);
	void KillLogExit();

	void SetCrosshairUI(Crosshair* crosshair);

	void SetDamageLogUI(DamageLog* damageLog);
	void DisplayDamageLog(std::string nickname, int damage, int remainHP);

private:
	bool _isMyInfo = false;

	// server info
	Vector3 _serverPos;
	Quaternion _serverRot;
	bool _isInterpolation = false;

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

	// count info
	int _kill;
	int _death;
	bool _isRespawn;

	// serial kill

	HitEffect* _hitEffect;
	HDData::ParticleSystem* _particleSystem;
	HDData::ImageUI* _killEffectImg;
	HDData::ImageUI* _dieEffectImg;
	HDData::TextUI* _killLog;

	Crosshair* _crosshair;
	DamageLog* _damageLog;

public:
	eMeshType type = eMeshType::NONE;
	HDData::AudioSource* audio;
	int _serialkillcount;
};

