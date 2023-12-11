#pragma once
#include "../HODOengine/Camera.h"

class MovableCamera : public hodoData::Camera
{
public:
	MovableCamera();

protected:
	virtual void Update() override;

private:
	float _moveSpeed;

};