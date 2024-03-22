﻿#pragma once
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
		void LoadARMMap(const std::string& fileName);
		void LoadRoughnessMap(const std::string& fileName);
		void LoadMetallicMap(const std::string& fileName);
		void SetRoughnessValue(float value);
		void SetMetallicValue(float value);
		void PlayAnimation(const std::string& fileName, bool isLoop = true);
		bool IsAnimationEnd();
		void SetOutlineActive(bool isActive);
		const Matrix& GetBoneTransformByNodeName(std::string nodeName);

	protected:
		virtual void Update() override;
		virtual void OnEnable() override;
		virtual void OnDisable() override;

	private:
		HDEngine::ISkinnedMesh* _skinnedMesh;
	};
}

