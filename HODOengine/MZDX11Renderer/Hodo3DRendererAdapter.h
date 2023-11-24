#pragma once
#include "IHodoGIRenderer.h"
#include "MZDX11Renderer.h"

namespace hodoGIAdapter
{
	class RendererAdapter : public hodoGI::I3DRenderer
	{
	public:
		RendererAdapter() {
			MZDX11Renderer::Instance.Get().Initialize();
		};
		virtual ~RendererAdapter() {

		};

		// DX를 초기화한다.
		//virtual void Initialize() { }
		// 그림을 그릴 대상 윈도우를 정함
		virtual void SetOutputWindow(UINT64 hWnd) override { 
			MZDX11Renderer::Instance.Get().SetOutputWindow(hWnd); 
		};
		// 업데이트
		virtual void Update(float deltaTime) override { 
			MZDX11Renderer::Instance.Get().Update(deltaTime); 
		};
		
		virtual void Render() override { 
			MZDX11Renderer::Instance.Get().Render(); 
		};

		// 창 크기가 변했을 때 처리해야 할 것들
		virtual void ResizeResolution(unsigned int width, unsigned int height) override { 
			MZDX11Renderer::Instance.Get().ResizeResolution(width, height); 
		};

	};
}