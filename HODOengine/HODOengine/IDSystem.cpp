#include "IDSystem.h"


namespace hodoEngine
{
	std::string IDSystem::CreateID()
	{
		UUID one;
		RPC_STATUS s;
		std::string newID;

		UuidCreate(&one);
		UuidToStringA(&one, (RPC_CSTR*)&newID);

		return newID;
	}

}
