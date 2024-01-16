#include <cassert>
#include <algorithm>

#include "RenderSystem.h"
#include "..\\HODO3DGraphicsInterface\\IRenderable.h"
#include "SceneSystem.h"
#include "Scene.h"
#include "GameObject.h"
#include "Transform.h"
#include "Camera.h"
#include "TimeSystem.h"
#include "RendererBase.h"
#include "UIBase.h"
#include "GraphicsObjFactory.h"

#ifdef _DEBUG
#pragma comment(lib,"..\\x64\\Debug\\HODOmath.lib")
#else
#pragma comment(lib,"..\\x64\\Release\\HODOmath.lib")
#endif // _DEBUG

using GRAPHICS_CREATE_SIGNATURE = HDEngine::I3DRenderer* (*)(void);
constexpr const char* GRAPHICS_CREATE_NAME = "CreateGraphicsInstance";

#define GRAPHICS_RELEASE_SIGNATURE void(*)(HDEngine::I3DRenderer*)
#define GRAPHICS_RELEASE_NAME "ReleaseGraphicsInstance"

namespace HDEngine
{
	RenderSystem::RenderSystem()
		:_hWnd(), _screenWidth(), _screenHeight(),
		_dx11Renderer()
	{
	}

	void RenderSystem::Initialize(HWND hWnd, HMODULE hModule, int screenWidth, int screenHeight)
	{
		_dllHandle = hModule;
		_dx11Renderer.reset((reinterpret_cast<GRAPHICS_CREATE_SIGNATURE>(GetProcAddress(_dllHandle, GRAPHICS_CREATE_NAME)))());

		_hWnd = hWnd;
		_screenWidth = screenWidth;
		_screenHeight = screenHeight;

		_dx11Renderer->Initialize(static_cast<void*>(hWnd), screenWidth, screenHeight);
		_lineRenderer = GraphicsObjFactory::Instance().GetFactory()->CreateLineRenderer();
	}

	void RenderSystem::Finalize()
	{
		reinterpret_cast<GRAPHICS_RELEASE_SIGNATURE>(GetProcAddress(_dllHandle, GRAPHICS_RELEASE_NAME))(_dx11Renderer.release());
	}

	void RenderSystem::Update(float deltaTime)
	{
		HDData::Scene* currentScene = SceneSystem::Instance().GetCurrentScene();
		HDData::Camera* mainCam = currentScene->GetMainCamera();
		mainCam->UpdateRenderData();
		_dx11Renderer->Update(deltaTime);
	}

	void RenderSystem::DrawProcess()
	{
		//UpdateRenderData(); //SetRenderData + 전체 렌더 시작,
		_dx11Renderer->Render();
	}

	void RenderSystem::UpdateRenderData()
	{
		HDData::Scene* currentScene = SceneSystem::Instance().GetCurrentScene();
		HDData::Camera* mainCam = currentScene->GetMainCamera();
		mainCam->UpdateRenderData();

		for (auto rendererBase : _rendererList)
		{
			rendererBase->UpdateRenderData();
		}

		for (auto uiBase : _uiList)
		{
			uiBase->UpdateRenderData();
		}
	}

	int RenderSystem::GetScreenWidth() const
	{
		return _screenWidth;
	}

	int RenderSystem::GetScreenHeight() const
	{
		return _screenHeight;
	}

	void RenderSystem::PushRenderComponent(HDData::RendererBase* comp)
	{
		_rendererList.emplace_back(comp);
	}

	void RenderSystem::PushSketchComponent(HDData::UIBase* comp)
	{
		_uiList.emplace_back(comp);
	}

	void RenderSystem::DrawLine(HDMath::HDFLOAT3 start, HDMath::HDFLOAT3 end, HDMath::HDFLOAT4 color)
	{
		_lineRenderer->DrawLine(start, end, color);
	}

	void RenderSystem::DrawLine(HDMath::HDFLOAT3 start, HDMath::HDFLOAT3 direction, float length, HDMath::HDFLOAT4 color)
	{
		_lineRenderer->DrawLine(start, direction, length, color);
	}

}
