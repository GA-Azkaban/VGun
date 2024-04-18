#include "RasterizerState.h"

LazyObjects<RasterizerState> RasterizerState::Instance;

RasterizerState::RasterizerState()
	: m_wireframeRS(0), m_solidRS(0), m_cubeMapRS(0)
{
	
}

void RasterizerState::CreateRenderStates(ID3D11Device* device)
{
	D3D11_RASTERIZER_DESC solidDesc;
	ZeroMemory(&solidDesc, sizeof(D3D11_RASTERIZER_DESC));
	solidDesc.FillMode = D3D11_FILL_SOLID;
	solidDesc.CullMode = D3D11_CULL_BACK;
	solidDesc.FrontCounterClockwise = false;
	solidDesc.DepthClipEnable = true;

	device->CreateRasterizerState(&solidDesc, m_solidRS.GetAddressOf());

	D3D11_RASTERIZER_DESC wireframeDesc;
	ZeroMemory(&wireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
	wireframeDesc.FillMode = D3D11_FILL_WIREFRAME;
	wireframeDesc.CullMode = D3D11_CULL_BACK;
	wireframeDesc.FrontCounterClockwise = false;
	wireframeDesc.DepthClipEnable = true;

	device->CreateRasterizerState(&wireframeDesc, m_wireframeRS.GetAddressOf());

	D3D11_RASTERIZER_DESC cubeMapDesc;
	ZeroMemory(&cubeMapDesc, sizeof(D3D11_RASTERIZER_DESC));
	cubeMapDesc.FillMode = D3D11_FILL_SOLID;
	cubeMapDesc.CullMode = D3D11_CULL_NONE;
	cubeMapDesc.FrontCounterClockwise = false;
	cubeMapDesc.DepthClipEnable = true;

	device->CreateRasterizerState(&cubeMapDesc, m_cubeMapRS.GetAddressOf());
}

void RasterizerState::DestroyRenderStates()
{
	m_solidRS.Reset();
	m_wireframeRS.Reset();
	m_cubeMapRS.Reset();
}