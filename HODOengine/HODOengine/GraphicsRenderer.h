#pragma once
#include "Singleton.h"
#include <Windows.h>
#include <memory>
#include <string>

namespace hodoEngine
{
	class GraphicsRenderer
	{
	public:
		static GraphicsRenderer& Instance();

		void LoadGraphicsDll(const std::wstring& dllFileName);
		void SetOutputWindow(const HWND& hWnd);
		void Update(float deltaTime);
		void Render();

	private:
		static std::unique_ptr<GraphicsRenderer> instance;
	};
}
