#include "Material.h"

Material::Material(VertexShader* vertexShader, PixelShader* pixelShader)
	: m_vertexShader(vertexShader), m_pixelShader(pixelShader),
	m_materialSRV(0), m_materialNormal(0), m_materialSampler(0)
{

}

//Material::Material(VertexShader* vertexShader, PixelShader* pixelShader, ID3D11ShaderResourceView* materialSRV, ID3D11ShaderResourceView* materialNormal, ID3D11SamplerState* materialSampler)
//	: m_vertexShader(vertexShader), m_pixelShader(pixelShader), m_materialSRV(materialSRV), m_materialNormal(materialNormal), m_materialSampler(materialSampler)
//{
//
//}

Material::~Material()
{
	delete m_vertexShader;
	delete m_pixelShader;
	m_materialSRV.Reset();
	m_materialNormal.Reset();
	m_materialSampler.Reset();
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
