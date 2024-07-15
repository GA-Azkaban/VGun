#pragma once

#include "dllExporter.h"
#include "StaticCollider.h"
#include "DynamicCollider.h"

namespace HDData
{
	//class HODO_API TriggerBoxCollider : public StaticCollider
	class HODO_API TriggerBoxCollider : public DynamicCollider
	{
	public:
		TriggerBoxCollider();
		TriggerBoxCollider(float width, float height, float depth);

	public:
		virtual float GetWidth() const override;
		virtual float GetHeight() const override;
		virtual float GetDepth() const override;

		void SetHeight(float val);
		void SetWidth(float val);
		void SetDepth(float val);

		virtual void Update() override;

	private:
		float _width;
		float _height;
		float _depth;
		HDEngine::CubePrimitive* _cubeDebugStruct;
	};
}

