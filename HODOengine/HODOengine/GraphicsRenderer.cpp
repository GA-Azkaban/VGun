#include "GraphicsRenderer.h"
#include "HodoGIObject.h"

std::unique_ptr<hodoEngine::GraphicsRenderer> hodoEngine::GraphicsRenderer::instance = nullptr;

hodoEngine::GraphicsRenderer& hodoEngine::GraphicsRenderer::Instance()
{
	if (!instance.get())
	{
		instance = std::make_unique<GraphicsRenderer>();
	}
	return *instance;
}

void hodoEngine::GraphicsRenderer::LoadGraphicsDll(const std::wstring& dllFileName)
{
	hodoEngine::HodoGIObject::Instance().LoadDLL(dllFileName);
}

void hodoEngine::GraphicsRenderer::SetOutputWindow(const HWND& hWnd)
{
	auto renderer = hodoEngine::HodoGIObject::Instance().renderer;
	renderer->SetOutputWindow(reinterpret_cast<UINT64>(hWnd));
}

void hodoEngine::GraphicsRenderer::Update(float deltaTime)
{
	auto renderer = hodoEngine::HodoGIObject::Instance().renderer;
	renderer->Update(deltaTime);
}

void hodoEngine::GraphicsRenderer::Render()
{
	auto renderer = hodoEngine::HodoGIObject::Instance().renderer;
	renderer->Render();
}

