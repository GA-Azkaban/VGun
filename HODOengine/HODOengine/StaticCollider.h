#pragma once
#include "dllExporter.h"
#include "Collider.h"

namespace HDData
{
	class HODO_API StaticCollider : public Collider
	{
	public:
		StaticCollider();

	public:
		void UpdateToPhysics() override;

	public:
		void Collided();
		bool GetIsCollided();

	private:
		bool _isCollided;
	};
}

