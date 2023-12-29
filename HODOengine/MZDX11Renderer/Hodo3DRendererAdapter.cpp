#include "Hodo3DRendererAdapter.h"

namespace HDEngine
{
	I3DRenderer* CreateGraphicsInstance()
	{
		return new hodoGIAdapter::RendererAdapter();
	}

	void ReleaseGraphicsInstance(I3DRenderer* instance)
	{
		delete instance;
	}
}