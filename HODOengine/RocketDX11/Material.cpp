﻿#include "Material.h"
#include "ResourceManager.h"
#include "VertexShader.h"
#include "PixelShader.h"

namespace RocketCore::Graphics
{

	Material::Material(const HDEngine::MaterialDesc& materialDesc)
		: _materialName(materialDesc.materialName),
		_color{ materialDesc.color.x,
				materialDesc.color.y,
				materialDesc.color.z,
				materialDesc.color.w },
		_colorFloat4{ static_cast<float>(materialDesc.color.x) / 255.0f,
					  static_cast<float>(materialDesc.color.y) / 255.0f,
					  static_cast<float>(materialDesc.color.z) / 255.0f,
					  static_cast<float>(materialDesc.color.w) / 255.0f },
		_albedo(materialDesc.albedo),
		_normalMap(materialDesc.normalMap),
		_occlusionRoughMatel(materialDesc.occlusionRoughMatel),
		_metallic(materialDesc.metallic),
		_roughness(materialDesc.roughness),
		_mask(materialDesc.mask),
		_metallicValue(materialDesc.metallicValue),
		_roughnessValue(materialDesc.roughnessValue),
		_vertexShader(ResourceManager::Instance().GetVertexShader("VertexShader.cso")),
		_pixelShader(ResourceManager::Instance().GetPixelShader("PixelShader.cso")),
		_materialAlbedo(0), _materialNormal(0), _materialARM(0),
		_materialMetallic(0), _materialRoughness(0)
	{
		LoadAlbedoTexture(_albedo);
		LoadNormalTexture(_normalMap);
		LoadARMTexture(_occlusionRoughMatel);
		LoadMetallicTexture(_metallic);
		LoadRoughnessTexture(_roughness);
		LoadMaskTexture(_mask);
	}

	Material::~Material()
	{
		delete _vertexShader;
		_vertexShader = nullptr;
		delete _pixelShader;
		_pixelShader = nullptr;

		_materialAlbedo.Reset();
		_materialNormal.Reset();
		_materialARM.Reset();
		_materialMetallic.Reset();
		_materialRoughness.Reset();
	}

	void Material::SetMaterialName(const std::string& materialName)
	{
		std::unordered_map<std::string, Material*>& materialList = ResourceManager::Instance().GetLoadedMaterials();

		std::string matName = materialName;
		if (materialName == "")
		{
			matName = "NewMaterial";
		}

		// 이미 있는 이름이라면 뒤에 숫자를 붙혀준다.
		if (materialList.find(matName) != materialList.end())
		{
			std::string tempName = matName;
			for (UINT i = 1; ++i;)
			{
				matName = tempName + std::to_string(i);
				if (materialList.find(matName) == materialList.end())
				{
					break;
				}
			}
		}

		materialList.erase(_materialName);
		materialList.insert(std::make_pair(matName, this));

		_materialName = matName;
	}

	void Material::SetColor(UINT r, UINT g, UINT b, UINT a)
	{
		_color.x = r;
		_color.y = g;
		_color.z = b;
		_color.w = a;

		_colorFloat4.x = static_cast<float>(r) / 255.0f;
		_colorFloat4.y = static_cast<float>(g) / 255.0f;
		_colorFloat4.z = static_cast<float>(b) / 255.0f;
		_colorFloat4.w = static_cast<float>(a) / 255.0f;
	}

	void Material::LoadAlbedoTexture(const std::string& fileName)
	{
		if (fileName == "")
			return;

		ID3D11ShaderResourceView* albedo = ResourceManager::Instance().GetTexture(fileName);
		if (!albedo)
			return;
		_albedo = fileName;
		_materialAlbedo = albedo;
	}

	void Material::LoadNormalTexture(const std::string& fileName)
	{
		if (fileName == "")
			return;

		ID3D11ShaderResourceView* normal = ResourceManager::Instance().GetTexture(fileName);
		if (!normal)
			return;
		_normalMap = fileName;
		_materialNormal = normal;
	}

	void Material::LoadARMTexture(const std::string& fileName)
	{
		if (fileName == "")
			return;

		ID3D11ShaderResourceView* arm = ResourceManager::Instance().GetTexture(fileName);
		if (!arm)
			return;
		_occlusionRoughMatel = fileName;
		_materialARM = arm;
	}

	void Material::LoadMetallicTexture(const std::string& fileName)
	{
		if (fileName == "")
			return;

		ID3D11ShaderResourceView* metallic = ResourceManager::Instance().GetTexture(fileName);
		if (!metallic)
			return;
		_metallic = fileName;
		_materialMetallic = metallic;
	}

	void Material::LoadRoughnessTexture(const std::string& fileName)
	{
		if (fileName == "")
			return;

		ID3D11ShaderResourceView* roughness = ResourceManager::Instance().GetTexture(fileName);
		if (!roughness)
			return;
		_roughness = fileName;
		_materialRoughness = roughness;
	}

	void Material::LoadMaskTexture(const std::string& fileName)
	{
		if (fileName == "")
			return;

		ID3D11ShaderResourceView* mask = ResourceManager::Instance().GetTexture(fileName);
		if (!mask)
			return;
		_mask = fileName;
		_materialMask = mask;
	}

	void Material::SetMetallicValue(float value)
	{
		_metallicValue = value;
	}

	void Material::SetRoughnessValue(float value)
	{
		_roughnessValue = value;
	}

	const std::string& Material::GetMaterialName() const
	{
		return _materialName;
	}

	const DirectX::XMINT4& Material::GetColor() const
	{
		return _color;
	}

	const DirectX::XMFLOAT4& Material::GetColorFloat4() const
	{
		return _colorFloat4;
	}

	const std::string& Material::GetAlbedoTextureName() const
	{
		return _albedo;
	}

	const std::string& Material::GetNormalTextureName() const
	{
		return _normalMap;
	}

	const std::string& Material::GetARMTextureName() const
	{
		return _occlusionRoughMatel;
	}

	const std::string& Material::GetMetallicTextureName() const
	{
		return _metallic;
	}

	const std::string& Material::GetRoughnessTextureName() const
	{
		return _roughness;
	}

	const std::string& Material::GetMaskTextureName() const
	{
		return _mask;
	}

	float Material::GetMetallicValue() const
	{
		return _metallicValue;
	}

	float Material::GetRoughnessValue() const
	{
		return _roughnessValue;
	}

	ID3D11ShaderResourceView* Material::GetAlbedoMap()
	{
		return _materialAlbedo.Get();
	}

	ID3D11ShaderResourceView* Material::GetNormalMap()
	{
		return _materialNormal.Get();
	}

	ID3D11ShaderResourceView* Material::GetRoughnessMap()
	{
		return _materialRoughness.Get();
	}

	ID3D11ShaderResourceView* Material::GetMetallicMap()
	{
		return _materialMetallic.Get();
	}

	ID3D11ShaderResourceView* Material::GetOcclusionRoughnessMetalMap()
	{
		return _materialARM.Get();
	}

	ID3D11ShaderResourceView* Material::GetMaskMap()
	{
		return _materialMask.Get();
	}

	void Material::SetAlbedoMap(ID3D11ShaderResourceView* srv, const std::string& fileName)
	{
		_materialAlbedo = srv;
		_albedo = fileName;
	}

	void Material::SetNormalMap(ID3D11ShaderResourceView* srv, const std::string& fileName)
	{
		_materialNormal = srv;
		_normalMap = fileName;
	}

	void Material::SetOcclusionRoughnessMetalMap(ID3D11ShaderResourceView* srv, const std::string& fileName)
	{
		_materialARM = srv;
		_occlusionRoughMatel = fileName;
	}

	void Material::SetRoughnessMap(ID3D11ShaderResourceView* srv, const std::string& fileName)
	{
		_materialRoughness = srv;
		_roughness = fileName;
	}

	void Material::SetMetallicMap(ID3D11ShaderResourceView* srv, const std::string& fileName)
	{
		_materialMetallic = srv;
		_metallic = fileName;
	}

	void Material::SetMaskMap(ID3D11ShaderResourceView* srv, const std::string& fileName)
	{
		_materialMask = srv;
		_mask = fileName;
	}

	VertexShader* Material::GetVertexShader() const
	{
		return _vertexShader;
	}

	PixelShader* Material::GetPixelShader() const
	{
		return _pixelShader;
	}

	void Material::SetVertexShader(VertexShader* vertexShader)
	{
		_vertexShader = vertexShader;
	}

	void Material::SetPixelShader(PixelShader* pixelShader)
	{
		_pixelShader = pixelShader;
	}

}
