#pragma once
#include "../HODOengine/MathHeader.h"
#include "PhysicsContactPoint.h"

namespace HDData
{
	class Collider;
}

namespace HDData
{
	class PhysicsCollision
	{
	public:
		// 충돌 점의 최대 개수를 지정해둠.
		enum { MAX_CONTACT_POINTS = 8 };

	public:
		void SwapObjects();

		Vector3 _impulse;
		Vector3 _thisVelocity;
		Vector3 _otherVelocity;

		// 유효한 충돌점의 숫자. 위에서 정한 MAX 보다 적어야 함.
		int _contactsCount;

		Collider* _thisActor = nullptr;
		Collider* _otherActor = nullptr;

		PhysicsContactPoint _contactPoints[MAX_CONTACT_POINTS];

		unsigned int _activeContactCount = 0;
	};
}
