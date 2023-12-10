#pragma once
#include "StaticCollider.h"


namespace hodoData
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

