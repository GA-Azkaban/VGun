#pragma once
#include "IHodoGIFactory.h"
#include "Hodo3DCameraAdapter.h"

namespace hodoGIAdapter
{
	class FactoryAdapter : public hodoGI::IFactory
	{
	public:
		virtual hodoGI::ICamera* CreateCamera() {
			return new hodoGIAdapter::CameraAdapter();
		}
	};
}