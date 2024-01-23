#pragma once
#include "dllExporter.h"
#include "DynamicCollider.h"

namespace HDData
{
	class HODO_API DynamicSphereCollider : public DynamicCollider
	{
	public:
		DynamicSphereCollider();

	public:
		virtual float GetWidth() const override;
		virtual float GetHeight() const override;
		virtual float GetDepth() const override;

	public:
		float GetRadius() const;
		void SetRadius(float radius);

		virtual void DrawDebug() override;

	private:
		float _radius;
	};
}


