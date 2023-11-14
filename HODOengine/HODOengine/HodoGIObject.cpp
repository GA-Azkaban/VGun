#include "HodoGIObject.h"
#include "HodoGIGlobal.h"
#include <windows.h>
#include <libloaderapi.h>
#include <assert.h>

typedef hodoGI::IFactory* (*CreateFactoryFunc)();

void hodoEngine::HodoGIObject::LoadDLL(const std::wstring& dllFileName)
{
	HINSTANCE hInstDLL{ LoadLibraryW(dllFileName.c_str()) };
	assert(hInstDLL != nullptr, "Graphics Engine DLL is not found!");

	auto createFactoryFunc = reinterpret_cast<CreateFactoryFunc>(GetProcAddress(hInstDLL, "CreateFactory"));
	hodoGI::Global::CreateFactory = createFactoryFunc;
	assert(hodoGI::Global::CreateFactory != nullptr, "CreateFactory Function is not found.");

	_factory = hodoGI::Global::CreateFactory();
}
