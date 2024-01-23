#pragma once
#include "dllExporter.h"
#include "DynamicCollider.h"

namespace HDData
{
	class HODO_API DynamicBoxCollider : public DynamicCollider
	{
	public:
		DynamicBoxCollider();

		void SetVolume(float w, float h, float d);
		virtual float GetWidth() const override;
		virtual float GetHeight() const override;
		virtual float GetDepth() const override;

		virtual void DrawDebug() override;

	private:
		float _width;
		float _height;
		float _depth;
	};
}


