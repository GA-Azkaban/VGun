#pragma once
#include "dllExporter.h"
#include "StaticCollider.h"

namespace HDData
{
	class HODO_API StaticBoxCollider : public StaticCollider
	{
	public:
		StaticBoxCollider();

	public:
		void Start() override;
		void Update() override;

	public:
		float GetHeight();
		float GetWidth();
		float GetDepth();

		void SetHeight(float val);
		void SetWidth(float val);
		void SetDepth(float val);

	private:
		float _width;
		float _height;
		float _depth;
	};
}

