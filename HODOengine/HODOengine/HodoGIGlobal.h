#pragma once
#include "IHodoGIFactory.h"
#include <functional>

/// <summary>
/// 전역함수, 전역변수를 Global이라는 namespace를 통해 분류했다.
/// </summary>

namespace hodoGI
{
	namespace Global
	{
		std::function<hodoGI::IFactory* ()> CreateFactory;
	}
}