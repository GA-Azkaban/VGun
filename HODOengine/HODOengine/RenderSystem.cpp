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
#include "Renderer.h"
#include "CubeRenderer.h"

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
		hGraphicsModule(LoadLibrary(GRAPHICSDLL_PATH)),
		_dx11Renderer()
	{
		DWORD error_code = GetLastError();
		assert(hGraphicsModule);

		auto a = GetProcAddress(hGraphicsModule, GRAPHICS_CREATE_NAME);
		error_code = GetLastError();

		_dx11Renderer.reset((reinterpret_cast<GRAPHICS_CREATE_SIGNATURE>(GetProcAddress(hGraphicsModule, GRAPHICS_CREATE_NAME)))());

		//extern "C" __declspec(dllexport) IRocketGraphics * CreateGraphicsInstance();
		//extern "C" __declspec(dllexport) void ReleaseGraphicsInstance(IRocketGraphics * instance);
	}

	void RenderSystem::Initialize(HWND hWnd, int screenWidth, int screenHeight)
	{
		_hWnd = hWnd;
		_screenWidth = screenWidth;
		_screenHeight = screenHeight;

		_dx11Renderer->Initialize(static_cast<void*>(hWnd), screenWidth, screenHeight);
	}

	void RenderSystem::Finalize()
	{
		reinterpret_cast<GRAPHICS_RELEASE_SIGNATURE>(GetProcAddress(hGraphicsModule, GRAPHICS_RELEASE_NAME))(_dx11Renderer.release());
		FreeLibrary(hGraphicsModule);
	}

	void RenderSystem::MakeAndLinkRenderable()
	{
// 		for (auto& sceneIter : SceneSystem::Instance().GetAllScenes())
// 		{
// 			if (sceneIter.second == nullptr)
// 			{
// 				return;
// 			}
// 
// 			for (auto& object : sceneIter.second->GetGameObjectList())
// 			{
// 				// 각각의 객체가 본인들을 그리는 것이 아닌
// 				// RenderSystem에서 각각의 객체의 정보를 바탕으로 Graphics에게 그리라고 시키는 것
// 				// object->GetRenderData();
// 
// 				HDData::Renderer* renderer = object->GetComponent<HDData::BoxRenderer>();
// 
// 				if (!renderer)
// 				{
// 					continue;
// 				}
// 
// 				if (_renderMap.find(renderer) == _renderMap.end())
// 				{
// 					IRenderable* renderable = _dx11Renderer->CreateRenderable();
// 					_renderMap.insert({ renderer, renderable });
// 				}
// 			}
// 		}
	}

	void RenderSystem::DrawProcess()
	{
		UpdateRenderData(); //SetRenderData + 전체 렌더 시작,
		_dx11Renderer->Render();
	}

	void RenderSystem::UpdateRenderData()
	{
		HDData::Scene* currentScene = SceneSystem::Instance().GetCurrentScene();
		HDData::Camera* mainCam = currentScene->GetMainCamera();
		_dx11Renderer->UpdateCamera(mainCam->GetCameraData());
	}

	int RenderSystem::GetScreenWidth() const
	{
		return _screenWidth;
	}

	int RenderSystem::GetScreenHeight() const
	{
		return _screenHeight;
	}
}
