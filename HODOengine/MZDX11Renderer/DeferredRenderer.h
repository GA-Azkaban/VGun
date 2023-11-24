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

	void Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, float screenWidth, float screenHeight);
	void Update(MZCamera* pCamera, float deltaTime);
	void RenderToBackBuffer();

	void SetRenderTarget(ID3D11RenderTargetView* rtv, ID3D11DepthStencilView* dsv);
	void ClearRenderTarget(ID3D11RenderTargetView* rtv, ID3D11DepthStencilView* dsv, const float color[4]);
	
	ID3D11Device* GetDevice() { return m_d3dDevice.Get(); };
	ID3D11DeviceContext* GetDeviceContext() { return m_d3dImmediateContext.Get(); };
	IRenderableObject* Pick(MZCamera* pCamera, float normalizedX, float normalizedY);

private:
	void CreateDepthStecilStates();
	void SetLights();
	void BuildQuadBuffers();


	void EnableZBuffering();
	void DisableZBuffering();

	void RenderAll();
	
private:
	DeferredRenderer();

	ComPtr<ID3D11Device> m_d3dDevice;
	ComPtr<ID3D11DeviceContext> m_d3dImmediateContext;

	ComPtr<ID3D11DepthStencilState> m_depthStencilStateEnable;
	ComPtr<ID3D11DepthStencilState> m_depthStencilStateDisable;

	ID3D11Buffer* m_VB;
	ID3D11Buffer* m_QuadVB;
	ID3D11Buffer* m_IB;
	ID3D11Buffer* m_QuadIB;

	DirectionalLight m_dirLights[3];
	PointLight m_pointLights[3];
	SpotLight m_spotLights[3];
	Material m_material;

	XMFLOAT3 mEyePosW;

private:
	/// Objects
	std::map<float, IRenderableObject*> pickedObjects;

	std::vector<TextureBox*> m_textureBoxes;

	float m_screenWidth;
	float m_screenHeight;

	// Input Key
	int switcher;
};

