#pragma once

#include "dllExporter.h"
#include "StaticCollider.h"

namespace HDData
{
	class HODO_API TriggerBoxCollider : public StaticCollider
	{
	public:
		TriggerBoxCollider();
		TriggerBoxCollider(float width, float height, float depth);

	public:
		float GetHeight();
		float GetWidth();
		float GetDepth();

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

