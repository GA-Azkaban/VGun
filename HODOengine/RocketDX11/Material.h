#pragma once
#include <d3d11.h>
#include <wrl.h>
#include <string>
#include <vector>
#include <DirectXMath.h>
#include "../HODO3DGraphicsInterface/IMaterial.h"
using namespace Microsoft::WRL;

namespace RocketCore::Graphics
{
	class VertexShader;
	class PixelShader;

	class Material : public HDEngine::IMaterial
	{
	public:
		friend class ObjectManager;
		virtual ~Material();

		virtual void SetMaterialName(const std::string& materialName);
		virtual void SetColor(UINT r, UINT g, UINT b, UINT a);
		virtual void LoadAlbedoTexture(const std::string& fileName);
		virtual void LoadNormalTexture(const std::string& fileName);
		virtual void LoadARMTexture(const std::string& fileName);
		virtual void LoadMetallicTexture(const std::string& fileName);
		virtual void LoadRoughnessTexture(const std::string& fileName);
		virtual void SetMetallicValue(float value);
		virtual void SetRoughnessValue(float value);

		virtual const std::string& GetMaterialName() const;
		virtual const DirectX::XMINT4& GetColor() const;
		virtual const std::string& GetAlbedoTextureName() const;
		virtual const std::string& GetNormalTextureName() const;
		virtual const std::string& GetARMTextureName() const;
		virtual const std::string& GetMetallicTextureName() const;
		virtual const std::string& GetRoughnessTextureName() const;
		virtual float GetMetallicValue() const;
		virtual float GetRoughnessValue() const;

		const DirectX::XMFLOAT4& GetColorFloat4() const;

		ID3D11ShaderResourceView* GetAlbedoMap();
		ID3D11ShaderResourceView* GetNormalMap();
		ID3D11ShaderResourceView* GetRoughnessMap();
		ID3D11ShaderResourceView* GetMetallicMap();
		ID3D11ShaderResourceView* GetOcclusionRoughnessMetalMap();

		void SetAlbedoMap(ID3D11ShaderResourceView* srv, const std::string& fileName);
		void SetNormalMap(ID3D11ShaderResourceView* srv, const std::string& fileName);
		void SetOcclusionRoughnessMetalMap(ID3D11ShaderResourceView* srv, const std::string& fileName);
		void SetRoughnessMap(ID3D11ShaderResourceView* srv, const std::string& fileName);
		void SetMetallicMap(ID3D11ShaderResourceView* srv, const std::string& fileName);

		VertexShader* GetVertexShader() const;
		PixelShader* GetPixelShader() const;

		void SetVertexShader(VertexShader* vertexShader);
		void SetPixelShader(PixelShader* pixelShader);

	private:
		Material(const HDEngine::MaterialDesc& materialDesc);

	private:
		std::string _materialName;
		DirectX::XMINT4 _color;
		DirectX::XMFLOAT4 _colorFloat4;
		std::string _albedo;
		std::string _normalMap;
		std::string _occlusionRoughMatel;
		std::string _metallic;
		std::string _roughness;
		float _metallicValue;
		float _roughnessValue;

		VertexShader* _vertexShader;
		PixelShader* _pixelShader;

		ComPtr<ID3D11ShaderResourceView> _materialAlbedo;
		ComPtr<ID3D11ShaderResourceView> _materialNormal;
		ComPtr<ID3D11ShaderResourceView> _materialARM;
		ComPtr<ID3D11ShaderResourceView> _materialMetallic;
		ComPtr<ID3D11ShaderResourceView> _materialRoughness;
	};
}

