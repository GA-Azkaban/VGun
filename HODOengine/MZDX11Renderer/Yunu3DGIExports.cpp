#include "YunuGraphicsInterface.h"
#include "Yunu3DRendererAdapter.h"
#include "Yunu3DFactoryAdapter.h"
#include "YunuGIResourceManagerAdapter.h"
#include "YunuGIEventHandlerAdapter.h"
#include "YunuGIGlobal.h"

//#include "YunuGIRendererAdapter.h"

extern "C"
{
    __declspec(dllexport) yunuGI::I3DRenderer* CreateRenderer()
    {
        return new yunuGIAdapter::RendererAdapter();
    }
    __declspec(dllexport) yunuGI::IFactory* CreateFactory()
    {
        return new yunuGIAdapter::FactoryAdapter();
    }
    __declspec(dllexport) yunuGI::IEventHandler* CreateEventHandler()
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
    }
}