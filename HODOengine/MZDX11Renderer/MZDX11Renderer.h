#pragma once
#include "I3DRenderer.h"
#include "DX11Define.h"
#include <windows.h>

/// DirectX 11 버전으로 만든 렌더러
/// 2023.06.27 MJKIM

class MZCamera;
class IRenderableObject;
class DeferredBuffers;

class MZDX11Renderer : public MZRenderer::I3DRenderer
{
public:
    MZDX11Renderer();
    ~MZDX11Renderer();
    
    static MZDX11Renderer& Instance() 
    { 
        if (instance == nullptr)
        {
            instance = new MZDX11Renderer();
        }
        return *instance; 
    };

    // 엔진 초기화
    virtual bool Initialize() override;

    // 윈도우 창 초기화
    virtual void SetOutputWindow(unsigned int hWnd) override;

    // 업데이트
    virtual void Update(float deltaTime) override;

    // 렌더
    virtual void Render() override;

    // 엔진 종료
    virtual void Finalize() override;

    // 창 변환 관련
    virtual void ResizeResolution(unsigned int width, unsigned int height) override;

public:
	ID3D11Device* GetDevice() { return m_d3dDevice.Get(); };
	ID3D11DeviceContext* GetDeviceContext() { return m_d3dDeviceContext.Get(); };

	unsigned int GetScreenWidth() const { return m_screenWidth; }
	unsigned int GetScreenHeight() const { return m_screenHeight; }
	float GetAspectRatio() const;

private:
    void ResizeBuffers();

private:
    static MZDX11Renderer* instance;

    HWND m_hWnd;

    ComPtr<ID3D11Device> m_d3dDevice;
    ComPtr<ID3D11DeviceContext> m_d3dDeviceContext;
    ComPtr<IDXGISwapChain> m_swapChain;
    D3D11_VIEWPORT m_viewPort;

    ComPtr<ID3D11Texture2D> m_quadTexture;
    ComPtr<ID3D11RenderTargetView> m_quadRTV;
    ComPtr<ID3D11ShaderResourceView> m_quadSRV;

    ComPtr<ID3D11RenderTargetView> m_backBufferRTV;

	unsigned int m_screenWidth;
	unsigned int m_screenHeight;

    DeferredBuffers* m_deferredBuffers;
};
