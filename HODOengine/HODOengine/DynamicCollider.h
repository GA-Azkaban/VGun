#pragma once
#include "Collider.h"

namespace hodoData
{
	class DynamicCollider : public Collider
	{
	public:
		DynamicCollider();

	public:
		// PhysX ���� �����͸� �ְ�޴� �Լ�
		void UpdateToPhysics() override;
		void UpdateFromPhysics();

	public:
		void Collide();
		bool GetIsCollided();

	private:
		bool _isCollided;

	};

}

