#pragma once
#include "dllExporter.h"
#include "DynamicCollider.h"

namespace HDData
{
	class HODO_API DynamicCapsuleCollider : public DynamicCollider
	{

	public:
		DynamicCapsuleCollider();

	public:
		virtual float GetWidth() const override;
		virtual float GetHeight() const override;
		virtual float GetDepth() const override;

	public:
		float GetRadius() const;
		float GetHalfHeight() const;
		void SetCapsuleInfo(float radius, float halfHeight);

		void SetRadius(float rad);
		void SetHalfHeight(float val);

	private:
		float _radius;
		float _halfHeight;
	};
}

