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

	void BuildQuadBuffers();

	void EnableZBuffering();
	void DisableZBuffering();

	void RenderAll();

	//--------
	void ResizeBuffers();
	HRESULT GetAdapterInfo();

	// µð¹ö±ë Á¤º¸ Ãâ·Â
	void DrawStatus();
	
private:
	DeferredRenderer();

	ComPtr<ID3D11Device> m_d3dDevice;
	ComPtr<ID3D11DeviceContext> m_d3dDeviceContext;
	ComPtr<IDXGISwapChain> m_swapChain;
	ComPtr<ID3D11Texture2D> m_depthStencilBuffer;		// µª½º ½ºÅÙ½Ç ¹öÆÛ
	ComPtr<ID3D11DepthStencilState> m_depthStencilStateEnable;
	ComPtr<ID3D11DepthStencilState> m_depthStencilStateDisable;
	ComPtr<ID3D11RenderTargetView> m_backBufferRTV;	// ¹é¹öÆÛ ·»´õ Å¸°Ù ºä
	ComPtr<ID3D11DepthStencilView> m_depthStencilView;	// µª½º ½ºÅÙ½Ç ºä
	D3D11_VIEWPORT m_viewPort;							// ºä Æ÷Æ®

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

	GeometryGenerator* m_geometryGen;
	DXTKFont* m_font;

	// Input Key
	int switcher;

	::Mesh* m_axisMesh;
	Material* m_axisMaterial;
};

