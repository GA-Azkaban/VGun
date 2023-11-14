#pragma once
#include "IHodoGIFactory.h"
#include "Hodo3DFactoryAdapter.h"

extern "C"
{
	/*__declspec(dllexport) yunuGI::I3DRenderer* CreateRenderer()
	{
		return new yunuGIAdapter::RendererAdapter();
	}*/
	__declspec(dllexport) hodoGI::IFactory* CreateFactory()
	{
		return new hodoGIAdapter::FactoryAdapter();
	}
	/*__declspec(dllexport) yunuGI::IEventHandler* CreateEventHandler()
	{
		return new yunuGIAdapter::EventHandlerAdapter();
	}
	__declspec(dllexport) yunuGI::IResourceManager* CreateResourceManager()
	{
		return new yunuGIAdapter::ResourceManagerAdapter();
	}
	__declspec(dllexport) void ReleaseYunuGI(yunuGI::IUnknown* target)
	{
		delete target;
	}*/
}