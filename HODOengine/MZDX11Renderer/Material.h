#pragma once
#include "DX11Define.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include <string>
#include <vector>

//struct MZTexture
//{
//	std::string type;
//	std::string path;
//	ComPtr<ID3D11ShaderResourceView> texture;
//};

class VertexShader;
class PixelShader;

class Material
{
public:
	Material(VertexShader* vertexShader, PixelShader* pixelShader);
	//Material(VertexShader* vertexShader, PixelShader* pixelShader, ID3D11ShaderResourceView* materialSRV, ID3D11ShaderResourceView* materialNormal, ID3D11SamplerState* materialSampler);
	//Material(VertexShader* vertexShader, PixelShader* pixelShader, std::vector<MZTexture> textures, ID3D11SamplerState* materialSampler);
	~Material();
	ID3D11ShaderResourceView* GetTextureSRV();
	ID3D11ShaderResourceView* GetNormalMapSRV();
	ID3D11SamplerState* GetSamplerState();
	VertexShader* GetVertexShader();
	PixelShader* GetPixelShader();

	void SetVertexShader(VertexShader* vertexShader) { m_vertexShader = vertexShader; };
	void SetPixelShader(PixelShader* pixelShader) { m_pixelShader = pixelShader; };
	void SetTextureSRV(ID3D11ShaderResourceView* textureSRV) { m_materialSRV = textureSRV; }
	void SetNormalTexture(ID3D11ShaderResourceView* normalTex) { m_materialNormal = normalTex; }
	void SetSamplerState(ID3D11SamplerState* sampler) { m_materialSampler = sampler; }

private:
	VertexShader* m_vertexShader;
	PixelShader* m_pixelShader;
	ComPtr<ID3D11ShaderResourceView> m_materialSRV;
	ComPtr<ID3D11ShaderResourceView> m_materialNormal;
	//std::vector<MZTexture> m_textures;
	ComPtr<ID3D11SamplerState> m_materialSampler;
};

