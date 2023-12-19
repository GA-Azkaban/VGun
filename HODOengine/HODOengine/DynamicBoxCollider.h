#pragma once
#include "DynamicCollider.h"

namespace HDData
{
	class DynamicBoxCollider : public DynamicCollider
	{
	public:
		DynamicBoxCollider();

		void SetVolume(float w, float h, float d);
		virtual float GetWidth() const override;
		virtual float GetHeight() const override;
		virtual float GetDepth() const override;

	private:
		float _width;
		float _height;
		float _depth;
	};
}


