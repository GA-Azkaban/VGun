#include "HodoGIObject.h"
#include "HodoGIGlobal.h"
#include <windows.h>
#include <libloaderapi.h>
#include <assert.h>

typedef hodoGI::I3DRenderer* (*CreateRendererFunc)();
typedef hodoGI::IFactory* (*CreateFactoryFunc)();

void hodoEngine::HodoGIObject::LoadDLL(const std::wstring& dllFileName)
{
	HINSTANCE hInstDLL{ LoadLibraryW(dllFileName.c_str()) };
	assert(hInstDLL != nullptr, "Graphics Engine DLL is not found!");

	auto createRendererFunc = reinterpret_cast<CreateRendererFunc>(GetProcAddress(hInstDLL, "CreateRenderer"));
	hodoGI::Global::CreateRenderer = createRendererFunc;

	auto createFactoryFunc = reinterpret_cast<CreateFactoryFunc>(GetProcAddress(hInstDLL, "CreateFactory"));
	hodoGI::Global::CreateFactory = createFactoryFunc;

	renderer = hodoGI::Global::CreateRenderer();
	factory = hodoGI::Global::CreateFactory();
}
