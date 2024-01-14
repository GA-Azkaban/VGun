#pragma once
#include "DX11Define.h"
#include "LazyObjects.h"
#include "Vertex.h"
#include "Animation.h"
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include <string>
#include <vector>
#include <unordered_map>

class GeometryGenerator;
class Mesh;

struct FileInfo
{
	std::vector<Mesh*> loadedMeshes;
	Node* node;
	std::unordered_map<std::string, Animation*> loadedAnimation;	// <animName, animationInfo>
};

class ResourceManager
{
public:
	static LazyObjects<ResourceManager> Instance;
	friend LazyObjects<ResourceManager>;

    ~ResourceManager();

    void Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext);

	void LoadFBXFile(std::string fileName);
	void LoadTextureFile(std::string fileName);

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

private:
	ResourceManager();

	void CreatePrimitiveMeshes();

	void ProcessNode(aiNode* node, const aiScene* scene);
	void ProcessMesh(aiMesh* mesh, const aiScene* scene);
	void ProcessStaticMesh(aiMesh* mesh, const aiScene* scene);
	void ProcessSkinningMesh(aiMesh* mesh, const aiScene* scene);
	void LoadMaterialTextures(aiMaterial* material, aiTextureType type, const aiScene* scene);
	ID3D11ShaderResourceView* LoadEmbeddedTexture(const aiTexture* embeddedTexture);
	void ReadNodeHierarchy(Node& nodeOutput, aiNode* node, std::unordered_map<std::string, std::pair<int, DirectX::XMMATRIX>>& boneInfo);
	void LoadAnimation(const aiScene* scene);

private:
	ComPtr<ID3D11Device> m_device;
	ComPtr<ID3D11DeviceContext> m_deviceContext;
	std::unordered_map<std::string, FileInfo> m_loadedFileInfo; //<fileName, infos>
	std::unordered_map<std::string, ID3D11ShaderResourceView*> m_loadedTextures;	//<fileName, texture>
	
	std::string m_fileName;

	GeometryGenerator* m_geometryGen;
};

