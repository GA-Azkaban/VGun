#pragma once
#include "../HODOengine/HODO_API.h"
#include "PlayerInfo.h"

class TPScript : public HDData::Script
{
public:
	TPScript();

public:
	void Start() override;
	void Update() override;

private:
	HDData::Animator* _animator;
	HDData::SkinnedMeshRenderer* _mesh;
	PlayerInfo* _info;
};

