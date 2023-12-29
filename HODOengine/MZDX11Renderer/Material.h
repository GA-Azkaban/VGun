#pragma once
#include "DX11Define.h"
#include "VertexShader.h"
#include "PixelShader.h"

class VertexShader;
class PixelShader;

class Material
{
public:
	Material(VertexShader* vertexShader, PixelShader* pixelShader, ID3D11ShaderResourceView* materialSRV, ID3D11ShaderResourceView* materialNormal, ID3D11SamplerState* materialSampler);
	~Material();
	ID3D11ShaderResourceView* GetTextureSRV();
	ID3D11ShaderResourceView* GetNormalMapSRV();
	ID3D11SamplerState* GetSamplerState();
	VertexShader* GetVertexShader();
	PixelShader* GetPixelShader();

private:
	VertexShader* m_vertexShader;
	PixelShader* m_pixelShader;
	ComPtr<ID3D11ShaderResourceView> m_materialSRV;
	ComPtr<ID3D11ShaderResourceView> m_materialNormal;
	ComPtr<ID3D11SamplerState> m_materialSampler;
};

