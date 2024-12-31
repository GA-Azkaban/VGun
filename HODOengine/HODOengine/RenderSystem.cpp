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
#include "Collider.h"
#include "../HODO3DGraphicsInterface/ICubeMap.h"

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
		UpdateRenderData();
		_dx11Renderer->Update(deltaTime);
	}

	void RenderSystem::DrawProcess()
	{
		_dx11Renderer->Render();
	}

	void RenderSystem::UpdateRenderData()
	{
		HDData::Scene* currentScene = SceneSystem::Instance().GetCurrentScene();
		HDData::Camera* mainCam = currentScene->GetMainCamera();
		mainCam->UpdateRenderData();
	}

	int RenderSystem::GetScreenWidth() const
	{
		return _screenWidth;
	}

	int RenderSystem::GetScreenHeight() const
	{
		return _screenHeight;
	}

	void RenderSystem::DrawLine(Vector3 start, Vector3 end, Vector4 color)
	{
		_lineRenderer->DrawLine(start, end, color);
	}

	void RenderSystem::DrawLine(Vector3 start, Vector3 direction, float length, Vector4 color)
	{
		_lineRenderer->DrawLine(start, direction, length, color);
	}

	void RenderSystem::OnResize(int screenWidth, int screenHeight)
	{
		_screenWidth = screenWidth;
		_screenHeight = screenHeight;
		_dx11Renderer->OnResize(screenWidth, screenHeight);
	}

	HDEngine::ICubeMap* RenderSystem::GetCubeMap()
	{
		return HDEngine::GraphicsObjFactory::Instance().GetFactory()->GetCubeMap();
	}

}
