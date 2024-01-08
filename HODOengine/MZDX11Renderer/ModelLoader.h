#pragma once
#include "DX11Define.h"
#include "Vertex.h"
#include "ModelData.h"
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include <string>
#include <vector>

class ModelLoader
{
public:
	ModelLoader() = delete;
	ModelLoader(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	~ModelLoader();

	void Load(std::string fileName);

private:
	void ProcessNode(aiNode* node, const aiScene* scene, std::vector<::Mesh*>& outputMeshes);
	void ProcessMesh(aiMesh* mesh, std::vector<VertexStruct::Vertex>& vertices, std::vector<UINT>& indices, std::vector<::Mesh*>& outputMeshes);
	void ProcessMesh(aiMesh* mesh, std::vector<VertexStruct::VertexSkinning>& vertices, std::vector<UINT>& indices, std::vector<::Mesh*>& outputMeshes);


private:
	ComPtr<ID3D11Device> m_device;
	ComPtr<ID3D11DeviceContext> m_deviceContext;
	std::string directory;
};

