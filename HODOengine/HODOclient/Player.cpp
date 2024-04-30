#include "Player.h"

void Player::Start()
{
	// 플레이어에 대한 데이터를 세팅
	_kill = 0;
	_death = 0;
	_weapon = eWeaponType::HANDGUN;
	_isDie = false;
}

void Player::Update()
{
	// Animation Test용 임시코드
	/*if (API::GetKeyDown(DIK_1))
	{
		GetGameObject()->GetComponentInChildren<HDData::SkinnedMeshRenderer>()->PlayAnimationUpper("AR_idle", true, 0.1f);
		GetGameObject()->GetComponentInChildren<HDData::SkinnedMeshRenderer>()->PlayAnimationLower("AR_crouch", true, 0.1f);
		//GetGameObject()->GetComponentInChildren<HDData::SkinnedMeshRenderer>()->PlayAnimation("AR_fire", false, true, 0.0f);
	}
	else if (API::GetKeyDown(DIK_2))
	{
		GetGameObject()->GetComponentInChildren<HDData::SkinnedMeshRenderer>()->PlayAnimationUpper("AR_fire", true, 0.1f);
		GetGameObject()->GetComponentInChildren<HDData::SkinnedMeshRenderer>()->PlayAnimationLower("AR_crouch", true, 0.1f);
	}
	else if (API::GetKeyDown(DIK_3))
	{
		GetGameObject()->GetComponentInChildren<HDData::SkinnedMeshRenderer>()->PlayAnimationUpper("AR_crouch", true);
		GetGameObject()->GetComponentInChildren<HDData::SkinnedMeshRenderer>()->PlayAnimationLower("AR_crouch", true);
	}
	else if (API::GetKeyDown(DIK_4))
	{
		GetGameObject()->GetComponentInChildren<HDData::SkinnedMeshRenderer>()->PlayAnimationUpper("HG_shoot", false);
		//GetGameObject()->GetComponentInChildren<HDData::SkinnedMeshRenderer>()->PlayAnimationLower("X_crouch", true);
	}
	else if (API::GetKeyDown(DIK_5))
	{
		//GetGameObject()->GetComponentInChildren<HDData::SkinnedMeshRenderer>()->PlayAnimationUpper("AR_idle", true);
		GetGameObject()->GetComponentInChildren<HDData::SkinnedMeshRenderer>()->PlayAnimationLower("AR_idle", true);
	}
	else if (API::GetKeyDown(DIK_6))
	{
		GetGameObject()->GetComponentInChildren<HDData::SkinnedMeshRenderer>()->PlayAnimationUpper("AR_fire", true);
		//GetGameObject()->GetComponentInChildren<HDData::SkinnedMeshRenderer>()->PlayAnimationLower("AR_idle", true);
	}
	else if (API::GetKeyDown(DIK_7))
	{
		GetGameObject()->GetComponentInChildren<HDData::SkinnedMeshRenderer>()->PlayAnimationUpper("AR_reload", false);
		GetGameObject()->GetComponentInChildren<HDData::SkinnedMeshRenderer>()->PlayAnimationLower("AR_idle", true);
	} */


	_upperState = eUpperState::IDLE;
	_lowerState = eLowerState::IDLE;

	if (API::GetKeyPressing(DIK_C))
	{
		_upperState = eUpperState::CROUCH;
		_lowerState = eLowerState::CROUCH;
	}
	else if (API::GetKeyPressing(DIK_F))
	{
		_upperState = eUpperState::RUN;
		_lowerState = eLowerState::RUN;
	}

	if (API::GetMouseHold(MOUSE_LEFT))
	{
		_upperState = eUpperState::SHOOT;
		if (_lowerState == eLowerState::IDLE)
		{
			_lowerState = eLowerState::SHOOT;
		}
	}

	switch (_upperState)
	{
		case eUpperState::IDLE:
			GetGameObject()->GetComponentInChildren<HDData::SkinnedMeshRenderer>()->PlayAnimationUpper("AR_idle", true);
			break;
		case eUpperState::RUN:
			GetGameObject()->GetComponentInChildren<HDData::SkinnedMeshRenderer>()->PlayAnimationUpper("AR_run", true);
			break;
		case eUpperState::SHOOT:
			GetGameObject()->GetComponentInChildren<HDData::SkinnedMeshRenderer>()->PlayAnimationUpper("AR_fire", false, 0.075f, true, 0.18f);
			break;
		case eUpperState::CROUCH:
			GetGameObject()->GetComponentInChildren<HDData::SkinnedMeshRenderer>()->PlayAnimationUpper("AR_crouch", true);
			break;
		default:
			break;
	}
	switch (_lowerState)
	{
		case eLowerState::IDLE:
			GetGameObject()->GetComponentInChildren<HDData::SkinnedMeshRenderer>()->PlayAnimationLower("AR_idle", true);
			break;
		case eLowerState::RUN:
			GetGameObject()->GetComponentInChildren<HDData::SkinnedMeshRenderer>()->PlayAnimationLower("AR_run", true);
			break;
		case eLowerState::SHOOT:
			GetGameObject()->GetComponentInChildren<HDData::SkinnedMeshRenderer>()->PlayAnimationLower("AR_fire", false, 0.075f, true, 0.18f);
			break;
		case eLowerState::CROUCH:
			GetGameObject()->GetComponentInChildren<HDData::SkinnedMeshRenderer>()->PlayAnimationLower("AR_crouch", true);
			break;
		default:
			break;
	}
}

void Player::LateUpdate()
{

}

int Player::GetPlayerIndex()
{
	return _playerIndex;
}

int Player::GetTeamIndex()
{
	return _teamID;
}

int Player::GetHealth()
{
	return _health;
}

int Player::GetSpeed()
{
	return _speed;
}

int Player::GetKillCount()
{
	return _kill;
}

int Player::GetDeathCount()
{
	return _death;
}

eWeaponType Player::GetWeaponNow()
{
	return _weapon;
}

int Player::GetBulletCount()
{
	return _bulletCount;
}

void Player::ChangeWeapon(eWeaponType type)
{
	_weapon = type;
}
