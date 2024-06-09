#include "DecalObject.h"
#include "ResourceManager.h"
#include "Mesh.h"
#include "Material.h"

namespace RocketCore::Graphics
{

	DecalObject::DecalObject()
	{
		_mesh = ResourceManager::Instance().GetMeshes(fileName)[0];
		_material = ResourceManager::Instance().GetMaterials()[0];
		_boundingBox = ResourceManager::Instance().GetBoundingBox(fileName);
		_rasterizerState = ResourceManager::Instance().GetRasterizerState(ResourceManager::eRasterizerState::SOLID);
		_vertexShader = ResourceManager::Instance().GetVertexShader("VertexShader.cso");
		_pixelShader = ResourceManager::Instance().GetPixelShader("PixelShader.cso");
	}

	DecalObject::~DecalObject()
	{

	}

	void DecalObject::SetWorldTM(const Matrix& worldTM)
	{
		_world = worldTM;
		_receiveTMInfoFlag = true;
	}

	void DecalObject::SetActive(bool isActive)
	{
		_isActive = isActive;
	}

	void DecalObject::LoadMaterial(HDEngine::IMaterial* material)
	{
		Material* newMat = dynamic_cast<Material*>(material);
		if (newMat != nullptr)
		{
			_material = newMat;
		}
	}

	void DecalObject::SetColor(UINT r, UINT g, UINT b, UINT a)
	{
		if (_material != nullptr)
		{
			_material->SetColor(r, g, b, a);
		}
	}

}
