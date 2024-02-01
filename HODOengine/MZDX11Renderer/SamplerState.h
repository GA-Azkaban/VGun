#pragma once
#include "DX11Define.h"
#include "LazyObjects.h"

class SamplerState
{
public:
	SamplerState();

	void CreateSamplerStates(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	void DestroySamplerStates();

private:
	void SetSamplers(ID3D11DeviceContext* deviceContext);

private:
	ComPtr<ID3D11SamplerState> _linearSampler;
	ComPtr<ID3D11SamplerState> _pointSampler;
};

