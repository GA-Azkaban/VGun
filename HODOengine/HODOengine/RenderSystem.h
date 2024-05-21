#pragma once
#include <windows.h>
#include <vector>
#include <memory>
#include <unordered_map>

#include "..\\HODO3DGraphicsInterface\\I3DRenderer.h"
#include "Singleton.h"
#include "MathHeader.h"
#include "../HODO3DGraphicsInterface/ILineRenderer.h"

namespace HDData
{
	class RendererBase;
	class UIBase;
	class Collider;
}

namespace HDEngine
{
	class IRenderable;
}

namespace HDEngine
{
	class RenderSystem : public Singleton<RenderSystem>
	{
		friend Singleton;
	private:
		RenderSystem();		// 싱글턴이기 때문에 외부에서 생성할 수 없도록.

		/// 시스템 초기화 관련
	public:
		void Initialize(HWND hWnd, HMODULE hModule, int screenWidth, int screenHeight);
		void Finalize();

		/// 렌더링 관련
	public:
		void Update(float deltaTime);
		void DrawProcess();
		void DrawLine(Vector3 start, Vector3 end, Vector4 color);
		void DrawLine(Vector3 start, Vector3 direction, float length, Vector4 color);

	private:
		void UpdateRenderData();

	public:
		int GetScreenWidth() const;
		int GetScreenHeight() const;

		/// 기본 정보(윈도우 핸들, 윈도우 사이즈 등)
	private:
		HWND _hWnd;
		int _screenWidth;
		int _screenHeight;

		/// DLL 관련
	private:
		HMODULE _dllHandle;
		std::unique_ptr<I3DRenderer> _dx11Renderer;

	public:
		//void PushRenderComponent(HDData::RendererBase* comp);
		//void PushSketchComponent(HDData::UIBase* comp);
		void PushCollider(HDData::Collider* col);

	private:
		//std::vector<HDData::RendererBase*> _rendererList;
		//std::vector<HDData::UIBase*> _uiList;
		std::vector<HDData::Collider*> _colliderList;
		ILineRenderer* _lineRenderer;		
	};
};
