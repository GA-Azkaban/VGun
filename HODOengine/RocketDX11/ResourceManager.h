#pragma once
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
#include "BoundingVolumeHelper.h"

#include "../HODO3DGraphicsInterface/IResourceManager.h"
#include "../HODO3DGraphicsInterface/Node.h"

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

	enum class MeshType
	{
		STATIC,
		SKINNING,
	};

	struct FileInfo
	{
		std::vector<Mesh*> loadedMeshes;
		std::vector<Material*> loadedMaterials;
		Node* node;
		DirectX::BoundingBox boundingBox;
		MinMaxElements minMaxElements;
		MeshType meshType;
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

	class ResourceManager : public Singleton<ResourceManager>, public HDEngine::IResourceManager
	{
		friend Singleton;
	private:
		ResourceManager();

	public:
		enum class eRasterizerState
		{
			SOLID,
			CULLNONESOLID,
			WIREFRAME,
			SHADOWMAP,
			CUBEMAP,
		};

	public:
		void Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext);

		void LoadFBXFile(std::string path);
		void LoadTextureFile(std::string path);
		void LoadUITextureFile(std::string path);
		void LoadCubeMapTextureFile(std::string fileName);

		/// <summary>
		/// Get meshes informations in model file.
		/// </summary>
		/// <param name="fileName">model file name included extension</param>
		/// ex) model1.fbx
		/// <returns></returns>
		std::vector<Mesh*>& GetMeshes(const std::string& fileName);

		/// <summary>
		/// Get materials informations in model file.
		/// </summary>
		/// <param name="fileName">model file name included extension</param>
		/// ex) model1.fbx
		/// <returns></returns>
		std::vector<Material*>& GetMaterials(const std::string& fileName);

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
		virtual Node* GetNode(const std::string& fileName);

		/// <summary>
		/// Get animation informations in model file.
		/// </summary>
		/// <param name="fileName">model file name included extension</param>
		/// ex)model1.fbx
		/// <returns>animation name and animation infos of all animations in model file</returns>
		std::unordered_map<std::string, Animation*> GetAnimations(const std::string& fileName);

		DirectX::BoundingBox GetBoundingBox(const std::string& fileName);
		
		/// <summary>
		/// Get loaded material.
		/// </summary>
		/// <param name="materialName">material's name</param>
		/// <returns>pointer of material's interface class</returns>
		Material* GetLoadedMaterial(const std::string& materialName);
		std::unordered_map<std::string, Material*>& GetLoadedMaterials();

		VertexShader* GetVertexShader(const std::string& name);
		PixelShader* GetPixelShader(const std::string& name);

		EnvMapInfo& GetEnvMapInfo(const std::string& fileName);

		DirectX::SpriteFont* GetDefaultFont();
		DirectX::SpriteFont* SetFont(const std::string& str);

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
		void ProcessBoundingBox();
		void LoadMaterialTextures(aiMaterial* material, aiTextureType type, const aiScene* scene, Material* outMaterial);
		ID3D11ShaderResourceView* LoadEmbeddedTexture(const aiTexture* embeddedTexture);
		void ReadNodeHierarchy(Node& nodeOutput, aiNode* node);
		void ReadNodeHierarchy(Node& nodeOutput, aiNode* node, std::unordered_map<std::string, std::pair<int, DirectX::XMMATRIX>>& boneInfo);
		void LoadAnimation(const aiScene* scene, std::string animationName);

		void GenerateEnvMap(Texture& envMapTexture, ID3D11ShaderResourceView* cubeMapSRV);
		void GenerateEnvPreFilter(Texture& envPreFilterMap, ID3D11ShaderResourceView* cubeMapSRV);
		void GenerateBrdfLUT(Texture& brdfLUT);

		std::string GetFileInfoKey(std::string fileName);

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
		std::unordered_map<std::string, Material*> _loadedMaterials; //<materialName, materialInfo>
		std::unordered_map<std::string, std::unordered_map<std::string, Animation*>> _loadedAnimations;	// <animGroup, <animName, animations>>
		std::unordered_map<std::string, ID3D11ShaderResourceView*> _loadedTextureFiles;	//<fileName, texture>
		std::unordered_map<std::string, EnvMapInfo> _loadedEnvMaps;

		std::string _fileName;
		std::string _fileInfoKeyName;

		GeometryGenerator* _geometryGen;
		SamplerState* _samplerState;

		const float cubeMapSize = 512.0f;
	};
}
