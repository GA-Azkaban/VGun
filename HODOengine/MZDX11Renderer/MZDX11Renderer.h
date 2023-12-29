#pragma once
#include "I3DRenderer.h"
#include "DX11Define.h"
#include <windows.h>

/// DirectX 11 버전으로 만든 렌더러
/// 2023.06.27 MJKIM

class MZCamera;
class IRenderableObject;
//class DeferredRenderer;

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

	ID3D11Device* GetDevice() { return m_d3dDevice.Get(); };
	ID3D11DeviceContext* GetDeviceContext() { return m_d3dDeviceContext.Get(); };

    unsigned int GetScreenWidth() const { return m_screenWidth; }
    unsigned int GetScreenHeight() const { return m_screenHeight; }
    float GetAspectRatio() const;

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

	// 마우스 입력 관련
	virtual void OnMouseDown(int btnState, int x, int y) override;
	virtual void OnMouseUp(int x, int y) override;
	virtual void OnMouseMove(int btnState, int x, int y) override;
    
    // 이벤트(클릭, 컬링 등) 관련
    IRenderableObject* Pick(float normalizedX, float normalizedY);

private:
    static MZDX11Renderer* instance;

    HWND m_hWnd;

    ComPtr<ID3D11Device> m_d3dDevice;
    ComPtr<ID3D11DeviceContext> m_d3dDeviceContext;
    ComPtr<IDXGISwapChain> m_swapChain;

	unsigned int m_screenWidth;
	unsigned int m_screenHeight;

    MZCamera* m_camera;
    float m_cameraSpeed;

    POINT m_lastMousePos;
    float m_deltaTime;
};
