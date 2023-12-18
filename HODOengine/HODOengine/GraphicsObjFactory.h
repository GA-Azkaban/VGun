#pragma once
#include <windows.h>
#include <vector>
#include <memory>
#include <unordered_map>

#include "..\\HODO3DGraphicsInterface\\IFactory.h"
#include "Singleton.h"

namespace HDEngine
{
	class GraphicsObjFactory : public Singleton<GraphicsObjFactory>
	{
		friend Singleton;
	private:
		GraphicsObjFactory();

	public:
		void Initialize(HMODULE hModule);
		void Finalize();

		IFactory* GetFactory() const;

	private:
		HMODULE _dllHandle;
		std::unique_ptr<HDEngine::IFactory> _factory;
	};
}
