#pragma once
#include "DX11Define.h"
#include "LazyObjects.h"
#include "Vertex.h"
#include "Node.h"
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include <string>
#include <vector>
#include <unordered_map>

class GeometryGenerator;
class Mesh;
class Bone;

class ResourceManager
{
public:
	static LazyObjects<ResourceManager> Instance;
	friend LazyObjects<ResourceManager>;

    ~ResourceManager();

    void Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext);

	void LoadFBXFile(std::string filePath);
	void LoadTextureFile(std::string filePath);

	std::vector<Mesh*>& GetLoadedMesh(const std::string& fileName);
	ID3D11ShaderResourceView* GetLoadedTexture(const std::string& fileName);
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
	ID3D11ShaderResourceView* LoadTexture(const std::string& fileName);
	void ReadNodeHierarchy(Node& nodeOutput, aiNode* node, std::unordered_map<std::string, std::pair<int, DirectX::XMMATRIX>>& boneInfo);
	void LoadAnimation(const aiScene* scene);

private:
	ComPtr<ID3D11Device> m_device;
	ComPtr<ID3D11DeviceContext> m_deviceContext;
	std::unordered_map<std::string, std::vector<Mesh*>> m_loadedMeshes;
	std::unordered_map<std::string, ID3D11ShaderResourceView*> m_loadedTextures;
	std::unordered_map<std::string, std::unordered_map<std::string, Animation*>> m_loadedAnimation;	//<fileName, <animName, Animation>>
	
	std::string m_directory;
	std::string m_fileName;

	GeometryGenerator* m_geometryGen;
};

