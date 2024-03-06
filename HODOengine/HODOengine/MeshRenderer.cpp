#include "MeshRenderer.h"
#include "Transform.h"
#include "GraphicsObjFactory.h"
#include "RenderSystem.h"

namespace HDData
{
	MeshRenderer::MeshRenderer()
		: _staticMesh(HDEngine::GraphicsObjFactory::Instance().GetFactory()->CreateStaticMeshObject())
	{
		HDEngine::RenderSystem::Instance().PushRenderComponent(this);
		LoadDiffuseMap("Default.png");
	}

	void MeshRenderer::LoadMesh(const std::string& fileName)
	{
		_staticMesh->LoadMesh(fileName);
	}

	void MeshRenderer::LoadNormalMap(const std::string& fileName)
	{
		_staticMesh->LoadNormalMap(fileName);
	}

	void MeshRenderer::LoadDiffuseMap(const std::string& fileName)
	{
		_staticMesh->LoadDiffuseMap(fileName);
	}

	void MeshRenderer::UpdateRenderData()
	{
		_staticMesh->SetWorldTM(GetTransform()->GetWorldTM());
	}

	void MeshRenderer::OnEnable()
	{
		_staticMesh->SetActive(true);
	}

	void MeshRenderer::OnDisable()
	{
		_staticMesh->SetActive(false);
	}

}
