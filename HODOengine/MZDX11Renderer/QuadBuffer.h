#pragma once
#include "DX11Define.h"

class QuadBuffer
{
public:
	QuadBuffer(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	~QuadBuffer();

	void Initialize(UINT textureWidth, UINT textureHeight);

	ID3D11ShaderResourceView* GetShaderResourceView();
	void SetRenderTargets(ID3D11DepthStencilView* dsv = nullptr);
	void ClearRenderTargets(XMVECTOR color = XMVECTOR{ 0.0f, 0.0f, 0.0f, 0.0f });
	void FlushShaderResourceViews();

private:
	UINT m_textureWidth;
	UINT m_textureHeight;

	Microsoft::WRL::ComPtr<ID3D11Device> _device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> _deviceContext;

	// G-Buffer
	Microsoft::WRL::ComPtr<ID3D11Texture2D> _renderTargetTexture;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> _renderTargetView;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _shaderResourceView;
};

