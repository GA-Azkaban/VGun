#include "GraphicsObjFactory.h"

using FACTORY_CREATE_SIGNATURE = HDEngine::IFactory* (*)(void);
constexpr const char* FACTORY_CREATE_NAME = "CreateGraphicsObjectFactory";

#define FACTORY_RELEASE_SIGNATURE void(*)(HDEngine::IFactory*)
#define FACTORY_RELEASE_NAME "ReleaseFactory"

namespace HDEngine
{
	GraphicsObjFactory::GraphicsObjFactory()
	{

	}

	void GraphicsObjFactory::Initialize(HMODULE hModule)
	{
		_dllHandle = hModule;
		_factory.reset((reinterpret_cast<FACTORY_CREATE_SIGNATURE>(GetProcAddress(_dllHandle, FACTORY_CREATE_NAME)))());
	}

	void GraphicsObjFactory::Finalize()
	{
		reinterpret_cast<FACTORY_RELEASE_SIGNATURE>(GetProcAddress(_dllHandle, FACTORY_RELEASE_NAME))(_factory.release());
	}

	HDEngine::IFactory* GraphicsObjFactory::GetFactory() const
	{
		return _factory.get();
	}

}
