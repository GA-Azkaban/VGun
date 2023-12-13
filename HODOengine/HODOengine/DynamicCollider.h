#pragma once
#include "Collider.h"

namespace HDData
{
	class DynamicCollider : public Collider
	{
	public:
		DynamicCollider();

	public:
		// PhysX 씬과 데이터를 주고받는 함수
		void UpdateToPhysics() override;
		void UpdateFromPhysics();

	public:
		void Collide();
		bool GetIsCollided();

	private:
		bool _isCollided;

	};

}

