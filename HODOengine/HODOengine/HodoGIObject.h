#pragma once
#include "Singleton.h"
#include "IHodoGIRenderer.h"
#include "IHodoGIFactory.h"
#include <memory>
#include <string>

namespace hodoEngine
{
	class HodoGIObject : public Singleton<HodoGIObject>
	{
		friend Singleton;

	public:
		hodoGI::I3DRenderer* renderer;
		hodoGI::IFactory* factory;
		// resourceManager, eventHandler, ...
		void LoadDLL(const std::wstring& dllFileName);
	};
}

