#include "PlayerTest.h"
#include "DecalPool.h"

PlayerTest::PlayerTest()
{

}

void PlayerTest::Start()
{
	_renderer = GetGameObject()->GetComponentInChildren<HDData::SkinnedMeshRenderer>();
}

void PlayerTest::Update()
{
	/*if (API::GetKeyDown(DIK_1))
	{
		_renderer->PlayAnimation("AR_aim", true);
	}

	if (API::GetKeyDown(DIK_2))
	{
		_renderer->PlayAnimationAtOnce("AR_dying", false, 0.1f, false);
	}

	if (API::GetKeyDown(DIK_3))
	{
		_renderer->PlayAnimation("AR_reload", false, 0.1f, false);
	}*/

	if (API::GetKeyDown(DIK_1))
	{
		DecalPool::Instance()->SummonDecal({10.0f, 0.0f, 0.0f});
	}
}
