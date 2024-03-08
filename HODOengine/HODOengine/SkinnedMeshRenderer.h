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
		~SkinnedMeshRenderer();

		void LoadMesh(const std::string& fileName);
		void LoadNormalMap(const std::string& fileName);
		void LoadDiffuseMap(const std::string& fileName);
		void PlayAnimation(const std::string& fileName, bool isLoop = true);
		bool IsAnimationEnd();
		void SetOutlineActive(bool isActive);
		void SetOutlineData(const Vector4& color = Vector4{ 1.0f, 0.54f, 0.0f, 1.0f }, bool depthCheck = true);

	protected:
		virtual void Update() override;
		virtual void OnEnable() override;
		virtual void OnDisable() override;

	private:
		HDEngine::ISkinnedMesh* _skinnedMesh;
	};
}

