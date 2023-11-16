#include "DeferredBuffers.h"


MZRenderer::LazyObjects<DeferredBuffers> DeferredBuffers::Instance;

DeferredBuffers::DeferredBuffers()
{
	for (int iIndex = 0; iIndex < static_cast<int>(BUFFERTYPE::GBUFFER_COUNT); iIndex++)
	{
		m_pRenderTargetTextures[iIndex] = nullptr;
		m_pRenderTagetViews[iIndex] = nullptr;
		m_pShaderResourceViews[iIndex] = nullptr;
	}

	//m_pDepthStencilBuffer = nullptr;
	m_pDepthStencilView = nullptr;

}

DeferredBuffers::~DeferredBuffers()
{

}

void DeferredBuffers::Initialize(ID3D11Device* device, int textureWidth, int textureHeight)
{
	// Initialize the texture size
	m_textureWidth = textureWidth;
	m_textureHeight = textureHeight;

	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));
	textureDesc.Width = m_textureWidth;
	textureDesc.Height = m_textureHeight;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	//textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	// Create textures
	for (int i = 0; i < (int)BUFFERTYPE::GBUFFER_COUNT; ++i)
	{
		HR(device->CreateTexture2D(&textureDesc, NULL, &m_pRenderTargetTextures[i]));
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
		HR(device->CreateRenderTargetView(m_pRenderTargetTextures[i].Get(),
			&rtvDesc, m_pRenderTagetViews[i].GetAddressOf()));
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
		HR(device->CreateShaderResourceView(m_pRenderTargetTextures[i].Get(),
			&srvDesc, m_pShaderResourceViews[i].GetAddressOf()));
	}

	// Create the depth/stencil buffer and view
	ID3D11Texture2D* _pDepthStencilBuffer = nullptr;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));
	depthBufferDesc.Width = m_textureWidth;
	depthBufferDesc.Height = m_textureHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	//depthBufferDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	//depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// Create the texture for the depth buffer
	HR(device->CreateTexture2D(&depthBufferDesc, NULL, &_pDepthStencilBuffer));

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	//dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.Format = depthBufferDesc.Format;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;
	dsvDesc.Flags = 0;

	// Create the depth/stencil view
	HR(device->CreateDepthStencilView(_pDepthStencilBuffer,
		&dsvDesc, &m_pDepthStencilView));

	SafeRelease(_pDepthStencilBuffer);

	/*D3D11_SHADER_RESOURCE_VIEW_DESC depthSRVDesc;
	ZeroMemory(&depthSRVDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	depthSRVDesc.Format = DXGI_FORMAT_R32_FLOAT;
	depthSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	depthSRVDesc.Texture2D.MostDetailedMip = 0;
	depthSRVDesc.Texture2D.MipLevels = 1;

	HR(device->CreateShaderResourceView(m_pDepthStencilBuffer,
		&depthSRVDesc, &m_pZBufferRSV));*/
}

void DeferredBuffers::Finalize()
{
	for (int i = 0; i < (int)BUFFERTYPE::GBUFFER_COUNT; ++i)
	{
		ReleaseCOM(m_pRenderTargetTextures[i]);
		ReleaseCOM(m_pRenderTagetViews[i]);
		ReleaseCOM(m_pShaderResourceViews[i]);
	}
	//SafeRelease(m_pZBufferRSV);
	//SafeRelease(m_pDepthStencilBuffer);
	/*if (m_pDepthStencilView.Get())
		m_pDepthStencilView->Release();*/
	ReleaseCOM(m_pDepthStencilView);
}

void DeferredBuffers::SetRenderTargets(ID3D11DeviceContext* deviceContext)
{
	// Sets the render targets in the array as the location where the shaders will write to
	deviceContext->OMSetRenderTargets((int)BUFFERTYPE::GBUFFER_COUNT, m_pRenderTagetViews->GetAddressOf(), m_pDepthStencilView.Get());
}

void DeferredBuffers::ClearRenderTargets(ID3D11DeviceContext* deviceContext, XMVECTOR color)
{
	// Clear all the render targets
	for (int iTextureIndex = 0; iTextureIndex < (int)BUFFERTYPE::GBUFFER_COUNT; iTextureIndex++)
	{
		deviceContext->ClearRenderTargetView(m_pRenderTagetViews[iTextureIndex].Get(), reinterpret_cast<const float*>(&color));
	}

	// Clear the depth/stencil buffer
	deviceContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}


