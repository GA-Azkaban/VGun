#pragma once
#include "Singleton.h"
#include "IHodoGIFactory.h"
#include <memory>
#include <string>

/// <summary>
/// GIObject는 DLL Load를 통해 dll이 load 될 때
/// renderer, factory, resourceManager 등의 객체들을 생성하여 들고 있는다.
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

