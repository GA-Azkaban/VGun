#pragma once
#include "../HODO3DGraphicsInterface/IResourceManager.h"
#include "../HODO3DGraphicsInterface/Node.h"
#include "Singleton.h"

namespace HDEngine
{
	class ResourceManager : public Singleton<ResourceManager>
	{
	public:
		friend Singleton;

		~ResourceManager();
		Node* GetNode(const std::string& fileName);

	private:
		ResourceManager();

	private:
		IResourceManager* _resourceManager;
	};
}
