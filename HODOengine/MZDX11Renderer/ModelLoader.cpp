#include "ModelLoader.h"
#include "Mesh.h"


ModelLoader::ModelLoader(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
	: m_device(device), m_deviceContext(deviceContext)
{

}

ModelLoader::~ModelLoader()
{

}

void ModelLoader::Load(std::string fileName)
{
	Assimp::Importer importer;

	const aiScene* _scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);

	if (_scene == nullptr || _scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || _scene->mRootNode == nullptr)
	{
		MessageBox(NULL, L"Model file couldn't be loaded", L"Error!", MB_ICONERROR | MB_OK);
	}

	std::vector<::Mesh*> meshes;
	ProcessNode(_scene->mRootNode, _scene, meshes);
	if (meshes[0]->GetSingleVertexSize() == sizeof(VertexStruct::Vertex))
	{

	}
	else
	{

	}
}

void ModelLoader::ProcessNode(aiNode* node, const aiScene* scene, std::vector<::Mesh*>& outputMeshes)
{
	for (UINT i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		if (mesh->mNumBones == 0)
		{
			std::vector<VertexStruct::Vertex> vertices;
			std::vector<UINT> indices;
			ProcessMesh(mesh, vertices, indices, outputMeshes);
		}
		else
		{
			std::vector<VertexStruct::Vertex> vertices;
			std::vector<UINT> indices;
			ProcessMesh(mesh, vertices, indices, outputMeshes);
		}
	}

	for (UINT i = 0; i < node->mNumChildren; ++i)
	{
		ProcessNode(node->mChildren[i], scene, outputMeshes);
	}
}

void ModelLoader::ProcessMesh(aiMesh* mesh, std::vector<VertexStruct::Vertex>& vertices, std::vector<UINT>& indices, std::vector<::Mesh*>& outputMeshes)
{
	// Load position, normal, uv
	for (UINT i = 0; i < mesh->mNumVertices; ++i)
	{
		VertexStruct::Vertex vertex;

		// process position
		vertex.Position.x = mesh->mVertices[i].x;
		vertex.Position.y = mesh->mVertices[i].y;
		vertex.Position.z = mesh->mVertices[i].z;

		// process normal
		vertex.Normal.x = mesh->mNormals[i].x;
		vertex.Normal.y = mesh->mNormals[i].y;
		vertex.Normal.z = mesh->mNormals[i].z;

		// process uv
		vertex.UV.x = mesh->mTextureCoords[0][i].x;
		vertex.UV.y = mesh->mTextureCoords[0][i].y;

		vertices.push_back(vertex);
	}

	// Load indices
	for (UINT i = 0; i < mesh->mNumFaces; ++i)
	{
		aiFace& face = mesh->mFaces[i];
		for (UINT j = 0; j < face.mNumIndices; ++j)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	Mesh* newMesh = new Mesh(m_device.Get(), m_deviceContext.Get(), &vertices[0], vertices.size(), &indices[0], indices.size());
	outputMeshes.push_back(newMesh);
}

void ModelLoader::ProcessMesh(aiMesh* mesh, std::vector<VertexStruct::VertexSkinning>& vertices, std::vector<UINT>& indices, std::vector<::Mesh*>& outputMeshes)
{

}
