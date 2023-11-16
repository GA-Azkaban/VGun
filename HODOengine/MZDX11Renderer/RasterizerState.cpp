#include "RasterizerState.h"

void RasterizerState::CreateRenderStates(ID3D11Device* device)
{
	D3D11_RASTERIZER_DESC solidDesc;
	ZeroMemory(&solidDesc, sizeof(D3D11_RASTERIZER_DESC));
	solidDesc.FillMode = D3D11_FILL_SOLID;
	solidDesc.CullMode = D3D11_CULL_BACK;
	solidDesc.FrontCounterClockwise = false;
	solidDesc.DepthClipEnable = true;

	HR(device->CreateRasterizerState(&solidDesc, SolidRS.GetAddressOf()));

	D3D11_RASTERIZER_DESC wireframeDesc;
	ZeroMemory(&wireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
	wireframeDesc.FillMode = D3D11_FILL_WIREFRAME;
	wireframeDesc.CullMode = D3D11_CULL_BACK;
	wireframeDesc.FrontCounterClockwise = false;
	wireframeDesc.DepthClipEnable = true;

	HR(device->CreateRasterizerState(&wireframeDesc, WireframeRS.GetAddressOf()));
}

void RasterizerState::DestroyRenderStates()
{
	ReleaseCOM(SolidRS);
	ReleaseCOM(WireframeRS);
}

ComPtr<ID3D11RasterizerState> RasterizerState::WireframeRS = 0;
ComPtr<ID3D11RasterizerState> RasterizerState::SolidRS = 0;