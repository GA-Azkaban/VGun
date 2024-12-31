#pragma once
#include "Component.h"

namespace HDData
{
	class HODO_API Script : public Component
	{
	public:
		Script();
		virtual ~Script();

		virtual void OnEnable() override;
		virtual void OnDisable() override;
		virtual void Start() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void FixedUpdate() override;
		virtual void OnDestroy() override;
	};
}


