#pragma once
#include "IHodoGIFactory.h"
#include <functional>

namespace hodoGI
{
	namespace Global
	{
		std::function<hodoGI::IFactory* ()> CreateFactory;
	}
}