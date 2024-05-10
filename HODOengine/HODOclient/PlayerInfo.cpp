#include "PlayerInfo.h"

PlayerInfo::PlayerInfo()
{

}

void PlayerInfo::Start()
{
	this->Init();
}

void PlayerInfo::Update()
{

}

void PlayerInfo::Init()
{
	this-> _HP = 100;
	this-> _kill = 0;
	this-> _death = 0;
	this->_isDie = false;
	this->_bulletCount = 0;
	this->_state = ePlayerState::IDLE;
}

void PlayerInfo::SetPlayerID(std::string id)
{
	_playerID = id;
}

void PlayerInfo::PlayerIndex(int index)
{

}

void PlayerInfo::SetCurrentHP(int hp)
{
	this->_HP = hp;
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
	this->_state = state;
}

void PlayerInfo::SetCurrentBulletCount(int count)
{
	_bulletCount = count;
}

std::string PlayerInfo::GetPlayerID()
{
	return _playerID;
}

void PlayerInfo::SetTeamID(eTeam team)
{
	_teamID = team;
}

void PlayerInfo::SetIsHost(bool isHost)
{
	_isHost = isHost;
}

void PlayerInfo::SetNickName(std::string nickName)
{
	_playerNickname = nickName;
}

bool PlayerInfo::GetIsDie()
{
	return _isDie;
}

ePlayerState PlayerInfo::GetPlayerState()
{
	return _state;
}

int PlayerInfo::GetPlayerIndex()
{
	return _playerIndex;
}

eTeam PlayerInfo::GetPlayerTeam()
{
	return _teamID;
}

bool PlayerInfo::GetPlayerDie()
{
	return _isDie;
}

std::string PlayerInfo::GetPlayerNickName()
{
	return _playerNickname;
}

int PlayerInfo::GetPlayerHP()
{
	return _HP;
}

int PlayerInfo::GetPlayerKillCount()
{
	return _kill;
}

int PlayerInfo::GetPlayerDeathCount()
{
	return _death;
}
