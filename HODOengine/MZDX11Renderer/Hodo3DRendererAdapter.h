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

		// DX�� �ʱ�ȭ�Ѵ�.
		//virtual void Initialize() { }
		// �׸��� �׸� ��� �����츦 ����
		virtual void SetOutputWindow(UINT64 hWnd) override { 
			MZDX11Renderer::Instance.Get().SetOutputWindow(hWnd); 
		};
		// ������Ʈ
		virtual void Update(float deltaTime) override { 
			MZDX11Renderer::Instance.Get().Update(deltaTime); 
		};
		
		virtual void Render() override { 
			MZDX11Renderer::Instance.Get().Render(); 
		};

		// â ũ�Ⱑ ������ �� ó���ؾ� �� �͵�
		virtual void ResizeResolution(unsigned int width, unsigned int height) override { 
			MZDX11Renderer::Instance.Get().ResizeResolution(width, height); 
		};

	};
}