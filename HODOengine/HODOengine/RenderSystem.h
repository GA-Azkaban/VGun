#pragma once
#include <windows.h>
#include <vector>
#include <memory>
#include <unordered_map>

#include "..\\HODO3DGraphicsInterface\\I3DRenderer.h"
#include "Singleton.h"

namespace HDData
{
	class Renderer;
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
		RenderSystem();		// �̱����̱� ������ �ܺο��� ������ �� ������.

		/// �ý��� �ʱ�ȭ ����
	public:
		void Initialize(HWND hWnd, HMODULE hModule, int screenWidth, int screenHeight);
		void Finalize();

	public:
		void MakeAndLinkRenderable();

	private:
		std::unordered_map<HDData::Renderer*, IRenderable*> _renderMap;

		/// ������ ����
	public:
		void DrawProcess();

	private:
		void UpdateRenderData();

	public:
		int GetScreenWidth() const;
		int GetScreenHeight() const;

		/// �⺻ ����(������ �ڵ�, ������ ������ ��)
	private:
		HWND _hWnd;
		int _screenWidth;
		int _screenHeight;

		/// DLL ����
	private:
		HMODULE _dllHandle;
		std::unique_ptr<HDEngine::I3DRenderer> _dx11Renderer;
	};
};
