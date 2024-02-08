#include "Material.h"

namespace RocketCore::Graphics
{
	Material::Material(VertexShader* vertexShader, PixelShader* pixelShader)
		: m_vertexShader(vertexShader), m_pixelShader(pixelShader),
		m_materialSRV(0), m_materialNormal(0), m_occlusionRoughnessMetal(0),
		m_metallic(0.0f), m_roughness(1.0f)
	{

	}

	Material::~Material()
	{
		delete m_vertexShader;
		delete m_pixelShader;
		m_materialSRV.Reset();
		m_materialNormal.Reset();
	}

	ID3D11ShaderResourceView* Material::GetTextureSRV()
	{
		return m_materialSRV.Get();
	}

	ID3D11ShaderResourceView* Material::GetNormalMapSRV()
	{
		return m_materialNormal.Get();
	}

	ID3D11ShaderResourceView* Material::GetOcclusionRoughnessMetalMapSRV()
	{
		return m_occlusionRoughnessMetal.Get();
	}

	float Material::GetMetallic()
	{
		return m_metallic;
	}

	float Material::GetRoughness()
	{
		return m_roughness;
	}

	VertexShader* Material::GetVertexShader()
	{
		return m_vertexShader;
	}

	PixelShader* Material::GetPixelShader()
	{
		return m_pixelShader;
	}

}
