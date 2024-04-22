#pragma once
#include "dllExporter.h"
#include "DynamicCollider.h"

namespace HDData
{
	class HODO_API DynamicSphereCollider : public DynamicCollider
	{
	public:
		DynamicSphereCollider();
		DynamicSphereCollider(float rad, bool isPlayer);

	public:
		virtual float GetWidth() const override;
		virtual float GetHeight() const override;
		virtual float GetDepth() const override;

	public:
		float GetRadius() const;
		void SetRadius(float radius);
		bool GetIsPlayer() const;

		virtual void DrawDebug() override;

	private:
		float _radius;
		bool _isPlayer;
		HDEngine::SpherePrimitive* _sphereDebugStruct;
	};
}


