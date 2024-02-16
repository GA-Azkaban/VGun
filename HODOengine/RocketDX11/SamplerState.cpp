#include "SamplerState.h"

namespace RocketCore::Graphics
{
	SamplerState::SamplerState()
	{

	}

	void SamplerState::CreateSamplerStates(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
	{
		D3D11_SAMPLER_DESC linearSampDesc;
		ZeroMemory(&linearSampDesc, sizeof(linearSampDesc));
		linearSampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		linearSampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		linearSampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		linearSampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		linearSampDesc.MipLODBias = 0.0f;
		linearSampDesc.MaxAnisotropy = 1;
		linearSampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		linearSampDesc.MinLOD = 0;
		linearSampDesc.MaxLOD = D3D11_FLOAT32_MAX;

		device->CreateSamplerState(&linearSampDesc, _linearSampler.GetAddressOf());

		D3D11_SAMPLER_DESC pointSampDesc;
		ZeroMemory(&pointSampDesc, sizeof(pointSampDesc));
		pointSampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		pointSampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		pointSampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		pointSampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		pointSampDesc.MipLODBias = 0.0f;
		pointSampDesc.MaxAnisotropy = 1;
		pointSampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		pointSampDesc.MinLOD = 0;
		pointSampDesc.MaxLOD = D3D11_FLOAT32_MAX;

		device->CreateSamplerState(&pointSampDesc, _pointSampler.GetAddressOf());
		
		D3D11_SAMPLER_DESC shadowSampDesc;
		ZeroMemory(&shadowSampDesc, sizeof(shadowSampDesc));
		shadowSampDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
		shadowSampDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
		shadowSampDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
		shadowSampDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
		shadowSampDesc.BorderColor[0] = shadowSampDesc.BorderColor[1] = shadowSampDesc.BorderColor[2] = shadowSampDesc.BorderColor[3] = 1.0f;		
		shadowSampDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;

		device->CreateSamplerState(&shadowSampDesc, _shadowSampler.GetAddressOf());

		SetSamplers(deviceContext);
	}

	void SamplerState::DestroySamplerStates()
	{
		_linearSampler.Reset();
		_pointSampler.Reset();
	}

	void SamplerState::SetSamplers(ID3D11DeviceContext* deviceContext)
	{
		deviceContext->PSSetSamplers(0, 1, _linearSampler.GetAddressOf());
		deviceContext->PSSetSamplers(1, 1, _pointSampler.GetAddressOf());
		deviceContext->PSSetSamplers(2, 1, _shadowSampler.GetAddressOf());
	}
}
