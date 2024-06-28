#include "PlayerInfo.h"
#include "GameManager.h"

PlayerInfo::PlayerInfo()
{

}

PlayerInfo::PlayerInfo(PlayerInfo* info)
{
	_playerUID = info->GetPlayerUID();
	_isHost = info->GetIsHost();
	_playerNickname = info->GetPlayerNickName();
}

void PlayerInfo::Start()
{
	this->Init();
}

void PlayerInfo::Update()
{
	if (_isShoot)
	{
		//_shootParticle->Play();
		_isShoot = false;
	}
}

void PlayerInfo::Init()
{
	this-> _kill = 0;
	this-> _death = 0;
	this->_isDie = false;
	this->_bulletCount = 6;
	this->_currentHP = 70;
	this->_state = ePlayerState::IDLE;
}

void PlayerInfo::SetServerTransform(Vector3 pos, Quaternion rot)
{
	_serverPos = pos;
	_serverRot = rot;
}

Vector3& PlayerInfo::GetServerPosition()
{
	return _serverPos;
}

Quaternion& PlayerInfo::GetServerRotation()
{
	return _serverRot;
}

void PlayerInfo::SetPlayerUID(int uid)
{
	_playerUID = uid;
}

void PlayerInfo::SetCurrentHP(int hp)
{
	_currentHP = hp;
}

void PlayerInfo::SetCurrentKill(int killCount)
{
	_kill = killCount;
}

void PlayerInfo::SetCurrentDeath(int deathCount)
{
	_death = deathCount;
}

void PlayerInfo::SetIsDie(bool isDie)
{
	_isDie = isDie;
}

void PlayerInfo::SetCurrentState(ePlayerState state)
{
	this->_prevState = _state;
	this->_state = state;
}

void PlayerInfo::SetCurrentBulletCount(int count)
{
	_bulletCount = count;
}

int& PlayerInfo::GetPlayerUID()
{
	return _playerUID;
}

bool& PlayerInfo::GetIsHost()
{
	return _isHost;
}

void PlayerInfo::SetIsHost(bool isHost)
{
	_isHost = isHost;
}

void PlayerInfo::SetNickName(std::string nickName)
{
	_playerNickname = nickName;
}

bool& PlayerInfo::GetIsDie()
{
	return _isDie;
}

bool PlayerInfo::GetIsStateChange()
{
	return _isStateChange;
}

void PlayerInfo::SetIsStateChange(bool isChange)
{
	_isStateChange = isChange;
}

ePlayerState PlayerInfo::GetPrevPlayerState()
{
	return _prevState;
}

ePlayerState PlayerInfo::GetPlayerState()
{
	return _state;
}

void PlayerInfo::SetIsMyInfo(bool isMine)
{
	_isMyInfo = isMine;
	GameManager::Instance()->SetMyInfo(this);
}

bool PlayerInfo::GetIsMyInfo()
{
	return _isMyInfo;
}

void PlayerInfo::SetIsShoot(bool isShoot)
{
	_isShoot = isShoot;
}

void PlayerInfo::SetIsJump(bool isJump)
{
	_isJump = isJump;
}

bool PlayerInfo::GetIsShoot()
{
	return _isShoot;
}

bool PlayerInfo::GetIsJump()
{
	return _isJump;
}

void PlayerInfo::SetParticleObj(HDData::ParticleSystem* particle)
{
	_shootParticle = particle;
}

bool& PlayerInfo::GetPlayerDie()
{
	return _isDie;
}

std::string& PlayerInfo::GetPlayerNickName()
{
	return _playerNickname;
}

int& PlayerInfo::GetPlayerCurrentHP()
{
	return _currentHP;
}

int& PlayerInfo::GetPlayerKillCount()
{
	return _kill;
}

int& PlayerInfo::GetPlayerDeathCount()
{
	return _death;
}

int& PlayerInfo::GetCurrentBulletCount()
{
	return _bulletCount;
}

int PlayerInfo::GetMaxBulletCount()
{
	return _maxBulletCount;
}
