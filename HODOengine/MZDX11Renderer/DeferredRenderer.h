#pragma once
#include "DX11Define.h"
#include "LazyObjects.h"

class IRenderableObject;
class MZCamera;
class TextureBox;

class DeferredRenderer
{
public:
	~DeferredRenderer();
	static MZRenderer::LazyObjects<DeferredRenderer> Instance;
	friend MZRenderer::LazyObjects<DeferredRenderer>;

	static constexpr unsigned int BACK_BUFFER_COUNT = 2;

	void Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, float screenWidth, float screenHeight);
	void Update(MZCamera* pCamera, float deltaTime);
	void RenderToBackBuffer();

	void* GetSharedBackBufferTextureHandle();
	ID3D11Texture2D* GetBackBufferTexture() { return m_backBufferTextures[frontBufferIndex].Get(); }
	void SwapBackBuffer();
	ID3D11Device* GetDevice() { return m_d3dDevice.Get(); };
	ID3D11DeviceContext* GetDeviceContext() { return m_d3dImmediateContext.Get(); };
	void SetRenderTargets();
	IRenderableObject* Pick(MZCamera* pCamera, float normalizedX, float normalizedY);

	//void Render(ID3D11RenderTargetView* rtv, ID3D11DepthStencilView* dsv);

private:
	void SetUpViewport();
	
	void CreateResources();
	void SetLights();
	void BuildQuadBuffers();

	void ClearRenderTargets(const float color[4]);

	void EnableZBuffering();
	void DisableZBuffering();

	void RenderAll();
	
private:
	DeferredRenderer();

	ComPtr<ID3D11Device> m_d3dDevice;
	ComPtr<ID3D11DeviceContext> m_d3dImmediateContext;
	ComPtr<ID3D11Texture2D> m_depthStencilBuffer;		// µª½º ½ºÅÙ½Ç ¹öÆÛ
	ComPtr<ID3D11DepthStencilView> m_depthStencilView;	// µª½º ½ºÅÙ½Ç ºä
	Microsoft::WRL::ComPtr<ID3D11Texture2D>			m_backBufferTextures[BACK_BUFFER_COUNT];
	ComPtr<ID3D11RenderTargetView> m_backBufferRTV[BACK_BUFFER_COUNT];	// ¹é¹öÆÛ ·»´õ Å¸°Ù ºä

	ComPtr<ID3D11DepthStencilState> m_depthStencilStateEnable;
	ComPtr<ID3D11DepthStencilState> m_depthStencilStateDisable;

	D3D11_VIEWPORT m_viewport;

	ID3D11Buffer* m_VB;
	ID3D11Buffer* m_QuadVB;
	ID3D11Buffer* m_IB;
	ID3D11Buffer* m_QuadIB;

	// Transform Matrix
	//XMFLOAT4X4 m_world;	// Define transformations from local spaces to world space.
	//XMFLOAT4X4 m_view;
	//XMFLOAT4X4 m_proj;

	DirectionalLight m_dirLights[3];
	PointLight m_pointLights[3];
	SpotLight m_spotLights[3];
	Material m_material;

	XMFLOAT3 mEyePosW;

	UINT frontBufferIndex = 0;
	UINT backBufferIndex = 1;
	HANDLE m_backBufferSharedHandles[BACK_BUFFER_COUNT]{ nullptr };

private:
	/// Objects
	std::multimap<float, IRenderableObject*> pickedObjects;

	//Genji* m_genji;
	std::vector<TextureBox*> m_textureBoxes;

	float m_screenWidth;
	float m_screenHeight;

	// Input Key
	int switcher;
};

