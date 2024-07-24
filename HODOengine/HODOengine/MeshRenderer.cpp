#include "MeshRenderer.h"
#include "Transform.h"
#include "GraphicsObjFactory.h"
#include "RenderSystem.h"
#include "HDMaterial.h"

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

	void MeshRenderer::LoadMaterial(HDData::Material* material, unsigned int element /*= 0*/)
	{
		_staticMesh->LoadMaterial(material->Get(), element);
	}

	void MeshRenderer::LoadNormalMap(const std::string& fileName, unsigned int element /* = 0 */)
	{
		_staticMesh->LoadNormalMap(fileName, element);
	}

	void MeshRenderer::LoadAlbedoMap(const std::string& fileName, unsigned int element /* = 0 */)
	{
		_staticMesh->LoadAlbedoMap(fileName, element);
	}

	void MeshRenderer::LoadARMMap(const std::string& fileName, unsigned int element /* = 0 */)
	{
        _staticMesh->LoadARMMap(fileName, element);
    }

	void MeshRenderer::LoadRoughnessMap(const std::string& fileName, unsigned int element /* = 0 */)
	{
		_staticMesh->LoadRoughnessMap(fileName, element);
	}

	void MeshRenderer::LoadMetallicMap(const std::string& fileName, unsigned int element /* = 0 */)
	{
		_staticMesh->LoadMetallicMap(fileName, element);
	}

	void MeshRenderer::SetRoughnessValue(float value, unsigned int element /* = 0 */)
	{
		_staticMesh->SetRoughnessValue(value, element);
	}

	void MeshRenderer::SetMetallicValue(float value, unsigned int element /* = 0 */)
	{
		_staticMesh->SetMetallicValue(value, element);
	}

	void MeshRenderer::SetAlbedoColor(UINT r, UINT g, UINT b, UINT a, unsigned int element /* = 0 */)
	{
		_staticMesh->SetAlbedoColor(r, g, b, a, element);
	}

	void MeshRenderer::SetUseLight(bool useLight)
	{
		_staticMesh->SetUseLight(useLight);
	}

	void MeshRenderer::SetMeshActive(bool isActive, unsigned int index)
	{
		_staticMesh->SetMeshActive(isActive, index);
	}

	void MeshRenderer::SetShadowActive(bool isActive)
	{
		_staticMesh->SetShadowActive(isActive);
	}

	int MeshRenderer::GetMeshCount()
	{
		return _staticMesh->GetMeshCount();
	}

	void MeshRenderer::SetCullMode(HDEngine::CullMode cullMode)
	{
		_staticMesh->SetCullMode(cullMode);
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
