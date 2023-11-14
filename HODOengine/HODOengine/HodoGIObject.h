#pragma once
#include "Singleton.h"
#include "IHodoGIFactory.h"
#include <memory>
#include <string>

/// <summary>
/// GIObject�� DLL Load�� ���� dll�� load �� ��
/// renderer, factory, resourceManager ���� ��ü���� �����Ͽ� ��� �ִ´�.
/// </summary>

namespace hodoEngine
{
	class HodoGIObject : public Singleton<HodoGIObject>
	{
	public:
		hodoGI::IFactory* _factory;
		// renderer, resourceManager, eventHandler, ...
		void LoadDLL(const std::wstring& dllFileName);
	};
}

