#include "Hodo3DFactoryAdapter.h"

namespace HDEngine
{
	IFactory* CreateGraphicsObjectFactory()
	{
		return new hodoGIAdapter::FactoryAdapter();
	}

	void ReleaseFactory(IFactory* instance)
	{
		delete instance;
	}
}