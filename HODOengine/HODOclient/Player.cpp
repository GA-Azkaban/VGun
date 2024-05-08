#include "Player.h"

void Player::Start()
{
	// 플레이어에 대한 데이터를 세팅
	_kill = 0;
	_death = 0;
	_weapon = eWeaponType::HANDGUN;
	_isDie = false;

	_skinnedMeshRenderer = GetGameObject()->GetComponentInChildren<HDData::SkinnedMeshRenderer>();
}

void Player::Update()
{
	if (_upperState == eUpperState::RELOAD)
	{
		if (_skinnedMeshRenderer->IsUpperAnimationEnd())
		{
			_upperState = eUpperState::IDLE;
		}
	}
	else
	{
		_upperState = eUpperState::IDLE;
	}

	if (_lowerState == eLowerState::JUMP)
	{
		if (_skinnedMeshRenderer->IsLowerAnimationEnd())
		{
			_lowerState = eLowerState::IDLE;
		}
	}
	else
	{
		_lowerState = eLowerState::IDLE;
	}

	// 키 입력 받기

	// 재장전
	if (API::GetKeyDown(DIK_U))
	{
		_upperState = eUpperState::RELOAD;
	}

	// 점프와 앉기
	if (API::GetKeyPressing(DIK_C))
	{
		// 점프 중일 때 앉을 수 없다
		if (_lowerState != eLowerState::JUMP)
		{
			// 장전 중일 때는 장전 중인 상체 상태 그대로
			if (_upperState != eUpperState::RELOAD)
			{
				_upperState = eUpperState::CROUCH;
			}
			_lowerState = eLowerState::CROUCH;
		}
	}
	else if (API::GetKeyDown(DIK_SPACE))
	{
		_lowerState = eLowerState::JUMP;
	}

	// 이동
	// 점프할 때는 이동 키를 눌러도 lowerState가 점프상태에서 변하지 않게 해야한다.
	if (_lowerState != eLowerState::JUMP)
	{
		if (API::GetKeyPressing(DIK_H))
		{
			// 앉은 상태
			if (_lowerState == eLowerState::CROUCH)
			{
				_lowerState = eLowerState::CROUCH_RUN_R;
			}
			else // 일어선 상태
			{
				if (_upperState != eUpperState::RELOAD)
				{
					_upperState = eUpperState::RUN;
				}
				_lowerState = eLowerState::RUN_R;
			}

		}
		if (API::GetKeyPressing(DIK_F))
		{
			// 앉은 상태
			if (_lowerState == eLowerState::CROUCH || _lowerState == eLowerState::CROUCH_RUN_R)
			{
				_lowerState = eLowerState::CROUCH_RUN_L;
			}
			else // 일어선 상태
			{
				if (_upperState != eUpperState::RELOAD)
				{
					_upperState = eUpperState::RUN;
				}
				_lowerState = eLowerState::RUN_L;
			}
		}

		if (API::GetKeyPressing(DIK_G))
		{
			// 앉은 상태
			if (_lowerState == eLowerState::CROUCH || _lowerState == eLowerState::CROUCH_RUN_R || _lowerState == eLowerState::CROUCH_RUN_L)
			{
				_lowerState = eLowerState::CROUCH_RUN_B;
			}
			else // 일어선 상태
			{
				if (_upperState != eUpperState::RELOAD)
				{
					_upperState = eUpperState::RUN;
				}
				_lowerState = eLowerState::RUN_B;
			}
		}

		if (API::GetKeyPressing(DIK_T))
		{
			// 앉은 상태
			if (_lowerState == eLowerState::CROUCH || _lowerState == eLowerState::CROUCH_RUN_R || _lowerState == eLowerState::CROUCH_RUN_L || _lowerState == eLowerState::CROUCH_RUN_B)
			{
				_lowerState = eLowerState::CROUCH_RUN_F;
			}
			else // 일어선 상태
			{
				if (_upperState != eUpperState::RELOAD)
				{
					_upperState = eUpperState::RUN;
				}
				_lowerState = eLowerState::RUN_F;
			}
		}
	}

	// 사격
	if (API::GetMouseHold(MOUSE_LEFT))
	{
		if (_upperState != eUpperState::RELOAD)
		{
			_upperState = eUpperState::SHOOT;
		}
		if (_lowerState == eLowerState::IDLE)
		{
			_lowerState = eLowerState::SHOOT;
		}
	}

	// 키 입력을 받아 바뀐 state에 맞는 애니메이션 출력
	switch (_upperState)
	{
		case eUpperState::IDLE:
			_skinnedMeshRenderer->PlayAnimationUpper("AR_aim", true);
			break;
		case eUpperState::RUN:
			_skinnedMeshRenderer->PlayAnimationUpper("AR_run", true);
			break;
		case eUpperState::SHOOT:
			_skinnedMeshRenderer->PlayAnimationUpper("AR_fire", false, 0.075f, true, 0.25f);
			break;
		case eUpperState::CROUCH:
			_skinnedMeshRenderer->PlayAnimationUpper("AR_crouchAim", true);
			break;
		case eUpperState::RELOAD:
			_skinnedMeshRenderer->PlayAnimationUpper("AR_reload", false, 0.2f, false);
			break;
		default:
			break;
	}
	switch (_lowerState)
	{
		case eLowerState::IDLE:
			_skinnedMeshRenderer->PlayAnimationLower("AR_aim", true);
			break;
		case eLowerState::RUN_F:
			_skinnedMeshRenderer->PlayAnimationLower("AR_run", true);
			break;
		case eLowerState::RUN_B:
			_skinnedMeshRenderer->PlayAnimationLower("X_runB", true);
			break;
		case eLowerState::RUN_R:
			_skinnedMeshRenderer->PlayAnimationLower("X_runR", true);
			break;
		case eLowerState::RUN_L:
			_skinnedMeshRenderer->PlayAnimationLower("X_runL", true);
			break;
		case eLowerState::CROUCH:
			_skinnedMeshRenderer->PlayAnimationLower("AR_crouch", true, 0.35f);
			break;
		case eLowerState::CROUCH_RUN_F:
			_skinnedMeshRenderer->PlayAnimationLower("X_crouchWalkF", true);
			break;
		case eLowerState::CROUCH_RUN_B:
			_skinnedMeshRenderer->PlayAnimationLower("X_crouchWalkB", true);
			break;
		case eLowerState::CROUCH_RUN_R:
			_skinnedMeshRenderer->PlayAnimationLower("X_crouchWalkR", true);
			break;
		case eLowerState::CROUCH_RUN_L:
			_skinnedMeshRenderer->PlayAnimationLower("X_crouchWalkL", true);
			break;
		case eLowerState::SHOOT:
			_skinnedMeshRenderer->PlayAnimationLower("AR_fire", false, 0.075f, true, 0.25f);
			break;
		case eLowerState::JUMP:
			_skinnedMeshRenderer->PlayAnimationLower("X_jump", false, 0.2f, false);
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
