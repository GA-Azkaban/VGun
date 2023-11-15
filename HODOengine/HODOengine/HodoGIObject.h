#pragma once
#include "Singleton.h"
#include "IHodoGIFactory.h"
#include <memory>
#include <string>

namespace hodoEngine
{
	class HodoGIObject : public Singleton<HodoGIObject>
	{
	public:
		hodoGI::IFactory* factory;
		// renderer, resourceManager, eventHandler, ...
		void LoadDLL(const std::wstring& dllFileName);
	};
}

