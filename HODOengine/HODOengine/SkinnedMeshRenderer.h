﻿#pragma once
#include "RendererBase.h"
#include "dllExporter.h"
#include "../HODO3DGraphicsInterface/ISkinnedMesh.h"
#include "../HODO3DGraphicsInterface/Node.h"
#include "../HODO3DGraphicsInterface/IMaterial.h"

namespace HDData
{
	class Material;

	class HODO_API SkinnedMeshRenderer : public RendererBase
	{
	public:
		SkinnedMeshRenderer();
		~SkinnedMeshRenderer();

		void LoadMesh(const std::string& fileName);
		void LoadNode(const std::string& fileName);
		void LoadAnimation(const std::string& animListName);
		void LoadMaterial(HDData::Material* material, unsigned int element = 0);
		void LoadAlbedoMap(const std::string& fileName, unsigned int element = 0);
		void LoadNormalMap(const std::string& fileName, unsigned int element = 0);
		void LoadARMMap(const std::string& fileName, unsigned int element = 0);
		void LoadRoughnessMap(const std::string& fileName, unsigned int element = 0);
		void LoadMetallicMap(const std::string& fileName, unsigned int element = 0);
		void SetRoughnessValue(float value, unsigned int element = 0);
		void SetMetallicValue(float value, unsigned int element = 0);
		void SetAlbedoColor(UINT r, UINT g, UINT b, UINT a, unsigned int element = 0);
		void PlayAnimation(const std::string& animName, bool isLoop = true, float blendDuration = 0.1f, bool hasExitTime = true, float exitTime = 0.0f);
		void PlayAnimationAtOnce(const std::string& animName, bool isLoop = true, float blendDuration = 0.1f, bool hasExitTime = true, float exitTime = 0.0f);
		bool IsAnimationExitTimeElapsed();
		bool IsAnimationEnd();
		void SetOutlineActive(bool isActive);
		void SetFillModeWireFrame(bool setWireFrame);
		void SetMeshActive(bool isActive, unsigned int index);
		void SetShadowActive(bool isActive);
		Node* GetNode();
		int GetMeshCount();
		std::string GetCurrentAnimName();

	protected:
		virtual void Update() override;
		virtual void OnEnable() override;
		virtual void OnDisable() override;

	private:
		std::string _currentAnimName;
		HDEngine::ISkinnedMesh* _skinnedMesh;
	};
}

