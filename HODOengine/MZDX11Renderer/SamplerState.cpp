#include "SamplerState.h"

LazyObjects<SamplerState> SamplerState::Instance;

SamplerState::SamplerState()
	: m_textureSampler(0)
{

}

void SamplerState::CreateSamplerStates(ID3D11Device* device)
{
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	device->CreateSamplerState(&sampDesc, &m_textureSampler);
}

void SamplerState::DestroySamplerStates()
{
	m_textureSampler.Reset();
}

