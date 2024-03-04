#pragma once
#include "dllExporter.h"
#include "DynamicCollider.h"
#include "..\\HODO3DGraphicsInterface\\IDebugObject.h"

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
		virtual void Update() override;
		virtual void DrawDebug() override;

		virtual void OnEnable() override;
		virtual void OnDisable() override;

		void SetColor(Vector4& color);
		void SetFillModeSolid();
		void SetFillModeWireframe();

	private:
		HDEngine::IDebugObject* _capsuleObject;
	};
}

