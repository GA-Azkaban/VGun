﻿#include "SkinnedMeshRenderer.h"
#include "RenderSystem.h"
#include "GraphicsObjFactory.h"
#include "Transform.h"
#include "GameObject.h"
#include "HDMaterial.h"

namespace HDData
{
	SkinnedMeshRenderer::SkinnedMeshRenderer()
		: _skinnedMesh(HDEngine::GraphicsObjFactory::Instance().GetFactory()->CreateSkinnedMeshObject())
	{
		
	}

	SkinnedMeshRenderer::~SkinnedMeshRenderer()
	{
		_skinnedMesh = nullptr;
	}

	void SkinnedMeshRenderer::LoadMesh(const std::string& fileName)
	{
		_skinnedMesh->LoadMesh(fileName);
	}

	void SkinnedMeshRenderer::LoadNode(const std::string& fileName)
	{
		_skinnedMesh->LoadNode(fileName);
	}
	
	void SkinnedMeshRenderer::LoadAnimation(const std::string& animListName)
	{
		_skinnedMesh->LoadAnimation(animListName);
	}

	void SkinnedMeshRenderer::LoadMaterial(HDData::Material* material, unsigned int element /*= 0*/)
	{
		_skinnedMesh->LoadMaterial(material->Get(), element);
	}

	void SkinnedMeshRenderer::LoadNormalMap(const std::string& fileName, unsigned int element /* = 0 */)
	{
		_skinnedMesh->LoadNormalMap(fileName, element);
	}

	void SkinnedMeshRenderer::LoadAlbedoMap(const std::string& fileName, unsigned int element /* = 0 */)
	{
		_skinnedMesh->LoadAlbedoMap(fileName, element);
	}

	void SkinnedMeshRenderer::LoadARMMap(const std::string& fileName, unsigned int element /* = 0 */)
	{
        _skinnedMesh->LoadARMMap(fileName, element);
    }

	void SkinnedMeshRenderer::LoadRoughnessMap(const std::string& fileName, unsigned int element /* = 0 */)
	{
		_skinnedMesh->LoadRoughnessMap(fileName, element);
	}

	void SkinnedMeshRenderer::LoadMetallicMap(const std::string& fileName, unsigned int element /* = 0 */)
	{
		_skinnedMesh->LoadMetallicMap(fileName, element);
	}

	void SkinnedMeshRenderer::SetRoughnessValue(float value, unsigned int element /* = 0 */)
	{
		_skinnedMesh->SetRoughnessValue(value, element);
	}

	void SkinnedMeshRenderer::SetMetallicValue(float value, unsigned int element /* = 0 */)
	{
		_skinnedMesh->SetMetallicValue(value, element);
	}

	void SkinnedMeshRenderer::SetAlbedoColor(UINT r, UINT g, UINT b, UINT a, unsigned int element /* = 0 */)
	{
		_skinnedMesh->SetAlbedoColor(r, g, b, a, element);
	}

	void SkinnedMeshRenderer::PlayAnimation(const std::string& animName, bool isLoop /*= true*/, float blendDuration /*= 0.1f*/, bool hasExitTime /*= true*/, float exitTime /*= 0.0f*/)
	{
		_currentAnimName = animName;
		_skinnedMesh->PlayAnimation(animName, isLoop, blendDuration, hasExitTime, exitTime);
	}

	void SkinnedMeshRenderer::PlayAnimationAtOnce(const std::string& animName, bool isLoop, float blendDuration, bool hasExitTime, float exitTime)
	{
		_currentAnimName = animName;
		_skinnedMesh->PlayAnimationAtOnce(animName, isLoop, blendDuration, hasExitTime, exitTime);
	}

	bool SkinnedMeshRenderer::IsAnimationExitTimeElapsed()
	{
		return _skinnedMesh->IsAnimationExitTimeElapsed();
	}

	bool SkinnedMeshRenderer::IsAnimationEnd()
	{
		return _skinnedMesh->IsAnimationEnd();
	}

	void SkinnedMeshRenderer::SetOutlineActive(bool isActive)
	{
		_skinnedMesh->SetOutlineActive(isActive);
	}

	void SkinnedMeshRenderer::SetFillModeWireFrame(bool setWireFrame)
	{
		_skinnedMesh->SetFillModeWireFrame(setWireFrame);
	}

	void SkinnedMeshRenderer::SetMeshActive(bool isActive, unsigned int index)
	{
		_skinnedMesh->SetMeshActive(isActive, index);
	}

	void SkinnedMeshRenderer::SetShadowActive(bool isActive)
	{
		_skinnedMesh->SetShadowActive(isActive);
	}

	Node* SkinnedMeshRenderer::GetNode()
	{
		return _skinnedMesh->GetNode();
	}

	int SkinnedMeshRenderer::GetMeshCount()
	{
		return _skinnedMesh->GetMeshCount();
	}

	std::string SkinnedMeshRenderer::GetCurrentAnimName()
	{
		return _currentAnimName;
	}

	void SkinnedMeshRenderer::Update()
	{
		_skinnedMesh->SetWorldTM(GetTransform()->GetWorldTM());
	}

	void SkinnedMeshRenderer::OnEnable()
	{
		_skinnedMesh->SetActive(true);
	}

	void SkinnedMeshRenderer::OnDisable()
	{
		_skinnedMesh->SetActive(false);
	}

}
