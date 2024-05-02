#pragma once
#include "dllExporter.h"
#include "StaticCollider.h"

namespace HDData
{
	class HODO_API StaticSphereCollider : public StaticCollider
	{
	public:
		StaticSphereCollider();
		StaticSphereCollider(float radius);

	public:
		float GetRadius();

		void SetRadius(float radius);

		virtual void DrawDebug() override;

	private:
		float _radius;
		HDEngine::SpherePrimitive* _sphereDebugStruct;
	};
}
