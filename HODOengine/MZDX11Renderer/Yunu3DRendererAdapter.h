#pragma once
#include "YunuGraphicsInterface.h"
#include "MZDX11Renderer.h"
#include "DeferredRenderer.h"

namespace yunuGIAdapter
{
	class RendererAdapter : public yunuGI::I3DRenderer
	{
	public:
		RendererAdapter() { MZDX11Renderer::Instance.Get().Initialize(); };
		virtual ~RendererAdapter() {}

		// DX를 초기화한다.
		//virtual bool Initialize() { return true; };
		// 그림을 그릴 대상 윈도우를 정함
		virtual void SetOutputWindow(UINT64 hWnd) { MZDX11Renderer::Instance.Get().SetOutputWindow(hWnd); };
		// 업데이트
		virtual void Update(float deltaTime) { MZDX11Renderer::Instance.Get().Update(deltaTime); };
		// 렌더링 시작
		//virtual void BeginRender() { mzRenderer.BeginRender(); };
		virtual void Render() { MZDX11Renderer::Instance.Get().Render(); };
		// 렌더링 마무리. 그릴 것 다 그린 상태에서 Present
		//virtual void EndRender() { mzRenderer.EndRender(); };

		// 마우스 이동 관련
		//virtual void OnMouseDown(int x, int y) { mzRenderer.OnMouseDown(x, y); };
		//virtual void OnMouseUp(int x, int y) { mzRenderer.OnMouseUp(x, y); };
		//virtual void OnMouseMove(int btnState, int x, int y) { mzRenderer.OnMouseMove(btnState, x, y); };

		// 창 크기가 변했을 때 처리해야 할 것들
		virtual void ResizeResolution(unsigned int width, unsigned int height) { MZDX11Renderer::Instance.Get().ResizeResolution(width, height); };

		// 혹여나 내부 그래픽스 구조를 알아야 할 때
		virtual ID3D11Device* QueryD3d11Device() { return MZDX11Renderer::Instance.Get().m_d3dDevice.Get(); }
		virtual ID3D11DeviceContext* QueryD3dDeviceContext() { return MZDX11Renderer::Instance.Get().m_d3dImmediateContext.Get(); }
		//virtual ID3D11ShaderResourceView* QueryD3D11RenderOutputSRV() { return mzRenderer.m_deferredRenderer->m_deferredBuffers->GetShaderResourceView(0); }
		virtual ID3D11ShaderResourceView* QueryD3D11RenderOutputSRV() { return nullptr; }
		virtual void* QuerySharedOutputHandle()
		{
			return DeferredRenderer::Instance.Get().GetSharedBackBufferTextureHandle();
		}
	};
}
