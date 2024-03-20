#include "PhysicsCollision.h"
#include <algorithm>

void HDData::PhysicsCollision::SwapObjects()
{
	std::swap(_thisActor, _otherActor);
	std::swap(_thisVelocity, _otherVelocity);
}
