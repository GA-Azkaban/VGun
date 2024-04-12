#include "pch.h"
#include "Player.h"
#include "Room.h"

void Player::GetUserInfo(Protocol::UserInfo* userInfo)
{
	userInfo->set_id(id);
	userInfo->set_nickname(nickname);
}

void Player::GetUserInfo(Protocol::UserInfo& userInfo)
{
	this->GetUserInfo(&userInfo);
}
