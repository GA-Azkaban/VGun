#pragma once
#include "dllExporter.h"
#include "StaticCollider.h"

namespace HDData
{
	class HODO_API StaticBoxCollider : public StaticCollider
	{
	public:
		StaticBoxCollider();
		StaticBoxCollider(float width, float height, float depth);

	public:
		float GetHeight();
		float GetWidth();
		float GetDepth();

		void SetHeight(float val);
		void SetWidth(float val);
		void SetDepth(float val);

		virtual void DrawDebug() override;

	private:
		float _width;
		float _height;
		float _depth;
	};
}

