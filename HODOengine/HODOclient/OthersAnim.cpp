#include "OthersAnim.h"
#include "RoundManager.h"

OthersAnim::OthersAnim()
{

}

void OthersAnim::Start()
{
	auto a = GetGameObject();
	_mesh = GetGameObject()->GetComponentInChildren<HDData::SkinnedMeshRenderer>();
	_info = GetGameObject()->GetComponent<PlayerInfo>();
	_audio = SoundManager::Instance().AddAudioSourceInObject(this->GetGameObject());
	SoundManager::Instance().InitializePlayerAudio(_audio);
}

void OthersAnim::Update()
{
	if (!RoundManager::Instance()->GetIsRoundStart()) return;
	if (!_info->GetIsStateChange()) return;
	if (_info->GetIsDie())
	{
		_audio->Stop("3d_footstep");
		return;
	}

	switch (_info->GetPlayerState())
	{
		case ePlayerState::DIE:
		{
		}
		break;
		case ePlayerState::IDLE:
		{
			_mesh->PlayAnimation("RV_idle", true, 0.1, true, 0.1);
			_audio->Stop("3d_footstep");
		}
		break;
		case ePlayerState::FIRE:
		{
			_mesh->PlayAnimation("RV_fire", false, 0.1, true, 0.1);
			_audio->Play3DOnce("3d_fire");
		}
		break;
		case ePlayerState::WALK_R:
		{
			_mesh->PlayAnimation("RV_runR", true, 0.1, true, 0.1);
			_audio->Play3DRepeat("3d_footstep");
		}
		break;
		case ePlayerState::WALK_L:
		{
			_mesh->PlayAnimation("RV_runL", true, 0.1, true, 0.1);
			_audio->Play3DRepeat("3d_footstep");
		}
		break;
		case ePlayerState::WALK_F:
		{
			_mesh->PlayAnimation("RV_runF", true, 0.1, true, 0.1);
			_audio->Play3DRepeat("3d_footstep");
		}
		break;
		case ePlayerState::WALK_B:
		{
			_mesh->PlayAnimation("RV_runB", true, 0.1, true, 0.1);
			_audio->Play3DRepeat("3d_footstep");
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
		case ePlayerState::ROLL_F:
		{
			_mesh->PlayAnimation("RV_rollF", false, 0.1, true, 0.1);
		}
		break;
		case ePlayerState::ROLL_B:
		{
			_mesh->PlayAnimation("RV_rollB", false, 0.1, true, 0.1);
		}
		break;
		case ePlayerState::ROLL_R:
		{
			_mesh->PlayAnimation("RV_rollR", false, 0.1, true, 0.1);
		}
		break;
		case ePlayerState::ROLL_L:
		{
			_mesh->PlayAnimation("RV_rollL", false, 0.1, true, 0.1);
		}
		break;
		default:
			break;
	}

	_info->SetIsStateChange(false);
}
