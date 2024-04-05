#pragma once
#include "Singleton.h"
#include "../HODO3DGraphicsInterface/IMaterial.h"
#include <unordered_map>
#include <string>

namespace HDData
{
	class Material;
}

namespace HDEngine
{
	class MaterialManager : public Singleton<MaterialManager>
	{
	public:
		friend Singleton;
		HDData::Material* CreateMaterial(const HDEngine::MaterialDesc& desc);
		HDData::Material* GetMaterial(const std::string& name);

	private:
		std::unordered_map<std::string, HDData::Material*> _materialList;
	};
}
