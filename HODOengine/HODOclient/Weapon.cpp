#include "Weapon.h"

void Weapon::Update()
{
	auto skmRenderer = GetGameObject()->GetParentGameObject()->GetComponent<HDData::SkinnedMeshRenderer>();
	const Matrix& tm = skmRenderer->GetBoneTransformByNodeName("ik_hand_r");
	GetGameObject()->GetTransform()->SetWorldTM(tm);
}
