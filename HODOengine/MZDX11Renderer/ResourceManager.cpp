#include "ResourceManager.h"
#include "ResourceManager.h"
#include "Mesh.h"
#include "DDSTextureLoader.h"
#include "WICTextureLoader.h"

LazyObjects<ResourceManager> ResourceManager::Instance;

ResourceManager::ResourceManager()
{

}

ResourceManager::~ResourceManager()
{

}

void ResourceManager::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	m_device = device;
	m_deviceContext = deviceContext;
}

void ResourceManager::LoadFile(std::string fileName)
{
	Assimp::Importer importer;

	const aiScene* _scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);

	if (_scene == nullptr || _scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || _scene->mRootNode == nullptr)
	{
		MessageBox(NULL, L"Model file couldn't be loaded", L"Error!", MB_ICONERROR | MB_OK);
	}

	m_directory = fileName.substr(0, fileName.find_last_of("/\\"));
	m_fileName = fileName.substr(fileName.find_last_of("/\\") + 1, fileName.length() - fileName.find_last_of("/\\"));

	ProcessNode(_scene->mRootNode, _scene);
}

std::vector<::Mesh*> ResourceManager::GetLoadedMesh(const std::string& fileName)
{
	if (m_loadedMeshes.find(fileName) == m_loadedMeshes.end())
	{
		LoadFile(fileName);
	}
	return m_loadedMeshes[fileName];
}

Texture ResourceManager::GetLoadedTexture(const std::string& fileName)
{
	if (m_loadedTextures.find(fileName) != m_loadedTextures.end())
	{
		return m_loadedTextures[fileName];
	}
	Texture ret;
	ret.texture = nullptr;
	return ret;
}

void ResourceManager::ProcessNode(aiNode* node, const aiScene* scene)
{
	for (UINT i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		//if (mesh->mNumBones == 0)
		//{
		std::vector<VertexStruct::Vertex> vertices;
		std::vector<UINT> indices;
		ProcessMesh(mesh, scene, vertices, indices);
		//}
		//else
		//{
		//	std::vector<VertexStruct::VertexSkinning> vertices;
		//	std::vector<UINT> indices;
		//	ProcessMesh(mesh, scene, vertices, indices);
		//}
	}

	for (UINT i = 0; i < node->mNumChildren; ++i)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

void ResourceManager::ProcessMesh(aiMesh* mesh, const aiScene* scene, std::vector<VertexStruct::Vertex>& vertices, std::vector<UINT>& indices)
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
	m_loadedMeshes[m_fileName].push_back(newMesh);

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		LoadMaterialTextures(material, aiTextureType_DIFFUSE, scene);
		LoadMaterialTextures(material, aiTextureType_NORMALS, scene);
		LoadMaterialTextures(material, aiTextureType_DIFFUSE_ROUGHNESS, scene);
		LoadMaterialTextures(material, aiTextureType_METALNESS, scene);
		LoadMaterialTextures(material, aiTextureType_DISPLACEMENT, scene);
	}
}

void ResourceManager::ProcessMesh(aiMesh* mesh, const aiScene* scene, std::vector<VertexStruct::VertexSkinning>& vertices, std::vector<UINT>& indices)
{

}

void ResourceManager::LoadMaterialTextures(aiMaterial* material, aiTextureType type, const aiScene* scene)
{
	UINT textureCount = material->GetTextureCount(type);
	for (UINT i = 0; i < textureCount; ++i)
	{
		aiString str;
		material->GetTexture(type, i, &str);
		std::string s = std::string(str.C_Str());
		std::string fileName = s.substr(s.find_last_of("/\\") + 1, s.length() - s.find_last_of("/\\"));
		// Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		auto iter = m_loadedTextures.find(fileName);
		if (iter == m_loadedTextures.end())
		{
			Texture texture;
			const aiTexture* embeddedTexture = scene->GetEmbeddedTexture(str.C_Str());
			if (embeddedTexture != nullptr)
			{
				texture.texture = LoadEmbeddedTexture(embeddedTexture);
			}
			else
			{
				std::string path = m_directory + "/Textures/" + fileName;
				std::string extension = fileName.substr(fileName.find_last_of(".") + 1, fileName.length() - fileName.find_last_of("."));
				std::wstring pathWS = std::wstring(path.begin(), path.end());

				HRESULT hr = S_FALSE;

				if (extension == "png")
				{
					hr = CreateWICTextureFromFile(m_device.Get(), m_deviceContext.Get(), pathWS.c_str(), nullptr, texture.texture.GetAddressOf());
				}
				else if (extension == "dds")
				{
					hr = CreateDDSTextureFromFile(m_device.Get(), pathWS.c_str(), nullptr, texture.texture.GetAddressOf());
				}

				if (FAILED(hr))
				{
					MessageBox(nullptr, L"Texture couldn't be loaded", L"Error!", MB_ICONERROR | MB_OK);
				}

				texture.type = type;
				m_loadedTextures.insert(std::make_pair(fileName, texture));
			}
		}
	}
}

ID3D11ShaderResourceView* ResourceManager::LoadEmbeddedTexture(const aiTexture* embeddedTexture)
{
	HRESULT hr;
	ID3D11ShaderResourceView* texture = nullptr;

	if (embeddedTexture->mHeight != 0) {
		// Load an uncompressed ARGB8888 embedded texture
		D3D11_TEXTURE2D_DESC desc;
		desc.Width = embeddedTexture->mWidth;
		desc.Height = embeddedTexture->mHeight;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA subresourceData;
		subresourceData.pSysMem = embeddedTexture->pcData;
		subresourceData.SysMemPitch = embeddedTexture->mWidth * 4;
		subresourceData.SysMemSlicePitch = embeddedTexture->mWidth * embeddedTexture->mHeight * 4;

		ID3D11Texture2D* texture2D = nullptr;
		hr = m_device->CreateTexture2D(&desc, &subresourceData, &texture2D);
		if (FAILED(hr))
			MessageBox(nullptr, L"CreateTexture2D failed!", L"Error!", MB_ICONERROR | MB_OK);

		hr = m_device->CreateShaderResourceView(texture2D, nullptr, &texture);
		if (FAILED(hr))
			MessageBox(nullptr, L"CreateShaderResourceView failed!", L"Error!", MB_ICONERROR | MB_OK);

		return texture;
	}

	// mHeight is 0, so try to load a compressed texture of mWidth bytes
	const size_t size = embeddedTexture->mWidth;

	hr = CreateWICTextureFromMemory(m_device.Get(), m_deviceContext.Get(), reinterpret_cast<const unsigned char*>(embeddedTexture->pcData), size, nullptr, &texture);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Texture couldn't be loaded", L"Error!", MB_ICONERROR | MB_OK);
	}

	return texture;
}
