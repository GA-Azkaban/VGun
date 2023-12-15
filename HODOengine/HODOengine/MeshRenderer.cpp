#include "MeshRenderer.h"
#include "Transform.h"

namespace HDData
{
	MeshRenderer::MeshRenderer()
		:_staticMesh(nullptr //RenderSystem.CreateStaticMesh()
		)
	{

	}

	void MeshRenderer::Update()
	{
		_staticMesh->SetWorldTM(GetTransform()->GetWorldTM());
	}

	void MeshRenderer::SetActive(bool active)
	{
		_staticMesh->SetActive(active);
	}

	void MeshRenderer::LoadMesh(const char* fileName)
	{
		_staticMesh->LoadMesh(fileName);
	}

	void MeshRenderer::LoadDiffuseMap(const char* fileName)
	{
		_staticMesh->LoadDiffuseMap(fileName);
	}

	void MeshRenderer::LoadNormalMap(const char* fileName)
	{
		_staticMesh->LoadNormalMap(fileName);
	}
}