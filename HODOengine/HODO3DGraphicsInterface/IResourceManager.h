#pragma once
#include "IMaterial.h"
#include <unordered_map>

class Node;

namespace HDEngine
{
	class IResourceManager
	{
	public:
		virtual ~IResourceManager() {};
		//virtual void LoadFBXFile(std::string fileName) const = 0;
		//virtual IMaterial* CreateMaterial(MaterialDesc& materialDesc) const = 0;
		virtual Node* GetNode(const std::string& fileName) = 0;
	};
}
