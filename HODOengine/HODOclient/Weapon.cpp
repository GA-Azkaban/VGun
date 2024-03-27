#include "Weapon.h"

void Weapon::Update()
{
	auto skmRenderer = GetGameObject()->GetParentGameObject()->GetComponent<HDData::SkinnedMeshRenderer>();
	Matrix tm = skmRenderer->GetBoneTransformByNodeName("ik_hand_r");
	//Matrix tm = skmRenderer->GetBoneTransformByNodeName("root");
	GetGameObject()->GetTransform()->SetLocalTM(tm);
}
