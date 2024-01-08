#pragma once
#include "DX11Define.h"
#include "Vertex.h"
#include "Mesh.h"
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include <string>
#include <vector>

struct Texture
{
	std::string type;
	std::string fileName;
	ComPtr<ID3D11ShaderResourceView> texture;
};

struct ModelData
{
	std::string directory;
	std::string fileName;
	std::vector<::Mesh*> meshes;
	std::vector<Texture> materialTextures;
};

class ModelLoader
{
public:
	ModelLoader() = delete;
	ModelLoader(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	~ModelLoader();

	ModelData Load(std::string fileName);

private:
	void ProcessNode(aiNode* node, const aiScene* scene, ModelData& outputData);
	void ProcessMesh(aiMesh* mesh, const aiScene* scene, std::vector<VertexStruct::Vertex>& vertices, std::vector<UINT>& indices, ModelData& outputData);
	void ProcessMesh(aiMesh* mesh, const aiScene* scene, std::vector<VertexStruct::VertexSkinning>& vertices, std::vector<UINT>& indices, ModelData& outputData);
	void LoadMaterialTextures(aiMaterial* material, const aiScene* scene, ModelData& outputData);
	ID3D11ShaderResourceView* LoadEmbeddedTexture(const aiTexture* embeddedTexture);


private:
	ComPtr<ID3D11Device> m_device;
	ComPtr<ID3D11DeviceContext> m_deviceContext;
	std::vector<Texture> loadedTextures;
};

