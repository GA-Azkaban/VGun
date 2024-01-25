#include "QuadBuffer.h"

QuadBuffer::QuadBuffer(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
	: _device(device), _deviceContext(deviceContext),
	_renderTargetTexture(0), _renderTargetView(0), _shaderResourceView(0)
{

}

QuadBuffer::~QuadBuffer()
{
	_renderTargetTexture->Release();
	_renderTargetView->Release();
	_shaderResourceView->Release();
}

void QuadBuffer::Initialize(UINT textureWidth, UINT textureHeight)
{
	_renderTargetTexture.Reset();
	_renderTargetView.Reset();
	_shaderResourceView.Reset();

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
	_device->CreateTexture2D(&textureDesc, NULL, &_renderTargetTexture);

	// Create render target view to be able to access the render target textures
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	ZeroMemory(&rtvDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
	rtvDesc.Format = textureDesc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;

	// Create render target views	
	_device->CreateRenderTargetView(_renderTargetTexture.Get(),
		&rtvDesc, _renderTargetView.GetAddressOf());

	// Create the shader resource views for each texture
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	// Create the shader resource views	
	_device->CreateShaderResourceView(_renderTargetTexture.Get(),
		&srvDesc, _shaderResourceView.GetAddressOf());
}

ID3D11ShaderResourceView* QuadBuffer::GetShaderResourceView()
{
	return _shaderResourceView.Get();
}

void QuadBuffer::SetRenderTargets(ID3D11DepthStencilView* dsv)
{
	_deviceContext->OMSetRenderTargets(1, _renderTargetView.GetAddressOf(), dsv);
}

void QuadBuffer::ClearRenderTargets(XMVECTOR color)
{
	_deviceContext->ClearRenderTargetView(_renderTargetView.Get(), reinterpret_cast<const float*>(&color));	
}

void QuadBuffer::FlushShaderResourceViews()
{
	ID3D11ShaderResourceView* shaderResView = NULL;
	_deviceContext->PSSetShaderResources(0, 1, &shaderResView);
}
