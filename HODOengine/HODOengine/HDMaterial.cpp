#include "HDMaterial.h"
#include "GraphicsObjFactory.h"

namespace HDData
{

	Material::Material(const HDEngine::MaterialDesc& desc)
		: _material(HDEngine::GraphicsObjFactory::Instance().GetFactory()->CreateMaterial(desc))
	{

	}

	Material::~Material()
	{
		delete _material;
	}

	void Material::SetMaterialName(const std::string& materialName)
	{
		_material->SetMaterialName(materialName);
	}

	void Material::SetColor(UINT r, UINT g, UINT b, UINT a)
	{
		_material->SetColor(r, g, b, a);
	}

	void Material::LoadAlbedoTexture(const std::string& fileName)
	{
		_material->LoadAlbedoTexture(fileName);
	}

	void Material::LoadNormalTexture(const std::string& fileName)
	{
		_material->LoadNormalTexture(fileName);
	}

	void Material::LoadARMTexture(const std::string& fileName)
	{
		_material->LoadARMTexture(fileName);
	}

	void Material::LoadMetallicTexture(const std::string& fileName)
	{
		_material->LoadMetallicTexture(fileName);
	}

	void Material::LoadRoughnessTexture(const std::string& fileName)
	{
		_material->LoadRoughnessTexture(fileName);
	}

	void Material::SetMetallicValue(float value)
	{
		_material->SetMetallicValue(value);
	}

	void Material::SetRoughnessValue(float value)
	{
		_material->SetRoughnessValue(value);
	}

	const std::string& Material::GetMaterialName() const
	{
		return _material->GetMaterialName();
	}

	const DirectX::XMINT4& Material::GetColor() const
	{
		return _material->GetColor();
	}

	const DirectX::XMFLOAT4& Material::GetColorFloat4() const
	{
		return _material->GetColorFloat4();
	}

	const std::string& Material::GetAlbedoTextureName() const
	{
		return _material->GetAlbedoTextureName();
	}

	const std::string& Material::GetNormalTextureName() const
	{
		return _material->GetNormalTextureName();
	}

	const std::string& Material::GetARMTextureName() const
	{
		return _material->GetARMTextureName();
	}

	const std::string& Material::GetMetallicTextureName() const
	{
		return _material->GetMetallicTextureName();
	}

	const std::string& Material::GetRoughnessTextureName() const
	{
		return _material->GetRoughnessTextureName();
	}

	float Material::GetMetallicValue() const
	{
		return _material->GetMetallicValue();
	}

	float Material::GetRoughnessValue() const
	{
		return _material->GetRoughnessValue();
	}

}
