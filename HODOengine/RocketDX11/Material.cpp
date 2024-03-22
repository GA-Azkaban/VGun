#include "Material.h"

namespace RocketCore::Graphics
{
	Material::Material(VertexShader* vertexShader, PixelShader* pixelShader)
		: m_vertexShader(vertexShader), m_pixelShader(pixelShader),
		m_materialAlbedo(0), m_materialNormal(0), m_occlusionRoughnessMetal(0),
		m_materialMetallic(0), m_materialRoughness(0),
		m_metallic(0.1f), m_roughness(0.4f)
	{

	}

	Material::~Material()
	{
		m_vertexShader = nullptr;
		m_pixelShader = nullptr;
		delete m_vertexShader;
		delete m_pixelShader;
		m_materialAlbedo.Reset();
		m_materialNormal.Reset();
	}

	ID3D11ShaderResourceView* Material::GetAlbedoMap()
	{
		return m_materialAlbedo.Get();
	}

	ID3D11ShaderResourceView* Material::GetNormalMap()
	{
		return m_materialNormal.Get();
	}

	ID3D11ShaderResourceView* Material::GetRoughnessMap()
	{
		return m_materialRoughness.Get();
	}

	ID3D11ShaderResourceView* Material::GetMetallicMap()
	{
		return m_materialMetallic.Get();
	}

	ID3D11ShaderResourceView* Material::GetOcclusionRoughnessMetalMap()
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

	void Material::SetMetallic(float v)
	{
		m_metallic = v;
	}

	void Material::SetRoughness(float v)
	{
		m_roughness = v;
	}

}
