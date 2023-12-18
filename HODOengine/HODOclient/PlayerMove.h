#pragma once
#include "../HODOengine/HODO_API.h"

/// <summary>
/// �÷��̾� �����Ӱ� ���õ� ��ũ��Ʈ
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
	// ���콺�� ���� ī�޶� ȸ�� üũ
	void Pitch();
	void Roll(); // roll �ʿ��Ѱ�? ������ ��¦ ����̴�...
	void Yaw();

private:
	float _moveSpeed;
	int _moveDirection;
	
};
