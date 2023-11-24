#pragma once
#include "IHodoGIRenderer.h"
#include "IHodoGIFactory.h"
#include <functional>

namespace hodoGI
{
	namespace Global
	{
		std::function<hodoGI::I3DRenderer* ()> CreateRenderer;
		std::function<hodoGI::IFactory* ()> CreateFactory;
	}
}