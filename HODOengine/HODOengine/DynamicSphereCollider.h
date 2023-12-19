#pragma once
#include "DynamicCollider.h"

namespace HDData
{
	class DynamicSphereCollider : public DynamicCollider
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

	private:
		float _radius;
	};
}


