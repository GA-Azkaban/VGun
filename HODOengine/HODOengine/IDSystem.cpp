#include "IDSystem.h"


namespace HDEngine
{
	std::string IDSystem::CreateID()
	{
		UUID one;
		RPC_STATUS s;
		char* newID;

		UuidCreate(&one);
		UuidToStringA(&one, (RPC_CSTR*)&newID);

		return newID;
	}

}
