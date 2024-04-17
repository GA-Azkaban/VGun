﻿#pragma once
#include "dllExporter.h"
#include "../HODO3DGraphicsInterface/IMaterial.h"

namespace HDEngine
{
	class MaterialManager;
}

namespace HDData
{
	class HODO_API Material
	{
	public:
		friend HDEngine::MaterialManager;
		friend class MeshRenderer;
		friend class SkinnedMeshRenderer;
		Material() = delete;
		~Material();
		void SetMaterialName(const std::string& materialName);
		void SetColor(UINT r, UINT g, UINT b, UINT a);
		void LoadAlbedoTexture(const std::string& fileName);
		void LoadNormalTexture(const std::string& fileName);
		void LoadARMTexture(const std::string& fileName);
		void LoadMetallicTexture(const std::string& fileName);
		void LoadRoughnessTexture(const std::string& fileName);
		void SetMetallicValue(float value);
		void SetRoughnessValue(float value);

		const std::string& GetMaterialName() const;
		const DirectX::XMFLOAT4& GetColor() const;
		const std::string& GetAlbedoTextureName() const;
		const std::string& GetNormalTextureName() const;
		const std::string& GetARMTextureName() const;
		const std::string& GetMetallicTextureName() const;
		const std::string& GetRoughnessTextureName() const;
		float GetMetallicValue() const;
		float GetRoughnessValue() const;

	private:
		Material(const HDEngine::MaterialDesc& desc);

	private:
		HDEngine::IMaterial* _material;
	};
}
