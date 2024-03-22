#include "SkinnedMeshRenderer.h"
#include "RenderSystem.h"
#include "GraphicsObjFactory.h"
#include "Transform.h"

namespace HDData
{
	SkinnedMeshRenderer::SkinnedMeshRenderer()
		: _skinnedMesh(HDEngine::GraphicsObjFactory::Instance().GetFactory()->CreateSkinnedMeshObject())
	{
		//HDEngine::RenderSystem::Instance().PushRenderComponent(this);
	}

	SkinnedMeshRenderer::~SkinnedMeshRenderer()
	{
		_skinnedMesh = nullptr;
	}

	void SkinnedMeshRenderer::LoadMesh(const std::string& fileName)
	{
		_skinnedMesh->LoadMesh(fileName);
	}

	void SkinnedMeshRenderer::LoadNormalMap(const std::string& fileName)
	{
		_skinnedMesh->LoadNormalMap(fileName);
	}

	void SkinnedMeshRenderer::LoadDiffuseMap(const std::string& fileName)
	{
		_skinnedMesh->LoadDiffuseMap(fileName);
	}

	void SkinnedMeshRenderer::LoadARMMap(const std::string& fileName)
	{
        _skinnedMesh->LoadARMMap(fileName);
    }

	void SkinnedMeshRenderer::LoadRoughnessMap(const std::string& fileName)
	{
		_skinnedMesh->LoadRoughnessMap(fileName);
	}

	void SkinnedMeshRenderer::LoadMetallicMap(const std::string& fileName)
	{
		_skinnedMesh->LoadMetallicMap(fileName);
	}

	void SkinnedMeshRenderer::SetRoughnessValue(float value)
	{
		_skinnedMesh->SetRoughnessValue(value);
	}

	void SkinnedMeshRenderer::SetMetallicValue(float value)
	{
		_skinnedMesh->SetMetallicValue(value);
	}

	void SkinnedMeshRenderer::PlayAnimation(const std::string& fileName, bool isLoop /*= true*/)
	{
		_skinnedMesh->PlayAnimation(fileName, isLoop);
	}

	bool SkinnedMeshRenderer::IsAnimationEnd()
	{
		return _skinnedMesh->IsAnimationEnd();
	}

	void SkinnedMeshRenderer::SetOutlineActive(bool isActive)
	{
		_skinnedMesh->SetOutlineActive(isActive);
	}

	const Matrix& SkinnedMeshRenderer::GetBoneTransformByNodeName(std::string nodeName)
	{
		return _skinnedMesh->GetBoneTransformByNodeName(nodeName);
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
