#include "MeshRenderer.h"
#include "GraphicsObjFactory.h"
#include "Transform.h"

namespace HDData
{
	MeshRenderer::MeshRenderer()
		: _staticMesh(HDEngine::GraphicsObjFactory::Instance().GetFactory()->CreateStaticMeshObject())
	{
		
	}

	HDEngine::IStaticMesh& MeshRenderer::Get()
	{
		return *_staticMesh;
	}

	void MeshRenderer::OnEnable()
	{
		_staticMesh->SetActive(true);
	}

	void MeshRenderer::OnDisable()
	{
		_staticMesh->SetActive(false);
	}

	void MeshRenderer::Update()
	{
		_staticMesh->SetWorldTM(GetTransform()->GetWorldTM());
	}

}