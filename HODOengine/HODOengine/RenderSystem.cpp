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
	}

	void RenderSystem::Finalize()
	{
		reinterpret_cast<GRAPHICS_RELEASE_SIGNATURE>(GetProcAddress(_dllHandle, GRAPHICS_RELEASE_NAME))(_dx11Renderer.release());
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
// 				// ������ ��ü�� ���ε��� �׸��� ���� �ƴ�
// 				// RenderSystem���� ������ ��ü�� ������ �������� Graphics���� �׸���� ��Ű�� ��
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
		UpdateRenderData(); //SetRenderData + ��ü ���� ����,
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
}
