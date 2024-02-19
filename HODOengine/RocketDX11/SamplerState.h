#pragma once
#include <d3d11_2.h>
#include <wrl.h>

namespace RocketCore::Graphics
{
	class SamplerState
	{
	public:
		SamplerState();

		void CreateSamplerStates(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
		void DestroySamplerStates();

	private:
		void SetSamplers(ID3D11DeviceContext* deviceContext);

	private:
		Microsoft::WRL::ComPtr<ID3D11SamplerState> _linearSampler;
		Microsoft::WRL::ComPtr<ID3D11SamplerState> _pointSampler;
		Microsoft::WRL::ComPtr<ID3D11SamplerState> _shadowSampler;
	};
}
