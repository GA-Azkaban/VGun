#pragma once
#include "DX11Define.h"
#include "LazyObjects.h"

// Render Target Type
enum class BUFFERTYPE
{
	BUFFER_POSITION,
	BUFFER_NORNAL,
	BUFFER_DIFFUSE,
	BUFFER_TANGENT,
	BUFFER_NORMALMAP,

	GBUFFER_COUNT,
};

class DeferredBuffers
{
public:
	DeferredBuffers();
	~DeferredBuffers();

	void Initialize(ID3D11Device* device, UINT textureWidth, UINT textureHeight);

	void SetRenderTargets(ID3D11DeviceContext* deviceContext);
	void ClearRenderTargets(ID3D11DeviceContext* deviceContext, XMVECTOR color);

private:
	UINT m_textureWidth;
	UINT m_textureHeight;

	// G-Buffer
	Microsoft::WRL::ComPtr<ID3D11Texture2D> _renderTargetTextures[(int)BUFFERTYPE::GBUFFER_COUNT];
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> _renderTargetViews[(int)BUFFERTYPE::GBUFFER_COUNT];
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _shaderResourceViews[(int)BUFFERTYPE::GBUFFER_COUNT];
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> _depthStencilView;
};

