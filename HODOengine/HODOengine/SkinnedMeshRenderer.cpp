#include "SkinnedMeshRenderer.h"
#include "RenderSystem.h"
#include "GraphicsObjFactory.h"
#include "Transform.h"

namespace HDData
{
	SkinnedMeshRenderer::SkinnedMeshRenderer()
		: _skinnedMesh(HDEngine::GraphicsObjFactory::Instance().GetFactory()->CreateSkinnedMeshObject())
	{
		HDEngine::RenderSystem::Instance().PushRenderComponent(this);
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

	void SkinnedMeshRenderer::PlayAnimation(const std::string& fileName, bool isLoop /*= true*/)
	{
		_skinnedMesh->PlayAnimation(fileName, isLoop);
	}

	bool SkinnedMeshRenderer::IsAnimationEnd()
	{
		return _skinnedMesh->IsAnimationEnd();
	}

	void SkinnedMeshRenderer::UpdateRenderData()
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
