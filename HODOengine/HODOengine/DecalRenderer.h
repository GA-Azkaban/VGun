#pragma once
#include "RendererBase.h"
#include "dllExporter.h"
#include "../HODO3DGraphicsInterface/IDecal.h"

namespace HDData
{
	class Material;

	class HODO_API DecalRenderer : public RendererBase
	{
	public:
		DecalRenderer();
		~DecalRenderer();

		void LoadMaterial(HDData::Material* material);
		void SetColor(UINT r, UINT g, UINT b, UINT a);

	protected:
		virtual void OnEnable() override;
		virtual void OnDisable() override;
		virtual void OnDestroy() override;
		virtual void Update() override;

	private:
		HDEngine::IDecal* _decal;
	};
}
