#pragma once
#include "../HODOengine/HODO_API.h"
#include "PlayerInfo.h"

class OthersAnim : public HDData::Script
{
public:
	OthersAnim();

public:
	void Start() override;
	void Update() override;

private:
	HDData::SkinnedMeshRenderer* _mesh;
	PlayerInfo* _info;
	HDData::AudioSource* _audio;
};

