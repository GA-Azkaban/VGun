#include "DeferredBuffers.h"

DeferredBuffers::DeferredBuffers(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
	: _device(device), _deviceContext(deviceContext)
{
	for (int iIndex = 0; iIndex < static_cast<int>(BUFFERTYPE::GBUFFER_COUNT); iIndex++)
	{
		_renderTargetTextures[iIndex] = nullptr;
		_renderTargetViews[iIndex] = nullptr;
		_shaderResourceViews[iIndex] = nullptr;
	}
	_depthStencilView = nullptr;
}

DeferredBuffers::~DeferredBuffers()
{
	for (int i = 0; i < (int)BUFFERTYPE::GBUFFER_COUNT; ++i)
	{
		_renderTargetTextures[i]->Release();
		_renderTargetViews[i]->Release();
		_shaderResourceViews[i]->Release();
	}
	_depthStencilView->Release();
}

void DeferredBuffers::Initialize(UINT textureWidth, UINT textureHeight)
{
	for (int i = 0; i < (int)BUFFERTYPE::GBUFFER_COUNT; ++i)
	{
		_renderTargetTextures[i].Reset();
		_renderTargetViews[i].Reset();
		_shaderResourceViews[i].Reset();
	}
	_depthStencilView.Reset();

	// Initialize the texture size
	m_textureWidth = textureWidth;
	m_textureHeight = textureHeight;

	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));
	textureDesc.Width = m_textureWidth;
	textureDesc.Height = m_textureHeight;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	//textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	// Create textures
	for (int i = 0; i < (int)BUFFERTYPE::GBUFFER_COUNT; ++i)
	{
		_device->CreateTexture2D(&textureDesc, NULL, &_renderTargetTextures[i]);
	}

	// Create render target view to be able to access the render target textures
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	ZeroMemory(&rtvDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
	rtvDesc.Format = textureDesc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;

	// Create render target views
	for (int i = 0; i < (int)BUFFERTYPE::GBUFFER_COUNT; ++i)
	{
		_device->CreateRenderTargetView(_renderTargetTextures[i].Get(),
			&rtvDesc, _renderTargetViews[i].GetAddressOf());
	}

	// Create the shader resource views for each texture
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	// Create the shader resource views
	for (int i = 0; i < (int)BUFFERTYPE::GBUFFER_COUNT; ++i)
	{
		_device->CreateShaderResourceView(_renderTargetTextures[i].Get(),
			&srvDesc, _shaderResourceViews[i].GetAddressOf());
	}

	// Create the depth/stencil buffer and view
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
	depthBufferDesc.Width = m_textureWidth;
	depthBufferDesc.Height = m_textureHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// Create the texture for the depth buffer
	_device->CreateTexture2D(&depthBufferDesc, NULL, _depthTexture.GetAddressOf());

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;
	dsvDesc.Flags = 0;

	// Create the depth/stencil view
	_device->CreateDepthStencilView(_depthTexture.Get(),
		&dsvDesc, _depthStencilView.GetAddressOf());

	D3D11_SHADER_RESOURCE_VIEW_DESC depthSRVDesc;
	depthSRVDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	depthSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	depthSRVDesc.Texture2D.MostDetailedMip = 0;
	depthSRVDesc.Texture2D.MipLevels = 1;

	_device->CreateShaderResourceView(_depthTexture.Get(), &depthSRVDesc, _depthShaderResourceView.GetAddressOf());
}

void DeferredBuffers::SetRenderTargets()
{
	// Sets the render targets in the array as the location where the shaders will write to
	_deviceContext->OMSetRenderTargets((int)BUFFERTYPE::GBUFFER_COUNT, _renderTargetViews->GetAddressOf(), _depthStencilView.Get());
}

ID3D11ShaderResourceView* DeferredBuffers::GetShaderResourceView(UINT index)
{
	if (index >= (int)BUFFERTYPE::GBUFFER_COUNT)
		return nullptr;

	return _shaderResourceViews[index].Get();
}

ID3D11DepthStencilView* DeferredBuffers::GetDepthStencilView()
{
	return _depthStencilView.Get();
}

ID3D11ShaderResourceView* DeferredBuffers::GetDepthSRV()
{
	return _depthShaderResourceView.Get();
}

void DeferredBuffers::ClearRenderTargets(XMVECTOR color)
{
	// Clear all the render targets
	for (int iTextureIndex = 0; iTextureIndex < (int)BUFFERTYPE::GBUFFER_COUNT; iTextureIndex++)
	{
		_deviceContext->ClearRenderTargetView(_renderTargetViews[iTextureIndex].Get(), reinterpret_cast<const float*>(&color));
	}

	// Clear the depth/stencil buffer
	_deviceContext->ClearDepthStencilView(_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void DeferredBuffers::FlushShaderResourceViews()
{
	ID3D11ShaderResourceView* shaderResView = NULL;
	for (UINT i = 0; i < (int)BUFFERTYPE::GBUFFER_COUNT + 1; ++i)
	{
		_deviceContext->PSSetShaderResources(i, 1, &shaderResView);
	}
}