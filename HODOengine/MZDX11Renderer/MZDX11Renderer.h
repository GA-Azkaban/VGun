#pragma once
#include "I3DRenderer.h"
#include "DX11Define.h"
#include "LazyObjects.h"
#include <windows.h>

/// DirectX 11 버전으로 만든 렌더러
/// 2023.06.27 MJKIM

class DXTKFont;
class MZCamera;
class IRenderableObject;
class GridBuilder;
class DeferredRenderer;

class XLParticleEffect;
class XLParticleSystem;

namespace yunuGIAdapter
{
    class RendererAdapter;
}
class MZDX11Renderer : public MZ3DAPI::I3DRenderer
{
public:
    MZDX11Renderer();
    ~MZDX11Renderer();

    static MZRenderer::LazyObjects<MZDX11Renderer> Instance;
    friend MZRenderer::LazyObjects<MZDX11Renderer>;
    friend yunuGIAdapter::RendererAdapter;

    float GetScreenWidth() { return m_screenWidth; }
    float GetScreenHeight() { return m_screenHeight; }

    // 엔진 초기화
    bool Initialize();
    void SetOutputWindow(unsigned int hWnd);
    // 업데이트
    void Update(float deltaTime);

    // 렌더
    void Render();

    // 엔진 종료
    void Finalize();

public:
    // 창 변환 관련
    virtual void ResizeResolution(unsigned int width, unsigned int height);
    void OnResize();
    float GetAspectRatio() const;

private:
    float m_screenWidth;
    float m_screenHeight;

public:
    // 마우스 입력 관련
    void OnMouseDown(int btnState, int x, int y);
    void OnMouseUp(int x, int y);
    void OnMouseMove(int btnState, int x, int y);

    // 이벤트(클릭, 컬링 등) 관련
    IRenderableObject* Pick(float normalizedX, float normalizedY);

private:
    // Adapter Info
    DXGI_ADAPTER_DESC1 m_AdapterDesc1;
    HRESULT GetAdapterInfo();


private:
    void BeginRender();
    void EndRender();
    // 디버깅 정보 출력
    int switcher;
    void DrawStatus();

private:
    HWND m_hWnd;
    UINT m_4xMsaaQuality;
    bool m_enable4xMsaa;

    ComPtr<ID3D11Device> m_d3dDevice;					// D3D11 디바이스	
    ComPtr<ID3D11DeviceContext> m_d3dImmediateContext;		// 디바이스 컨텍스트	
    ComPtr<IDXGISwapChain> m_swapChain;					// 스왑체인
    //ComPtr<ID3D11Texture2D> m_depthStencilBuffer;		// 뎁스 스텐실 버퍼
    ComPtr<ID3D11RenderTargetView> m_backBufferRTV;	// 백버퍼 렌더 타겟 뷰
    //ComPtr<ID3D11DepthStencilView> m_depthStencilView;	// 뎁스 스텐실 뷰
    //D3D11_VIEWPORT m_viewPort;							// 뷰 포트
    ComPtr<ID3D11Texture2D> m_backBufferTexture;
    D3D_DRIVER_TYPE m_d3dDriverType;
    D3D_FEATURE_LEVEL m_featureLevel;

private:
    DXTKFont* m_pFont;
    float m_fontLineSpace;

    MZCamera* m_pCamera;
    float m_cameraSpeed;

    POINT m_lastMousePos;
    float m_deltaTime;

    /// Builder
    //GridBuilder* m_gridBuilder;

private:

	XLParticleSystem* m_pParticleSystem;
};

extern "C" __declspec(dllexport) MZ3DAPI::I3DRenderer * CreateDX11GraphicsInstance();
extern "C" __declspec(dllexport) void ReleaseDX11GraphicsInstance(MZ3DAPI::I3DRenderer * instance);