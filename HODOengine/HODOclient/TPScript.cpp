#include "TPScript.h"
#include "../HODOengine/HODO_API.h"
#include "RoundManager.h"

TPScript::TPScript()
{

}

void TPScript::Start()
{
	_mesh = GetGameObject()->GetComponent<HDData::SkinnedMeshRenderer>();
	_info = GetGameObject()->GetComponent<PlayerInfo>();
}

void TPScript::Update()
{
	if (!RoundManager::Instance()->GetIsRoundStart()) return;

	if (_info->GetPrevPlayerState() == _info->GetPlayerState()) return;

	switch (_info->GetPlayerState())
	{
		case ePlayerState::IDLE:
		{
			_mesh->PlayAnimation("AR_aim", true, 0.1, true, 0.1);
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
