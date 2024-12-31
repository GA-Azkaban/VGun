#pragma once
#include "../HODOengine/HODO_API.h"

/// <summary>
/// 플레이어 그래픽스 디버깅용 코드
/// </summary>

class PlayerTest : public HDData::Script
{
public:
	PlayerTest();

public:
	void Start() override;
	void Update() override;

private:
	HDData::SkinnedMeshRenderer* _renderer;
};
