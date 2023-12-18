#pragma once
#include "..\\HODO3DGraphicsInterface\\IFactory.h"

namespace RocketCore::Graphics
{
	class ObjectFactory : public HDEngine::IFactory
	{
	public:
		virtual HDEngine::ICamera* CreateCamera() override;
	};
}

