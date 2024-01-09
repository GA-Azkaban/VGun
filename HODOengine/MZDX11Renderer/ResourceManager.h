#pragma once
#include "DX11Define.h"
#include "LazyObjects.h"
#include "Vertex.h"
#include "Mesh.h"
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include <string>
#include <vector>
#include <unordered_map>

struct Texture
{
	std::string type;
	ComPtr<ID3D11ShaderResourceView> texture;

	Texture()
		: type(), texture(nullptr)
	{

	}
};

class ResourceManager
{
public:
	static LazyObjects<ResourceManager> Instance;
	friend LazyObjects<ResourceManager>;

    ~ResourceManager();

    void Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext);

	void LoadFile(std::string fileName);

	std::vector<::Mesh*> GetLoadedMesh(const std::string& fileName);
	Texture GetLoadedTexture(const std::string& fileName);

private:
	ResourceManager();

	void ProcessNode(aiNode* node, const aiScene* scene);
	void ProcessMesh(aiMesh* mesh, const aiScene* scene, std::vector<VertexStruct::Vertex>& vertices, std::vector<UINT>& indices);
	void ProcessMesh(aiMesh* mesh, const aiScene* scene, std::vector<VertexStruct::VertexSkinning>& vertices, std::vector<UINT>& indices);
	void LoadMaterialTextures(aiMaterial* material, aiTextureType type, const aiScene* scene);
	ID3D11ShaderResourceView* LoadEmbeddedTexture(const aiTexture* embeddedTexture);

private:
	ComPtr<ID3D11Device> m_device;
	ComPtr<ID3D11DeviceContext> m_deviceContext;
	std::unordered_map<std::string, std::vector<::Mesh*>> m_loadedMeshes;
	std::unordered_map<std::string, Texture> m_loadedTextures;

	std::string m_directory;
	std::string m_fileName;
};

