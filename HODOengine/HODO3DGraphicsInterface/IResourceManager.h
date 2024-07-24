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
		virtual Node* GetNode(const std::string& fileName) = 0;
	};
}
