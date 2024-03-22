#include "MeshRenderer.h"
#include "Transform.h"
#include "GraphicsObjFactory.h"
#include "RenderSystem.h"

namespace HDData
{
	MeshRenderer::MeshRenderer()
		: _staticMesh(HDEngine::GraphicsObjFactory::Instance().GetFactory()->CreateStaticMeshObject())
	{
		
	}

	MeshRenderer::~MeshRenderer()
	{
		_staticMesh = nullptr;
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

	void MeshRenderer::LoadARMMap(const std::string& fileName)
	{
        _staticMesh->LoadARMMap(fileName);
    }

	void MeshRenderer::LoadRoughnessMap(const std::string& fileName)
	{
		_staticMesh->LoadRoughnessMap(fileName);
	}

	void MeshRenderer::LoadMetallicMap(const std::string& fileName)
	{
		_staticMesh->LoadMetallicMap(fileName);
	}

	void MeshRenderer::SetRoughnessValue(float value)
	{
		_staticMesh->SetRoughnessValue(value);
	}

	void MeshRenderer::SetMetallicValue(float value)
	{
		_staticMesh->SetMetallicValue(value);
	}

	void MeshRenderer::Update()
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
