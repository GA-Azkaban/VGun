#include "PlayerTest.h"

PlayerTest::PlayerTest()
{

}

void PlayerTest::Start()
{
	_renderer = GetGameObject()->GetComponentInChildren<HDData::SkinnedMeshRenderer>();
}

void PlayerTest::Update()
{
	if (API::GetKeyDown(DIK_1))
	{
		_renderer->PlayAnimation("AR_aim", true);
	}

	if (API::GetKeyDown(DIK_2))
	{
		_renderer->PlayAnimation("AR_run_F", true);
	}

	if (API::GetKeyDown(DIK_3))
	{
		_renderer->PlayAnimation("AR_reload", true);
	}
}
