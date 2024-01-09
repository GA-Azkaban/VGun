#pragma once
#include "DX11Define.h"
#include "LazyObjects.h"

class SamplerState
{
public:
	static LazyObjects<SamplerState> Instance;
	friend LazyObjects<SamplerState>;

	void CreateSamplerStates(ID3D11Device* device);
	void DestroySamplerStates();

	ID3D11SamplerState* GetSamplerState() { return m_textureSampler.Get(); }

private:
	SamplerState();

	ComPtr<ID3D11SamplerState> m_textureSampler;
};

