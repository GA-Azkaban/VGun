#pragma once
#include "dllExporter.h"
#include "DynamicCollider.h"
#include "StaticCollider.h"

namespace HDData
{
	class HODO_API ParticleSphereCollider : public StaticCollider
	{
	public:
		ParticleSphereCollider();

	public:
		float GetRadius() const;
		void SetRadius(float radius);
		void CheckTimer(float deltaTime);
		void SetTimerActive();

		virtual void Update() override;

	private:
		float _radius;
		HDEngine::SpherePrimitive* _sphereDebugStruct;
		float _duration;
	};
}
