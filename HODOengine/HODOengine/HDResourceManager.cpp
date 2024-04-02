#include "HDResourceManager.h"
#include "GraphicsObjFactory.h"
#include <string>

namespace HDEngine
{

	ResourceManager::ResourceManager()
		: _resourceManager(GraphicsObjFactory::Instance().GetFactory()->CreateResourceManager())
	{

	}

	ResourceManager::~ResourceManager()
	{
		_resourceManager = nullptr;
	}

	Node* ResourceManager::GetNode(const std::string& fileName)
	{
		return _resourceManager->GetNode(fileName);
	}

}
