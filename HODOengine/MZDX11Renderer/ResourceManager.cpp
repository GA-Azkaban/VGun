#include "ResourceManager.h"
#include "Node.h"
#include "Mesh.h"
#include "DDSTextureLoader.h"
#include "WICTextureLoader.h"
#include "GeometryGenerator.h"
#include "AssimpMathConverter.h"

LazyObjects<ResourceManager> ResourceManager::Instance;

ResourceManager::ResourceManager()
	: m_geometryGen(0), m_directory("../3DModels/")
{
	m_geometryGen = new GeometryGenerator();
}

ResourceManager::~ResourceManager()
{

}

void ResourceManager::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	m_device = device;
	m_deviceContext = deviceContext;

	CreatePrimitiveMeshes();
}

void ResourceManager::CreatePrimitiveMeshes()
{
	GeometryGenerator::DebugMeshData gridMesh;
	m_geometryGen->CreateGrid(gridMesh);
	GeometryGenerator::DebugMeshData axisMesh;
	m_geometryGen->CreateAxis(axisMesh);

	Mesh* _gridMesh = new ::Mesh(&gridMesh.Vertices[0], gridMesh.Vertices.size(), &gridMesh.Indices[0], gridMesh.Indices.size());
	m_loadedMeshes["grid"].push_back(_gridMesh);
	Mesh* _axisMesh = new ::Mesh(&axisMesh.Vertices[0], axisMesh.Vertices.size(), &axisMesh.Indices[0], axisMesh.Indices.size());
	m_loadedMeshes["axis"].push_back(_axisMesh);

	GeometryGenerator::MeshData skySphere;
	m_geometryGen->CreateSphere(5000, 30, 30, skySphere);

	Mesh* _sphere = new ::Mesh(&skySphere.Vertices[0], skySphere.Vertices.size(), &skySphere.Indices[0], skySphere.Indices.size());
	m_loadedMeshes["skySphere"].push_back(_sphere);
}

void ResourceManager::LoadFBXFile(std::string filePath)
{
	Assimp::Importer importer;


	const aiScene* _scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);

	if (_scene == nullptr || _scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || _scene->mRootNode == nullptr)
	{
		MessageBox(NULL, L"Model file couldn't be loaded", L"Error!", MB_ICONERROR | MB_OK);
	}

	//m_directory = fileName.substr(0, fileName.find_last_of("/\\") + 1);
	m_fileName = filePath.substr(filePath.find_last_of("/\\") + 1, filePath.length() - filePath.find_last_of("/\\"));

	ProcessNode(_scene->mRootNode, _scene);
	LoadAnimation(_scene);
}

void ResourceManager::LoadTextureFile(std::string filePath)
{
	std::string fileName = filePath.substr(filePath.find_last_of("/\\") + 1, filePath.length() - filePath.find_last_of("/\\"));
	LoadTexture(fileName);
}

std::vector<Mesh*>& ResourceManager::GetLoadedMesh(const std::string& fileName)
{
	if (m_loadedMeshes.find(fileName) == m_loadedMeshes.end())
	{
		LoadFBXFile(m_directory + fileName);
	}
	return m_loadedMeshes[fileName];
}

ID3D11ShaderResourceView* ResourceManager::GetLoadedTexture(const std::string& fileName)
{
	if (m_loadedTextures.find(fileName) == m_loadedTextures.end())
	{
		LoadTexture(fileName);
	}
	return m_loadedTextures[fileName];
}

std::unordered_map<std::string, Animation*>& ResourceManager::GetAnimations(const std::string& fileName)
{
	if (m_loadedAnimation.find(fileName) != m_loadedAnimation.end())
	{
		return m_loadedAnimation[fileName];
	}
	// 애니메이션이 없다면?
	// 일단은 애니메이션 정보 없이 리턴한다.
	//LoadFBXFile(m_directory + fileName);
	std::unordered_map<std::string, Animation*> ret = {};
	return ret;
}

void ResourceManager::ProcessNode(aiNode* node, const aiScene* scene)
{
	for (UINT i = 0; i < node->mNumMeshes; ++i)
	{
		//aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		aiMesh* mesh = scene->mMeshes[i];
		ProcessMesh(mesh, scene);
	}

	for (UINT i = 0; i < node->mNumChildren; ++i)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

void ResourceManager::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	if (scene->mNumAnimations > 0)
	{
		ProcessSkinningMesh(mesh, scene);
	}
	else
	{
		ProcessStaticMesh(mesh, scene);
	}
}

void ResourceManager::ProcessStaticMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<VertexStruct::Vertex> vertices;
	std::vector<UINT> indices;

	// Load position, normal, uv
	for (UINT i = 0; i < mesh->mNumVertices; ++i)
	{
		VertexStruct::Vertex vertex;

		// process position
		vertex.Position.x = mesh->mVertices[i].x;
		vertex.Position.y = -mesh->mVertices[i].y;
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

	Mesh* newMesh = new Mesh(&vertices[0], vertices.size(), &indices[0], indices.size());
	m_loadedMeshes[m_fileName].push_back(newMesh);

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		for (UINT i = 0; i <= aiTextureType_UNKNOWN; ++i)
		{
			LoadMaterialTextures(material, (aiTextureType)i, scene);
		}
	}
}

void ResourceManager::ProcessSkinningMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<VertexStruct::VertexSkinning> vertices;
	std::vector<UINT> indices;

	// Load position, normal, uv
	for (UINT i = 0; i < mesh->mNumVertices; ++i)
	{
		VertexStruct::VertexSkinning vertex;

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

		vertex.BoneIndices = DirectX::XMUINT4{ 0, 0, 0, 0 };
		vertex.Weights = DirectX::XMFLOAT4{ 0.0f, 0.0f, 0.0f, 0.0f };

		vertices.push_back(vertex);
	}

	// Load boneData to vertices
	std::unordered_map<std::string, std::pair<int, DirectX::XMMATRIX>> boneInfo = {};
	std::vector<UINT> boneCounts;
	boneCounts.resize(vertices.size(), 0);

	int _boneCount = mesh->mNumBones;
	// loop through each bone
	for (UINT i = 0; i < _boneCount; ++i)
	{
		aiBone* bone = mesh->mBones[i];
		DirectX::XMMATRIX m = AIMatrix4x4ToXMMatrix(bone->mOffsetMatrix);
		boneInfo[bone->mName.C_Str()] = { i, m };

		// loop through each vertex that have that bone
		for (UINT j = 0; j < bone->mNumWeights; ++j)
		{
			if (bone->mWeights == nullptr)
				break;

			UINT id = bone->mWeights[j].mVertexId;
			float weight = bone->mWeights[j].mWeight;
			boneCounts[id]++;
			switch (boneCounts[id])
			{
				case 1:
					vertices[id].BoneIndices.x = i;
					vertices[id].Weights.x = weight;
					break;
				case 2:
					vertices[id].BoneIndices.y = i;
					vertices[id].Weights.y = weight;
					break;
				case 3:
					vertices[id].BoneIndices.z = i;
					vertices[id].Weights.z = weight;
					break;
				case 4:
					vertices[id].BoneIndices.w = i;
					vertices[id].Weights.w = weight;
					break;
				default:
					break;
			}
		}
	}

	// normalize weights to make all weights sum 1
	for (UINT i = 0; i < vertices.size(); ++i)
	{
		DirectX::XMFLOAT4 boneWeights = vertices[i].Weights;
		float totalWeight = boneWeights.x + boneWeights.y + boneWeights.z + boneWeights.w;
		if (totalWeight > 0.0f)
		{
			vertices[i].Weights = DirectX::XMFLOAT4{
				boneWeights.x / totalWeight,
				boneWeights.y / totalWeight,
				boneWeights.z / totalWeight,
				boneWeights.w / totalWeight
			};
		}
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

	// create node hierarchy
	Node rootNode;
	DirectX::XMMATRIX rootNodeTM = AIMatrix4x4ToXMMatrix(scene->mRootNode->mTransformation);
	rootNode.rootNodeInvTransform = DirectX::XMMatrixInverse(0, rootNodeTM);
	//rootNode.rootNodeInvTransform = DirectX::XMMatrixTranspose(rootNode.rootNodeInvTransform);
	ReadNodeHierarchy(rootNode, scene->mRootNode, boneInfo);

	Mesh* newMesh = new Mesh(&vertices[0], vertices.size(), &indices[0], indices.size(), rootNode);
	m_loadedMeshes[m_fileName].push_back(newMesh);

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		for (UINT i = 0; i <= aiTextureType_UNKNOWN; ++i)
		{
			LoadMaterialTextures(material, (aiTextureType)i, scene);
		}
	}
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
			const aiTexture* embeddedTexture = scene->GetEmbeddedTexture(str.C_Str());
			if (embeddedTexture != nullptr)
			{
				ID3D11ShaderResourceView* texture = LoadEmbeddedTexture(embeddedTexture);
				m_loadedTextures.insert(std::make_pair(fileName, texture));
			}
			else
			{
				LoadTexture(fileName);
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

ID3D11ShaderResourceView* ResourceManager::LoadTexture(const std::string& fileName)
{
	ID3D11ShaderResourceView* ret = nullptr;

	std::string path = m_directory + "Textures/" + fileName;
	std::string extension = fileName.substr(fileName.find_last_of(".") + 1, fileName.length() - fileName.find_last_of("."));
	std::wstring pathWS = std::wstring(path.begin(), path.end());

	HRESULT hr = S_FALSE;

	if (extension == "dds")
	{
		hr = CreateDDSTextureFromFile(m_device.Get(), pathWS.c_str(), nullptr, &ret);
	}
	else
	{
		hr = CreateWICTextureFromFile(m_device.Get(), m_deviceContext.Get(), pathWS.c_str(), nullptr, &ret);
	}

	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Texture couldn't be loaded", L"Error!", MB_ICONERROR | MB_OK);
	}

	m_loadedTextures.insert(std::make_pair(fileName, ret));

	return ret;
}

// A recursive function to read all bones and form skeleton
void ResourceManager::ReadNodeHierarchy(Node& nodeOutput, aiNode* node, std::unordered_map<std::string, std::pair<int, DirectX::XMMATRIX>>& boneInfo)
{
	if (boneInfo.find(node->mName.C_Str()) != boneInfo.end())
	{
		nodeOutput.name = node->mName.C_Str();
		nodeOutput.nodeTransform = AIMatrix4x4ToXMMatrix(node->mTransformation);
		//nodeOutput.nodeTransform = XMMatrixTranspose(nodeOutput.nodeTransform);

		Bone bone;
		bone.id = boneInfo[nodeOutput.name].first;
		bone.offset = boneInfo[nodeOutput.name].second;
		//bone.offset = XMMatrixTranspose(bone.offset);

		nodeOutput.bone = bone;

		for (int i = 0; i < node->mNumChildren; ++i)
		{
			Node child;
			ReadNodeHierarchy(child, node->mChildren[i], boneInfo);
			nodeOutput.children.push_back(child);
		}
	}
	else
	{
		for (int i = 0; i < node->mNumChildren; ++i)
		{
			ReadNodeHierarchy(nodeOutput, node->mChildren[i], boneInfo);
		}
	}
}

void ResourceManager::LoadAnimation(const aiScene* scene)
{
	// channel in animation contains aiNodeAnim (aiNodeAnim its transformation for bones)
	// numChannels == numBones
	UINT animCount = scene->mNumAnimations;
	for (UINT i = 0; i < animCount; ++i)
	{
		const aiAnimation* animation = scene->mAnimations[i];
		Animation* newAnimation = new Animation();
		newAnimation->duration = animation->mDuration;

		if (scene->mAnimations[i]->mTicksPerSecond != 0.0)
		{
			newAnimation->ticksPerSecond = animation->mTicksPerSecond;
		}
		else
		{
			newAnimation->ticksPerSecond = 30.0f;
		}

		for (UINT j = 0; j < animation->mNumChannels; ++j)
		{
			const aiNodeAnim* nodeAnim = animation->mChannels[j];
			NodeAnimation* newNodeAnim = new NodeAnimation();

			newNodeAnim->nodeName = nodeAnim->mNodeName.C_Str();
			
			for (int k = 0; k < nodeAnim->mNumPositionKeys; ++k)
			{
				newNodeAnim->positionTimestamps.push_back(nodeAnim->mPositionKeys[k].mTime);
				newNodeAnim->positions.push_back(AIVec3ToXMFloat3(nodeAnim->mPositionKeys[k].mValue));
			}
			for (int k = 0; k < nodeAnim->mNumRotationKeys; ++k)
			{
				newNodeAnim->rotationTimestamps.push_back(nodeAnim->mRotationKeys[k].mTime);
				newNodeAnim->rotations.push_back(AIQuaternionToXMFloat4(nodeAnim->mRotationKeys[k].mValue));
			}
			for (int k = 0; k < nodeAnim->mNumScalingKeys; ++k)
			{
				newNodeAnim->scaleTimestamps.push_back(nodeAnim->mScalingKeys[k].mTime);
				newNodeAnim->scales.push_back(AIVec3ToXMFloat3(nodeAnim->mScalingKeys[k].mValue));
			}

			newAnimation->nodeAnimations.push_back(newNodeAnim);
		}
		m_loadedAnimation[m_fileName].insert(std::make_pair(animation->mName.C_Str(), newAnimation));
	}
}
