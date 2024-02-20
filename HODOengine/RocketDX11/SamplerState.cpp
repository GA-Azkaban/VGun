#include "SamplerState.h"

namespace RocketCore::Graphics
{
	
	void SamplerState::CreateSamplerStates(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
	{
		D3D11_SAMPLER_DESC linearWrapSampDesc;
		ZeroMemory(&linearWrapSampDesc, sizeof(linearWrapSampDesc));
		linearWrapSampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		linearWrapSampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		linearWrapSampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		linearWrapSampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		linearWrapSampDesc.MipLODBias = 0.0f;
		linearWrapSampDesc.MaxAnisotropy = 1;
		linearWrapSampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		linearWrapSampDesc.MinLOD = 0;
		linearWrapSampDesc.MaxLOD = D3D11_FLOAT32_MAX;

		device->CreateSamplerState(&linearWrapSampDesc, _linearWrapSampler.GetAddressOf());

		D3D11_SAMPLER_DESC linearBorderSampDesc;
		ZeroMemory(&linearBorderSampDesc, sizeof(linearBorderSampDesc));
		linearBorderSampDesc.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
		linearBorderSampDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
		linearBorderSampDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
		linearBorderSampDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
		linearBorderSampDesc.MipLODBias = 0.0f;
		linearBorderSampDesc.MaxAnisotropy = 1;
		linearBorderSampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		linearBorderSampDesc.MinLOD = 0;
		linearBorderSampDesc.MaxLOD = D3D11_FLOAT32_MAX;

		device->CreateSamplerState(&linearBorderSampDesc, _linearBorderSampler.GetAddressOf());

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

		device->CreateSamplerState(&pointSampDesc, _pointClampSampler.GetAddressOf());
		
		D3D11_SAMPLER_DESC shadowSampDesc;
		ZeroMemory(&shadowSampDesc, sizeof(shadowSampDesc));
		shadowSampDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
		shadowSampDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
		shadowSampDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
		shadowSampDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
		shadowSampDesc.BorderColor[0] = shadowSampDesc.BorderColor[1] = shadowSampDesc.BorderColor[2] = shadowSampDesc.BorderColor[3] = 1.0f;		
		shadowSampDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;

		device->CreateSamplerState(&shadowSampDesc, _comparisonSampler.GetAddressOf());

		SetSamplers(deviceContext);
	}

	void SamplerState::DestroySamplerStates()
	{
		_linearWrapSampler.Reset();
		_linearBorderSampler.Reset();
		_pointClampSampler.Reset();
		_comparisonSampler.Reset();
	}

	void SamplerState::SetSamplers(ID3D11DeviceContext* deviceContext)
	{
		deviceContext->PSSetSamplers(0, 1, _linearWrapSampler.GetAddressOf());
		deviceContext->PSSetSamplers(1, 1, _linearBorderSampler.GetAddressOf());
		deviceContext->PSSetSamplers(2, 1, _pointClampSampler.GetAddressOf());
		deviceContext->PSSetSamplers(3, 1, _comparisonSampler.GetAddressOf());
	}
}
