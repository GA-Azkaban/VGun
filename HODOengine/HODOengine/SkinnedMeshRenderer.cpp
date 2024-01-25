#include "SkinnedMeshRenderer.h"
#include "GraphicsObjFactory.h"
#include "Transform.h"

namespace HDData
{
	SkinnedMeshRenderer::SkinnedMeshRenderer()
		: _skinnedMesh(HDEngine::GraphicsObjFactory::Instance().GetFactory()->CreateSkinnedMeshObject())
	{

	}

	HDEngine::ISkinnedMesh& SkinnedMeshRenderer::Get()
	{
		return *_skinnedMesh;
	}

	void SkinnedMeshRenderer::OnEnable()
	{
		_skinnedMesh->SetActive(true);
	}

	void SkinnedMeshRenderer::OnDisable()
	{
		_skinnedMesh->SetActive(false);
	}

	void SkinnedMeshRenderer::Update()
	{
		_skinnedMesh->SetWorldTM(GetTransform()->GetWorldTM());
	}

}