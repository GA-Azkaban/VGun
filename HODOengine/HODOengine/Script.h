#pragma once
#include "Component.h"

namespace HDData
{
	class HODO_API Script : public Component
	{
	public:
		Script();
		virtual ~Script();

		virtual void Start() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void FixedUpdate() override;
	};
}


