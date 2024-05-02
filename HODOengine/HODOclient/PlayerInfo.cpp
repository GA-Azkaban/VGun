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

void PlayerInfo::SetCurrentHP(int hp)
{
	this->_HP = hp;
}

void PlayerInfo::SetCurrentState(ePlayerState state)
{
	this->_state = state;
}

bool PlayerInfo::GetIsDie()
{
	return _isDie;
}

int PlayerInfo::GetPlayerIndex()
{
	return _playerIndex;
}
