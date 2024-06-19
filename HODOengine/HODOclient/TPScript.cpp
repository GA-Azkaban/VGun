#include "TPScript.h"
#include "../HODOengine/HODO_API.h"
#include "RoundManager.h"

TPScript::TPScript()
{

}

void TPScript::Start()
{
	_animator = GetGameObject()->GetComponent<HDData::Animator>();
	_mesh = GetGameObject()->GetComponent<HDData::SkinnedMeshRenderer>();
	_info = GetGameObject()->GetComponent<PlayerInfo>();
}

void TPScript::Update()
{
	if (API::GetKeyPressing(DIK_W))
	{
		_animator->GetAllAC()->SetBool("isWalkFront", true);
	}
	if (API::GetKeyUp(DIK_W))
	{
		_animator->GetAllAC()->SetBool("isWalkFront", false);
	}
	if (API::GetKeyPressing(DIK_A))
	{
		_animator->GetAllAC()->SetBool("isWalkLeft", true);
	}
	if (API::GetKeyUp(DIK_A))
	{
		_animator->GetAllAC()->SetBool("isWalkLeft", false);
	}
	if (API::GetKeyPressing(DIK_S))
	{
		_animator->GetAllAC()->SetBool("isWalkBack", true);
	}
	if (API::GetKeyUp(DIK_S))
	{
		_animator->GetAllAC()->SetBool("isWalkBack", false);

	}
	if (API::GetKeyPressing(DIK_D))
	{
		_animator->GetAllAC()->SetBool("isWalkRight", true);

	}
	if (API::GetKeyUp(DIK_D))
	{
		_animator->GetAllAC()->SetBool("isWalkRight", false);

	}

	if (API::GetKeyDown(DIK_SPACE))
	{
		_animator->GetAllAC()->SetTrigger("isJump");
	}
	if (API::GetKeyDown(DIK_R))
	{
		_animator->GetAllAC()->SetTrigger("isReload");
	}
	if (API::GetMouseDown(MOUSE_LEFT))
	{
		_animator->GetAllAC()->SetTrigger("isFire");
	}


	if (!RoundManager::Instance()->GetIsRoundStart()) return;

	if (_info->GetPrevPlayerState() == _info->GetPlayerState()) return;

	switch (_info->GetPlayerState())
	{
		case ePlayerState::IDLE:
		{
			_mesh->PlayAnimation("AR_aim", true, 0.1, true, 0.1);
		}
		break;
		case ePlayerState::FIRE:
		{
			_mesh->PlayAnimation("AR_fire", false, 0.1, true, 0.1);
		}
		break;
		case ePlayerState::WALK_R:
		{
			_mesh->PlayAnimation("AR_run_R", true, 0.1, true, 0.1);
		}
		break;
		case ePlayerState::WALK_L:
		{
			_mesh->PlayAnimation("AR_run_L", true, 0.1, true, 0.1);
		}
		break;
		case ePlayerState::WALK_F:
		{
			_mesh->PlayAnimation("AR_run_F", true, 0.1, true, 0.1);
		}
		break;
		case ePlayerState::WALK_B:
		{
			_mesh->PlayAnimation("AR_run_B", true, 0.1, true, 0.1);
		}
		break;
		case ePlayerState::JUMP:
		{
			_mesh->PlayAnimation("AR_jump", false, 0.1, true, 0.1);
		}
		break;
		case ePlayerState::RELOAD:
		{
			_mesh->PlayAnimation("AR_reload", false, 0.1, true, 0.1);
		}
		break;
		case ePlayerState::ROLL:
		{
			_mesh->PlayAnimation("AR_roll", false, 0.1, true, 0.1);
		}
		break;
		case ePlayerState::DIE:
		{
			_mesh->PlayAnimation("AR_dying", false, 0.1, true, 0.1);
		}
		break;
		default:
			break;
	}
}
