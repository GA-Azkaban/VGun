#pragma once
#include "StaticCollider.h"


namespace HDData
{
	class StaticPlaneCollider : public StaticCollider
	{
	public:
		StaticPlaneCollider();

	public:
		void Update() override;

	public:
		// plane angle
		HDMaths::HDFLOAT3 GetNormalVector() const;
		// how far from bottom
		float GetDistance() const;
	};
}

