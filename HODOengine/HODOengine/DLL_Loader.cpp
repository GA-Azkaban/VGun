#include <cassert>
#include "DLL_Loader.h"

namespace HDEngine
{

	DLL_Loader::DLL_Loader()
	{

	}

	DLL_Loader::~DLL_Loader()
	{
		FreeLibrary(_dllHandle);
	}

	void DLL_Loader::LoadDLL(std::wstring dllPath)
	{
		_dllHandle = LoadLibrary(dllPath.c_str());
		DWORD error_code = GetLastError();
		assert(_dllHandle);
	}

	HMODULE DLL_Loader::GetDLLHandle()
	{
		return _dllHandle;
	}

}
