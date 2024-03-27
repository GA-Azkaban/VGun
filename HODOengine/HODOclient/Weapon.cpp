#include "Weapon.h"

void Weapon::Update()
{
	auto skmRenderer = GetGameObject()->GetParentGameObject()->GetComponent<HDData::SkinnedMeshRenderer>();
	Matrix tm = skmRenderer->GetBoneTransformByNodeName("hand_r");
	//Matrix tm = skmRenderer->GetBoneTransformByNodeName("pelvis");
	GetGameObject()->GetTransform()->SetLocalTM(tm);
}
