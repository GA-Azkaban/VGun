#include "PlayerInfo.h"

PlayerInfo::PlayerInfo()
{

}

PlayerInfo::PlayerInfo(PlayerInfo* info)
{
	_playerUID = info->GetPlayerUID();
	_playerID = info->GetPlayerID();
	_teamID = info->GetPlayerTeam();
	_isHost = info->GetIsHost();
	_playerNickname = info->GetPlayerNickName();
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
	this-> _maxHP = 100;
	this-> _kill = 0;
	this-> _death = 0;
	this->_isDie = false;
	this->_bulletCount = 30;
	this->_state = ePlayerState::IDLE;
}

void PlayerInfo::SetPlayerUID(int uid)
{
	_playerUID = uid;
}

void PlayerInfo::SetPlayerID(std::string id)
{
	_playerID = id;
}

void PlayerInfo::PlayerIndex(int index)
{
	_playerIndex = index;
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

std::string& PlayerInfo::GetPlayerID()
{
	return _playerID;
}

bool& PlayerInfo::GetIsHost()
{
	return _isHost;
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

bool& PlayerInfo::GetIsDie()
{
	return _isDie;
}

ePlayerState& PlayerInfo::GetPlayerState()
{
	return _state;
}

void PlayerInfo::SetIsMyInfo(bool isMine)
{
	_isMyInfo = isMine;
}

bool PlayerInfo::GetIsMyInfo()
{
	return _isMyInfo;
}

void PlayerInfo::OtherPlayerShoot(eHITLOC loc)
{
	switch (loc)
	{
		case eHITLOC::NONE:
			break;
		case eHITLOC::NO_HIT:
			break;
		case eHITLOC::HEAD:
		{
			_currentHP -= 30;
		}
			break;
		case eHITLOC::BODY:
		{
			_currentHP -= 10;
		}
			break;
		default:
			break;
	}
}

int& PlayerInfo::GetPlayerIndex()
{
	return _playerIndex;
}

eTeam& PlayerInfo::GetPlayerTeam()
{
	return _teamID;
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
