﻿#pragma once
#include "dllExporter.h"
#include "DynamicCollider.h"

namespace HDData
{
	class HODO_API DynamicCapsuleCollider : public DynamicCollider
	{
	public:
		DynamicCapsuleCollider();
		DynamicCapsuleCollider(float rad, float halfHeight, int colFilterNum = 0);

	public:
		virtual float GetWidth() const override;
		virtual float GetHeight() const override;
		virtual float GetDepth() const override;

		virtual void Update() override;

	public:
		float GetRadius() const;
		float GetHalfHeight() const;

	public:
		void SetRadius(float val);
		void SetHalfHeight(float val);

	private:
		float _radius;
		float _halfHeight;
		HDEngine::CapsulePrimitive* _capsuleObject;
	};
}

