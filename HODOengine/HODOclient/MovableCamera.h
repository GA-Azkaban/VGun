#pragma once
#include "../HODOengine/Camera.h"

class MovableCamera : public HDData::Camera
{
public:
	MovableCamera();

protected:
	virtual void Update() override;

private:
	float _moveSpeed;

};