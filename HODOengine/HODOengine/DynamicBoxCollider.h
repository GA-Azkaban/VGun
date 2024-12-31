#pragma once
#include "dllExporter.h"
#include "DynamicCollider.h"

namespace HDData
{
	class HODO_API DynamicBoxCollider : public DynamicCollider
	{
	public:
		DynamicBoxCollider();
		DynamicBoxCollider(float w, float h, float d, int colliderGroup = 0);

		void SetVolume(float w, float h, float d);
		virtual float GetWidth() const override;
		virtual float GetHeight() const override;
		virtual float GetDepth() const override;

		virtual void Update() override;

		int GetColGroup() const;

	private:
		float _width;
		float _height;
		float _depth;
		int _colliderGroup;
		HDEngine::CubePrimitive* _cubeDebugStruct;
	};
}


