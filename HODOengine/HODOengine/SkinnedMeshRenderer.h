#pragma once
#include "RendererBase.h"
#include "dllExporter.h"
#include "..\\HODO3DGraphicsInterface\\ISkinnedMesh.h"

namespace HDData
{
	class HODO_API SkinnedMeshRenderer : public RendererBase
	{
	public:
		SkinnedMeshRenderer();

		void LoadMesh(const std::string& fileName);
		void LoadNormalMap(const std::string& fileName);
		void LoadDiffuseMap(const std::string& fileName);
		void LoadVertexShader(const std::string& fileName);
		void LoadPixelShader(const std::string& fileName);
		void PlayAnimation(const std::string& fileName, bool isLoop = true);

	protected:
		virtual void UpdateRenderData() override;
		virtual void OnEnable() override;
		virtual void OnDisable() override;

	private:
		HDEngine::ISkinnedMesh* _skinnedMesh;
	};
}

