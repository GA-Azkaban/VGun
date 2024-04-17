#pragma once
#include <string>
#include "MathHeader.h"

namespace HDEngine
{
	class IMaterial
	{
	public:
		IMaterial() {}
		virtual ~IMaterial() {}
		virtual void SetMaterialName(const std::string& materialName) = 0;
		virtual void SetColor(UINT r, UINT g, UINT b, UINT a) = 0;
		virtual void LoadAlbedoTexture(const std::string& fileName) = 0;
		virtual void LoadNormalTexture(const std::string& fileName) = 0;
		virtual void LoadARMTexture(const std::string& fileName) = 0;
		virtual void LoadMetallicTexture(const std::string& fileName) = 0;
		virtual void LoadRoughnessTexture(const std::string& fileName) = 0;
		virtual void SetMetallicValue(float value) = 0;
		virtual void SetRoughnessValue(float value) = 0;

		virtual const std::string& GetMaterialName() const = 0;
		virtual const DirectX::XMINT4& GetColor() const = 0;
		virtual const std::string& GetAlbedoTextureName() const = 0;
		virtual const std::string& GetNormalTextureName() const = 0;
		virtual const std::string& GetARMTextureName() const = 0;
		virtual const std::string& GetMetallicTextureName() const = 0;
		virtual const std::string& GetRoughnessTextureName() const = 0;
		virtual float GetMetallicValue() const = 0;
		virtual float GetRoughnessValue() const = 0;
	};

	struct MaterialDesc
	{
		std::string materialName = "";
		DirectX::XMINT4 color = { 255, 255, 255, 255 };
		std::string albedo = "";
		std::string normalMap = "";
		std::string occlusionRoughMatel = "";
		std::string metallic = "";
		std::string roughness = "";
		float metallicValue = 0.0f;
		float roughnessValue = 0.0f;
	};
}
