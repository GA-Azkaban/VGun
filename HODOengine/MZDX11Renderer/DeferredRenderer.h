#pragma once
#include "DX11Define.h"
#include "LazyObjects.h"
#include <map>

class IRenderableObject;
class MZCamera;
class DXTKFont;
class GeometryGenerator;
class TextureBox;
class Mesh;
class Material;
class Sky;
class SkinningMeshObject;

class DeferredRenderer
{
public:
	~DeferredRenderer();
	static LazyObjects<DeferredRenderer> Instance;
	friend LazyObjects<DeferredRenderer>;

	void Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, IDXGISwapChain* swapChain, unsigned int screenWidth, unsigned int screenHeight);
	void Update(float deltaTime);
	void RenderToBackBuffer();

	void ResizeResolution(unsigned int screenWidth, unsigned int screenHeight);

	void SetRenderTarget(ID3D11RenderTargetView* rtv, ID3D11DepthStencilView* dsv);
	void ClearRenderTarget(ID3D11RenderTargetView* rtv, ID3D11DepthStencilView* dsv, const float color[4]);
	
	IRenderableObject* Pick(float normalizedX, float normalizedY);

private:
	void CreateDepthStecilStates();

	void SetLights();

	void BuildQuadBuffers();

	void EnableZBuffering();
	void DisableZBuffering();
	void SetCubemapDSS();

	void RenderAll();

	//--------
	void ResizeBuffers();
	HRESULT GetAdapterInfo();

	// 디버깅 정보 출력
	void DrawStatus();
	
private:
	DeferredRenderer();

	ComPtr<ID3D11Device> m_d3dDevice;
	ComPtr<ID3D11DeviceContext> m_d3dDeviceContext;
	ComPtr<IDXGISwapChain> m_swapChain;
	ComPtr<ID3D11Texture2D> m_depthStencilBuffer;		// 뎁스 스텐실 버퍼
	ComPtr<ID3D11DepthStencilState> m_depthStencilStateEnable;
	ComPtr<ID3D11DepthStencilState> m_depthStencilStateDisable;
	ComPtr<ID3D11DepthStencilState> m_depthStencilStateCubeMap;
	ComPtr<ID3D11RenderTargetView> m_backBufferRTV;	// 백버퍼 렌더 타겟 뷰
	ComPtr<ID3D11DepthStencilView> m_depthStencilView;	// 뎁스 스텐실 뷰
	D3D11_VIEWPORT m_viewPort;


	// Adapter Info
	DXGI_ADAPTER_DESC1 m_AdapterDesc1;

	ID3D11Buffer* m_quadVB;
	ID3D11Buffer* m_quadIB;

	unsigned int m_screenWidth;
	unsigned int m_screenHeight;

	float m_deltaTime;

	/// Objects
	std::map<float, IRenderableObject*> pickedObjects;

	//std::vector<TextureBox*> m_textureBoxes;

	DXTKFont* m_font;

	// Input Key
	int switcher;

	// 임시
	Sky* sky;
	SkinningMeshObject* test1;
};

