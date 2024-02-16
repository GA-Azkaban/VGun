﻿#pragma once
#include <d3d11.h>
#include "VertexShader.h"
#include "PixelShader.h"
#include <string>
#include <vector>
using namespace Microsoft::WRL;

class VertexShader;
class PixelShader;

namespace RocketCore::Graphics
{
	class Material
	{
	public:
		Material(VertexShader* vertexShader, PixelShader* pixelShader);
		~Material();
		ID3D11ShaderResourceView* GetAlbedoMap();
		ID3D11ShaderResourceView* GetNormalMap();
		ID3D11ShaderResourceView* GetOcclusionRoughnessMetalMap();
		float GetMetallic();
		float GetRoughness();
		VertexShader* GetVertexShader();
		PixelShader* GetPixelShader();

		void SetVertexShader(VertexShader* vertexShader) { m_vertexShader = vertexShader; };
		void SetPixelShader(PixelShader* pixelShader) { m_pixelShader = pixelShader; };
		void SetAlbedoMap(ID3D11ShaderResourceView* albedoTex) { m_materialAlbedo = albedoTex; }
		void SetNormalMap(ID3D11ShaderResourceView* normalTex) { m_materialNormal = normalTex; }
		void SetOcclusionRoughnessMetalMap(ID3D11ShaderResourceView* occlusionRoughMetal) { m_occlusionRoughnessMetal = occlusionRoughMetal; }
		void SetMetallic(float n);
		void SetRoughness(float n);

	private:
		VertexShader* m_vertexShader;
		PixelShader* m_pixelShader;
		ComPtr<ID3D11ShaderResourceView> m_materialAlbedo;
		ComPtr<ID3D11ShaderResourceView> m_materialNormal;
		ComPtr<ID3D11ShaderResourceView> m_occlusionRoughnessMetal;

		float m_metallic;
		float m_roughness;
	};
}

