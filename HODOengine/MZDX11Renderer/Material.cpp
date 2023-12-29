#include "Material.h"

Material::Material(VertexShader* vertexShader, PixelShader* pixelShader, ID3D11ShaderResourceView* materialSRV, ID3D11ShaderResourceView* materialNormal, ID3D11SamplerState* materialSampler)
	: m_vertexShader(vertexShader), m_pixelShader(pixelShader), m_materialSRV(materialSRV), m_materialNormal(materialNormal), m_materialSampler(materialSampler)
{

}

Material::~Material()
{

}

ID3D11ShaderResourceView* Material::GetTextureSRV()
{
	return m_materialSRV.Get();
}

ID3D11ShaderResourceView* Material::GetNormalMapSRV()
{
	return m_materialNormal.Get();
}

ID3D11SamplerState* Material::GetSamplerState()
{
	return m_materialSampler.Get();
}

VertexShader* Material::GetVertexShader()
{
	return m_vertexShader;
}

PixelShader* Material::GetPixelShader()
{
	return m_pixelShader;
}
