#include "IHodoGIRenderer.h"
#include "Hodo3DRendererAdapter.h"
#include "IHodoGIFactory.h"
#include "Hodo3DFactoryAdapter.h"

extern "C"
{
	__declspec(dllexport) hodoGI::I3DRenderer* CreateRenderer()
	{
		return new hodoGIAdapter::RendererAdapter();
	}
	__declspec(dllexport) hodoGI::IFactory* CreateFactory()
	{
		return new hodoGIAdapter::FactoryAdapter();
	}
}