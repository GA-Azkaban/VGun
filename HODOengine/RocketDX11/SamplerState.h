#pragma once
#include <d3d11_2.h>
#include <wrl.h>

namespace RocketCore::Graphics
{
	class SamplerState
	{
	public:
		SamplerState() = default;

		void CreateSamplerStates(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
		void DestroySamplerStates();

	private:
		void SetSamplers(ID3D11DeviceContext* deviceContext);

	private:
		Microsoft::WRL::ComPtr<ID3D11SamplerState> _linearWrapSampler;
		Microsoft::WRL::ComPtr<ID3D11SamplerState> _linearBorderSampler;
		Microsoft::WRL::ComPtr<ID3D11SamplerState> _pointClampSampler;
		Microsoft::WRL::ComPtr<ID3D11SamplerState> _comparisonSampler;
	};
}
