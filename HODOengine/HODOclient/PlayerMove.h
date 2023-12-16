#pragma once
#include "../HODOengine/HODO_API.h"

/// <summary>
/// 플레이어 움직임과 관련된 스크립트
/// </summary>

class PlayerMove : public HDData::Script
{
public:
	PlayerMove();

public:
	void Start() override;
	void Update() override;


private:
	void CheckLookDirection();
	void Move(int direction);

private:
	// 마우스에 따른 카메라 회전 체크
	void Pitch();
	void Roll(); // roll 필요한가? 옆으로 살짝 기울이는...
	void Yaw();

private:
	float _moveSpeed;
	int _moveDirection;
	
};
