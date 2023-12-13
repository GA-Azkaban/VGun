#pragma once
#include <Rpc.h>
#include <string>
#pragma comment(lib, "Rpcrt4.lib")

#include "Singleton.h"

namespace HDEngine
{
	class IDSystem : public Singleton<IDSystem>
	{
		friend Singleton;

	private:
		IDSystem() = default;

	public:
		std::string CreateID();
	};
}

