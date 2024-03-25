﻿#pragma once
#include <d3d11_2.h>
#include <dxgi1_3.h>
#include <wrl.h>
#include <unordered_map>
#include <DXTK/SpriteFont.h>
#include <string>
#include <vector>
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include <GeometricPrimitive.h>
#include <memory>

#include "Singleton.h"
#include "Animation.h"

using Microsoft::WRL::ComPtr;

namespace RocketCore::Graphics
{
	class Mesh;
	class Material;
	class CubeMesh;
	class VertexShader;
	class PixelShader;
	class ImageRenderer;
	class GeometryGenerator;
	class SamplerState;

	struct FileInfo
	{
		std::vector<Mesh*> loadedMeshes;
		std::vector<Material*> loadedMaterials;
		Node* node;
		std::unordered_map<std::string, Animation*> loadedAnimation;	// <animName, animationInfo>
	};

	struct Texture
	{
		ComPtr<ID3D11Texture2D> texture;
		std::vector<ComPtr<ID3D11RenderTargetView>> renderTargetViews;
		ComPtr<ID3D11ShaderResourceView> shaderResourceView;
	};

	struct EnvMapInfo
	{
		Texture cubeMapTexture;
		Texture envMapTexture;
		Texture envPreFilterMapTexture;
		Texture brdfLUT;
	};

	class ResourceManager : public Singleton<ResourceManager>
	{
		friend Singleton;
	private:
		ResourceManager();

	public:
		enum class eRasterizerState
		{
			SOLID,
			WIREFRAME,
			SHADOWMAP,
			CUBEMAP,
		};

	public:
		void Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext);

		void LoadFBXFile(std::string fileName);
		void LoadTextureFile(std::string fileName);
		void LoadCubeMapTextureFile(std::string fileName);

		/// <summary>
	/// Get meshes informations in model file.
	/// </summary>
	/// <param name="fileName">model file name included extension</param>
	/// ex) model1.fbx
	/// <returns></returns>
		std::vector<Mesh*>& GetMeshes(const std::string& fileName);

		/// <summary>
		/// A method for get TEXTURE
		/// </summary>
		/// <param name="fileName">texture file name included extension</param>
		/// ex) texture1.dds, texture2.png
		/// <returns></returns>
		ID3D11ShaderResourceView* GetTexture(const std::string& fileName);

		/// <summary>
		/// Get node informations in model file.
		/// </summary>
		/// <param name="fileName">model file name include extension</param>
		/// ex) model1.fbx
		/// <returns></returns>
		Node* GetNode(const std::string& fileName);

		/// <summary>
		/// Get animation informations in model file.
		/// </summary>
		/// <param name="fileName">model file name included extension</param>
		/// ex)model1.fbx
		/// <returns>animation name and animation infos of all animations in model file</returns>
		std::unordered_map<std::string, Animation*>& GetAnimations(const std::string& fileName);

		VertexShader* GetVertexShader(const std::string& name);
		PixelShader* GetPixelShader(const std::string& name);

		EnvMapInfo& GetEnvMapInfo(const std::string& fileName);

		DirectX::SpriteFont* GetDefaultFont();

	public:
		ID3D11Device* GetDevice();
		ID3D11DeviceContext* GetDeviceContext();
		ID3D11RasterizerState* GetRasterizerState(eRasterizerState eState);

	public:
		DirectX::DX11::GeometricPrimitive* GetCubePrimitive();
		DirectX::DX11::GeometricPrimitive* GetSpherePrimitive();
		DirectX::DX11::GeometricPrimitive* GetCylinderPrimitive();
		DirectX::DX11::GeometricPrimitive* GetCapsulePrimitive();

	private:
		void LoadShaders();
		void CreateRasterizerStates();
		void CreateSamplerStates();
		void CreatePrimitiveMeshes();

		void ProcessNode(aiNode* node, const aiScene* scene);
		void ProcessMesh(aiMesh* mesh, const aiScene* scene);
		void ProcessStaticMesh(aiMesh* mesh, const aiScene* scene);
		void ProcessSkinningMesh(aiMesh* mesh, const aiScene* scene);
		void LoadMaterialTextures(aiMaterial* material, aiTextureType type, const aiScene* scene);
		ID3D11ShaderResourceView* LoadEmbeddedTexture(const aiTexture* embeddedTexture);
		void ReadNodeHierarchy(Node& nodeOutput, aiNode* node, std::unordered_map<std::string, std::pair<int, DirectX::XMMATRIX>>& boneInfo);
		void LoadAnimation(const aiScene* scene);

		void GenerateEnvMap(Texture& envMapTexture, ID3D11ShaderResourceView* cubeMapSRV);
		void GenerateEnvPreFilter(Texture& envPreFilterMap, ID3D11ShaderResourceView* cubeMapSRV);
		void GenerateBrdfLUT(Texture& brdfLUT);

	private:
		ComPtr<ID3D11Device> _device;
		ComPtr<ID3D11DeviceContext> _deviceContext;

		// primitive models
		std::unique_ptr<DirectX::DX11::GeometricPrimitive> _cubePrimitive;
		std::unique_ptr<DirectX::DX11::GeometricPrimitive> _spherePrimitive;
		std::unique_ptr<DirectX::DX11::GeometricPrimitive> _cylinderPrimitive;
		std::unique_ptr<DirectX::DX11::GeometricPrimitive> _capsulePrimitive;

		// 기본 폰트 들고있음
		DirectX::SpriteFont* _defaultFont;

		std::unordered_map<std::string, VertexShader*> _vertexShaders;
		std::unordered_map<std::string, PixelShader*> _pixelShaders;
		std::vector<ID3D11RasterizerState*> _rasterizerStates;

		std::unordered_map<std::string, FileInfo> _loadedFileInfo; //<fileName, infos>
		std::unordered_map<std::string, ID3D11ShaderResourceView*> _loadedTextures;	//<fileName, texture>
		std::unordered_map<std::string, EnvMapInfo> _loadedEnvMaps;

		std::string _fileName;

		GeometryGenerator* _geometryGen;
		SamplerState* _samplerState;

		const float cubeMapSize = 512.0f;
	};
}
