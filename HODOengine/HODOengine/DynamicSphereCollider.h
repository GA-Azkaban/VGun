#pragma once
#include "dllExporter.h"
#include "DynamicCollider.h"

namespace HDData
{
	class HODO_API DynamicSphereCollider : public DynamicCollider
	{
	public:
		DynamicSphereCollider();
		DynamicSphereCollider(float rad, int colFilterNum = 0);

	public:
		virtual float GetWidth() const override;
		virtual float GetHeight() const override;
		virtual float GetDepth() const override;

	public:
		float GetRadius() const;
		void SetRadius(float radius);

		virtual void Update() override;

		virtual void OnCollisionEnter(HDData::PhysicsCollision** colArr, unsigned int count) override;

	private:
		float _radius;
		HDEngine::SpherePrimitive* _sphereDebugStruct;
	};
}


