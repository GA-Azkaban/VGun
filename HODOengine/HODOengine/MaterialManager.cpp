#include "MaterialManager.h"
#include "HDMaterial.h"
#include "GraphicsObjFactory.h"

namespace HDEngine
{

	HDData::Material* MaterialManager::CreateMaterial(const HDEngine::MaterialDesc& desc)
	{
		HDData::Material* newMat = new HDData::Material(desc);
		_materialList.insert(std::make_pair(newMat->GetMaterialName(), newMat));
		return newMat;
	}

	HDData::Material* MaterialManager::GetMaterial(const std::string& name)
	{
		auto iter = _materialList.find(name);
		if (iter == _materialList.end())
		{
			auto material = GraphicsObjFactory::Instance().GetFactory()->GetMaterial(name);
			if (material != nullptr)
			{
				HDData::Material* newMat = new HDData::Material(material);
				_materialList.insert(std::make_pair(newMat->GetMaterialName(), newMat));
				return newMat;
			}
			else
			{
				return nullptr;
			}
		}
		return iter->second;
	}

}
