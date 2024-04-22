#pragma once
#include "../HODOengine/MathHeader.h"

namespace HDData
{
	class PhysicsContactPoint
	{
	public:
		// 접촉점과 그 점에서의 노말
		Vector3 _contactPoint;
		Vector3 _normal;

		// 얼마나 떨어져 있는지. 음수면 겹쳐있다는 뜻.
		float _distance;
	};
}
