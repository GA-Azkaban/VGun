#include "FSMtestScript.h"



FSMtestScript::FSMtestScript()
{
}

void FSMtestScript::Start()
{
	_animator = GetGameObject()->GetComponent<HDData::Animator>();
	_currentState = ePlayerState::IDLE;
}

void FSMtestScript::Update()
{
	if (API::GetKeyPressing(DIK_I))
	{
		_currentState = ePlayerState::WALK;
	}
	if (API::GetKeyUp(DIK_I))
	{
		_currentState = ePlayerState::IDLE;
	}
	if (API::GetKeyDown(DIK_SPACE))
	{
		_currentState = ePlayerState::JUMP;
	}

	switch (_currentState)
	{
		case FSMtestScript::ePlayerState::IDLE:
			_animator->SetBool("isWalk", false);
			break;
		case FSMtestScript::ePlayerState::WALK:
			_animator->SetBool("isWalk", true);
			break;
		case FSMtestScript::ePlayerState::JUMP:
			_animator->SetTrigger("isJump", true);
			break;
		case FSMtestScript::ePlayerState::SIT:
			break;
		case FSMtestScript::ePlayerState::LAST:
			break;
		default:
			break;
	}
}
