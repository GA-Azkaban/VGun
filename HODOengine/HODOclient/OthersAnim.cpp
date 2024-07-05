#include "OthersAnim.h"
#include "RoundManager.h"

OthersAnim::OthersAnim()
{

}

void OthersAnim::Start()
{
	_mesh = GetGameObject()->GetComponentInChildren<HDData::SkinnedMeshRenderer>();
	_info = GetGameObject()->GetComponent<PlayerInfo>();
	_audio = GetGameObject()->GetComponent<HDData::AudioSource>();
}

void OthersAnim::Update()
{
	if (!RoundManager::Instance()->GetIsRoundStart()) return;
	if (!_info->GetIsStateChange()) return;

	if(_info->GetIsShoot())
	{
		_audio->Play3DOnce("shootother", GetGameObject()->GetTransform()->GetPosition());
	}

	switch (_info->GetPlayerState())
	{
		case ePlayerState::IDLE:
		{
			_mesh->PlayAnimation("RV_idle", true, 0.1, true, 0.1);
			_audio->Stop("walkother");
		}
		break;
		case ePlayerState::FIRE:
		{
			_mesh->PlayAnimation("RV_fire", false, 0.1, true, 0.1);
		}
		break;
		case ePlayerState::WALK_R:
		{
			_mesh->PlayAnimation("RV_runR", true, 0.1, true, 0.1);
			_audio->Play3DRepeat("walkother", GetGameObject()->GetTransform()->GetPosition());
		}
		break;
		case ePlayerState::WALK_L:
		{
			_mesh->PlayAnimation("RV_runL", true, 0.1, true, 0.1);
			_audio->Play3DRepeat("walkother", GetGameObject()->GetTransform()->GetPosition());
		}
		break;
		case ePlayerState::WALK_F:
		{
			_mesh->PlayAnimation("RV_runF", true, 0.1, true, 0.1);
			_audio->Play3DRepeat("walkother", GetGameObject()->GetTransform()->GetPosition());
		}
		break;
		case ePlayerState::WALK_B:
		{
			_mesh->PlayAnimation("RV_runB", true, 0.1, true, 0.1);
			_audio->Play3DRepeat("walkother", GetGameObject()->GetTransform()->GetPosition());
		}
		break;
		case ePlayerState::JUMP:
		{
			_mesh->PlayAnimation("RV_jump", false, 0.1, true, 0.1);
		}
		break;
		case ePlayerState::RELOAD:
		{
			_mesh->PlayAnimation("RV_reload", false, 0.1, true, 0.1);
		}
		break;
		case ePlayerState::ROLL:
		{
			_mesh->PlayAnimation("RV_roll", false, 0.1, true, 0.1);
		}
		break;
		case ePlayerState::DIE:
		{
			_mesh->PlayAnimation("RV_dying", false, 0.1, true, 0.1);
		}
		break;
		default:
			break;
	}

	_info->SetIsStateChange(false);
}
