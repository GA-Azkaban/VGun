#pragma once
#include "RendererBase.h"
#include "dllExporter.h"
#include "..\\HODO3DGraphicsInterface\\IStaticMesh.h"

namespace HDData
{
	class HODO_API MeshRenderer : public RendererBase
	{
	public:
		MeshRenderer();

		void LoadMesh(const std::string& fileName);
		void LoadNormalMap(const std::string& fileName);
		void LoadDiffuseMap(const std::string& fileName);

	protected:
		virtual void UpdateRenderData() override;
		virtual void OnEnable() override;
		virtual void OnDisable() override;

	private:
		HDEngine::IStaticMesh* _staticMesh;
	};
}
