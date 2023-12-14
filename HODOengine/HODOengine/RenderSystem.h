#pragma once
#include <windows.h>
#include <vector>
#include <memory>
#include <unordered_map>

#include "..\\HODO3DGraphicsInterface\\I3DRenderer.h"
#include "Singleton.h"

#ifdef _DEBUG
#define GRAPHICSDLL_PATH (L"..\\x64\\Debug\\RocketDX11.dll") // (".\\my\\Path\\"#filename) ".\\my\\Path\\filename"
#else
#define GRAPHICSDLL_PATH ("Graphics\\RocketDX11.dll"#filename)
#endif // _DEBUG

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
		void Initialize(HWND hWnd, int screenWidth, int screenHeight, bool isEditor = false);
		void Finalize();

	private:
		bool _isEditor = false;

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
		HMODULE hGraphicsModule;
		std::unique_ptr<HDEngine::I3DRenderer> _dx11Renderer;
	};
};
