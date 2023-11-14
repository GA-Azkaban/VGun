#pragma once
#include "IHodoGIFactory.h"
#include <functional>

/// <summary>
/// �����Լ�, ���������� Global�̶�� namespace�� ���� �з��ߴ�.
/// </summary>

namespace hodoGI
{
	namespace Global
	{
		std::function<hodoGI::IFactory* ()> CreateFactory;
	}
}