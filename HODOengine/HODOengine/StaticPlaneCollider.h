#pragma once
#include "StaticCollider.h"


namespace HDData
{
	class StaticPlaneCollider : public StaticCollider
	{
	public:
		StaticPlaneCollider();

	public:
		void Start() override;
		void Update() override;
	};
}

