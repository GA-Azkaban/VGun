#pragma once
#include "../HODO3DGraphicsInterface/I3DRenderer.h"
#include "MZDX11Renderer.h"

namespace hodoGIAdapter
{
	class RendererAdapter : public HDEngine::I3DRenderer
	{
	public:
		RendererAdapter() {
			MZDX11Renderer::Instance().Initialize();
		};
		virtual ~RendererAdapter() {

		};

		virtual void Initialize(void* hWnd, int screenWidth, int screenHeight)
		{
			MZDX11Renderer::Instance().SetOutputWindow(reinterpret_cast<unsigned int>(hWnd));
		}
		
		// 업데이트
		/*virtual void Update(float deltaTime) override { 
			MZDX11Renderer::Instance().Update(deltaTime); 
		};*/
		
		virtual void Render() override { 
			MZDX11Renderer::Instance().Render(); 
		};

		// 창 크기가 변했을 때 처리해야 할 것들
		/*virtual void ResizeResolution(unsigned int width, unsigned int height) override {
			MZDX11Renderer::Instance().ResizeResolution(width, height);
		};*/

		virtual void Finalize() {}

	};
}
