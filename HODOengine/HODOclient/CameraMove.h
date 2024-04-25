#pragma once
#include "../HODOengine/HODO_API.h"

namespace HDData
{
	class Camera;
}


class CameraMove : public HDData::Script
{
public:
	CameraMove();

public:
	virtual void Update() override;

public:
	void OnMouseMove();

public:
	void Strafe(float delta);
	void Walk(float delta);
	void WorldUpDown(float delta);

	void Yaw(float angle);
	void Pitch(float angle);
	void Roll(float angle);
	void RotateY(float angle);

public:
	float moveSpeed;
	float rotateSpeed;

};
