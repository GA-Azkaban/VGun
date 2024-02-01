#include "ResourceManager.h"
#include "Mesh.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "DDSTextureLoader.h"
#include "WICTextureLoader.h"
#include "GeometryGenerator.h"
#include "AssimpMathConverter.h"

#define MODELS_DIRECTORY_NAME "Resources/3DModels/"
#define TEXTURES_DIRECTORY_NAME "Resources/Textures/"

LazyObjects<ResourceManager> ResourceManager::Instance;

ResourceManager::ResourceManager()
	: _geometryGen(0)
{
	_geometryGen = new GeometryGenerator();
}

ResourceManager::~ResourceManager()
{
	delete _geometryGen;
}

void ResourceManager::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	_device = device;
	_deviceContext = deviceContext;

	LoadShaders();
	CreatePrimitiveMeshes();
}

void ResourceManager::LoadShaders()
{
	VertexShader* vertexShader = new VertexShader(_device.Get(), _deviceContext.Get());
	if (vertexShader->LoadShaderFile(L"Resources/Shaders/VertexShader.cso"))
		_vertexShaders.insert(std::make_pair("VertexShader.cso", vertexShader));

	PixelShader* pixelShader = new PixelShader(_device.Get(), _deviceContext.Get());
	if (pixelShader->LoadShaderFile(L"Resources/Shaders/PixelShader.cso"))
		_pixelShaders.insert(std::make_pair("PixelShader.cso", pixelShader));

	VertexShader* skeletonVertexShader = new VertexShader(_device.Get(), _deviceContext.Get());
	if (skeletonVertexShader->LoadShaderFile(L"Resources/Shaders/SkeletonVertexShader.cso"))
		_vertexShaders.insert(std::make_pair("SkeletonVertexShader.cso", skeletonVertexShader));

	PixelShader* skeletonPixelShader = new PixelShader(_device.Get(), _deviceContext.Get());
	if (skeletonPixelShader->LoadShaderFile(L"Resources/Shaders/SkeletonPixelShader.cso"))
		_pixelShaders.insert(std::make_pair("SkeletonPixelShader.cso", skeletonPixelShader));

	VertexShader* deferredVertexShader = new VertexShader(_device.Get(), _deviceContext.Get());
	if (deferredVertexShader->LoadShaderFile(L"Resources/Shaders/DeferredStaticMeshVS.cso"))
		_vertexShaders.insert(std::make_pair("DeferredStaticMeshVS.cso", deferredVertexShader));

	PixelShader* deferredPixelShader = new PixelShader(_device.Get(), _deviceContext.Get());
	if (deferredPixelShader->LoadShaderFile(L"Resources/Shaders/DeferredStaticMeshPS.cso"))
		_pixelShaders.insert(std::make_pair("DeferredStaticMeshPS.cso", deferredPixelShader));

	VertexShader* deferredSkeletonVertexShader = new VertexShader(_device.Get(), _deviceContext.Get());
	if (deferredSkeletonVertexShader->LoadShaderFile(L"Resources/Shaders/DeferredSkeletonMeshVS.cso"))
		_vertexShaders.insert(std::make_pair("DeferredSkeletonMeshVS.cso", deferredSkeletonVertexShader));

	PixelShader* deferredSkeletonPixelShader = new PixelShader(_device.Get(), _deviceContext.Get());
	if (deferredSkeletonPixelShader->LoadShaderFile(L"Resources/Shaders/DeferredSkeletonMeshPS.cso"))
		_pixelShaders.insert(std::make_pair("DeferredSkeletonMeshPS.cso", deferredSkeletonPixelShader));

	VertexShader* debugVertexShader = new VertexShader(_device.Get(), _deviceContext.Get());
	if (debugVertexShader->LoadShaderFile(L"Resources/Shaders/DebugVertexShader.cso"))
		_vertexShaders.insert(std::make_pair("DebugVertexShader.cso", debugVertexShader));

	PixelShader* debugPixelShader = new PixelShader(_device.Get(), _deviceContext.Get());
	if (debugPixelShader->LoadShaderFile(L"Resources/Shaders/DebugPixelShader.cso"))
		_pixelShaders.insert(std::make_pair("DebugPixelShader.cso", debugPixelShader));

	VertexShader* skyboxVertexShader = new VertexShader(_device.Get(), _deviceContext.Get());
	if (skyboxVertexShader->LoadShaderFile(L"Resources/Shaders/SkyboxVertexShader.cso"))
		_vertexShaders.insert(std::make_pair("SkyboxVertexShader.cso", skyboxVertexShader));

	PixelShader* skyboxPixelShader = new PixelShader(_device.Get(), _deviceContext.Get());
	if (skyboxPixelShader->LoadShaderFile(L"Resources/Shaders/SkyboxPixelShader.cso"))
		_pixelShaders.insert(std::make_pair("SkyboxPixelShader.cso", skyboxPixelShader));

	VertexShader* cubeMapVertexShader = new VertexShader(_device.Get(), _deviceContext.Get());
	if (cubeMapVertexShader->LoadShaderFile(L"Resources/Shaders/CubeMapVertexShader.cso"))
		_vertexShaders.insert(std::make_pair("CubeMapVertexShader.cso", cubeMapVertexShader));

	PixelShader* cubeMapPixelShader = new PixelShader(_device.Get(), _deviceContext.Get());
	if (cubeMapPixelShader->LoadShaderFile(L"Resources/Shaders/CubeMapPixelShader.cso"))
		_pixelShaders.insert(std::make_pair("CubeMapPixelShader.cso", cubeMapPixelShader));

	VertexShader* quadVertexShader = new VertexShader(_device.Get(), _deviceContext.Get());
	if (quadVertexShader->LoadShaderFile(L"Resources/Shaders/FullScreenQuadVertexShader.cso"))
		_vertexShaders.insert(std::make_pair("FullScreenQuadVertexShader.cso", quadVertexShader));

	PixelShader* quadPixelShader = new PixelShader(_device.Get(), _deviceContext.Get());
	if (quadPixelShader->LoadShaderFile(L"Resources/Shaders/FullScreenQuadPixelShader.cso"))
		_pixelShaders.insert(std::make_pair("FullScreenQuadPixelShader.cso", quadPixelShader));

	PixelShader* blitPixelShader = new PixelShader(_device.Get(), _deviceContext.Get());
	if (blitPixelShader->LoadShaderFile(L"Resources/Shaders/BlitPixelShader.cso"))
		_pixelShaders.insert(std::make_pair("BlitPixelShader.cso", blitPixelShader));

}

void ResourceManager::CreatePrimitiveMeshes()
{
	GeometryGenerator::DebugMeshData gridMesh;
	_geometryGen->CreateGrid(gridMesh);
	GeometryGenerator::DebugMeshData axisMesh;
	_geometryGen->CreateAxis(axisMesh);

	Mesh* _gridMesh = new ::Mesh(&gridMesh.Vertices[0], gridMesh.Vertices.size(), &gridMesh.Indices[0], gridMesh.Indices.size());
	_loadedFileInfo["grid"].loadedMeshes.push_back(_gridMesh);
	Mesh* _axisMesh = new ::Mesh(&axisMesh.Vertices[0], axisMesh.Vertices.size(), &axisMesh.Indices[0], axisMesh.Indices.size());
	_loadedFileInfo["axis"].loadedMeshes.push_back(_axisMesh);

	GeometryGenerator::MeshData skybox;
	_geometryGen->CreateSkybox(skybox);

	Mesh* _skybox = new ::Mesh(&skybox.Vertices[0], skybox.Vertices.size(), &skybox.Indices[0], skybox.Indices.size());
	_loadedFileInfo["skybox"].loadedMeshes.push_back(_skybox);

	GeometryGenerator::MeshData skySphere;
	_geometryGen->CreateSphere(5000, 30, 30, skySphere);

	Mesh* _sphere = new Mesh(&skySphere.Vertices[0], skySphere.Vertices.size(), &skySphere.Indices[0], skySphere.Indices.size());
	_loadedFileInfo["skySphere"].loadedMeshes.push_back(_sphere);
}

void ResourceManager::LoadFBXFile(std::string fileName)
{
	UINT slashIndex = fileName.find_last_of("/\\");
	if (slashIndex != std::string::npos)
	{
		_fileName = fileName.substr(slashIndex + 1, fileName.length() - slashIndex);
	}
	else
	{
		_fileName = fileName;
	}

	std::string path = std::string(MODELS_DIRECTORY_NAME) + fileName;

	Assimp::Importer importer;

	const aiScene* _scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);

	if (_scene == nullptr || _scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || _scene->mRootNode == nullptr)
	{
		MessageBox(NULL, L"Model file couldn't be loaded", L"Error!", MB_ICONERROR | MB_OK);
	}

	ProcessNode(_scene->mRootNode, _scene);
	LoadAnimation(_scene);
}

void ResourceManager::LoadTextureFile(std::string fileName)
{
	ID3D11ShaderResourceView* srv;
	UINT slashIndex = fileName.find_last_of("/\\");
	if (slashIndex != std::string::npos)
	{
		fileName = fileName.substr(slashIndex + 1, fileName.length() - slashIndex);
	}
	std::string path = std::string(TEXTURES_DIRECTORY_NAME) + fileName;
	std::string extension = fileName.substr(fileName.find_last_of(".") + 1, fileName.length() - fileName.find_last_of("."));
	std::wstring pathWS = std::wstring(path.begin(), path.end());

	HRESULT hr = S_FALSE;

	if (extension == "dds")
	{
		hr = CreateDDSTextureFromFile(_device.Get(), pathWS.c_str(), nullptr, &srv);
	}
	else
	{
		hr = CreateWICTextureFromFile(_device.Get(), _deviceContext.Get(), pathWS.c_str(), nullptr, &srv);
	}

	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Texture couldn't be loaded", L"Error!", MB_ICONERROR | MB_OK);
	}

	_loadedTextures.insert(std::make_pair(fileName, srv));
}

std::vector<Mesh*>& ResourceManager::GetMeshes(const std::string& fileName)
{
	if (_loadedFileInfo.find(fileName) == _loadedFileInfo.end())
	{
		LoadFBXFile(fileName);
	}
	return _loadedFileInfo[fileName].loadedMeshes;
}

ID3D11ShaderResourceView* ResourceManager::GetTexture(const std::string& fileName)
{
	if (_loadedTextures.find(fileName) == _loadedTextures.end())
	{
		LoadTextureFile(fileName);
	}
	return _loadedTextures[fileName];
}

Node* ResourceManager::GetNode(const std::string& fileName)
{
	if (_loadedFileInfo.find(fileName) == _loadedFileInfo.end())
	{
		LoadFBXFile(fileName);
	}
	return _loadedFileInfo[fileName].node;
}

std::unordered_map<std::string, Animation*>& ResourceManager::GetAnimations(const std::string& fileName)
{
	if (_loadedFileInfo.find(fileName) == _loadedFileInfo.end())
	{
		LoadFBXFile(fileName);
	}
	return _loadedFileInfo[fileName].loadedAnimation;
}

VertexShader* ResourceManager::GetVertexShader(const std::string& name)
{
	if (_vertexShaders.find(name) == _vertexShaders.end())
	{
		return nullptr;
	}

	return _vertexShaders[name];
}

PixelShader* ResourceManager::GetPixelShader(const std::string& name)
{
	if (_pixelShaders.find(name) == _pixelShaders.end())
	{
		return nullptr;
	}

	return _pixelShaders[name];
}

void ResourceManager::ProcessNode(aiNode* node, const aiScene* scene)
{
	for (UINT i = 0; i < node->mNumMeshes; ++i)
	{
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

	Mesh* newMesh = new Mesh(&vertices[0], vertices.size(), &indices[0], indices.size());
	_loadedFileInfo[_fileName].loadedMeshes.push_back(newMesh);

	int upAxis = 0;
	scene->mMetaData->Get<int>("UpAxis", upAxis);
	int upAxisSign = 1;
	scene->mMetaData->Get<int>("UpAxisSign", upAxisSign);
	int frontAxis = 0;
	scene->mMetaData->Get<int>("FrontAxis", frontAxis);
	int frontAxisSign = 1;
	scene->mMetaData->Get<int>("FrontAxisSign", frontAxisSign);
	int coordAxis = 0;
	scene->mMetaData->Get<int>("CoordAxis", coordAxis);
	int coordAxisSign = 1;
	scene->mMetaData->Get<int>("CoordAxisSign", coordAxisSign);

	int originalUpAxis = 0;
	scene->mMetaData->Get<int>("OriginalUpAxis", originalUpAxis);
	int originalUpAxisSign = 1;
	scene->mMetaData->Get<int>("OriginalUpAxisSign", originalUpAxisSign);

	float unitScaleFactor = 1.0f;
	scene->mMetaData->Get<float>("UnitScaleFactor", unitScaleFactor);
	float originalUnitScaleFactor = 1.0f;
	scene->mMetaData->Get<float>("OriginalUnitScaleFactor", originalUnitScaleFactor);

	aiVector3D upVec = upAxis == 0 ? aiVector3D(upAxisSign, 0, 0) : upAxis == 1 ? aiVector3D(0, upAxisSign, 0) : aiVector3D(0, 0, upAxisSign);
	aiVector3D forwardVec = frontAxis == 0 ? aiVector3D(frontAxisSign, 0, 0) : frontAxis == 1 ? aiVector3D(0, frontAxisSign, 0) : aiVector3D(0, 0, frontAxisSign);
	aiVector3D rightVec = coordAxis == 0 ? aiVector3D(coordAxisSign, 0, 0) : coordAxis == 1 ? aiVector3D(0, coordAxisSign, 0) : aiVector3D(0, 0, coordAxisSign);

	upVec *= unitScaleFactor;
	forwardVec *= unitScaleFactor;
	rightVec *= unitScaleFactor;

	/*aiMatrix4x4 mat(
		rightVec.x, rightVec.y, rightVec.z, 0.0f,
		-upVec.x, -upVec.y, -upVec.z, 0.0f,
		forwardVec.x, forwardVec.y, forwardVec.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);	*/

	aiMatrix4x4 mat(
		rightVec.x, forwardVec.x, -upVec.x, 0.0f,
		rightVec.y, forwardVec.y, -upVec.y, 0.0f,
		rightVec.z, forwardVec.z, -upVec.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	// create node hierarchy
	Node* rootNode = new Node();
	DirectX::XMMATRIX rootNodeTM = AIMatrix4x4ToXMMatrix(scene->mRootNode->mTransformation * mat);
	rootNode->rootNodeInvTransform = DirectX::XMMatrixInverse(0, rootNodeTM);

	_loadedFileInfo[_fileName].node = rootNode;

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

	int upAxis = 0;
	scene->mMetaData->Get<int>("UpAxis", upAxis);
	int upAxisSign = 1;
	scene->mMetaData->Get<int>("UpAxisSign", upAxisSign);
	int frontAxis = 0;
	scene->mMetaData->Get<int>("FrontAxis", frontAxis);
	int frontAxisSign = 1;
	scene->mMetaData->Get<int>("FrontAxisSign", frontAxisSign);
	int coordAxis = 0;
	scene->mMetaData->Get<int>("CoordAxis", coordAxis);
	int coordAxisSign = 1;
	scene->mMetaData->Get<int>("CoordAxisSign", coordAxisSign);

	int originalUpAxis = 0;
	scene->mMetaData->Get<int>("OriginalUpAxis", originalUpAxis);
	int originalUpAxisSign = 1;
	scene->mMetaData->Get<int>("OriginalUpAxisSign", originalUpAxisSign);

	float unitScaleFactor = 1.0f;
	scene->mMetaData->Get<float>("UnitScaleFactor", unitScaleFactor);
	float originalUnitScaleFactor = 1.0f;
	scene->mMetaData->Get<float>("OriginalUnitScaleFactor", originalUnitScaleFactor);

	aiVector3D upVec = upAxis == 0 ? aiVector3D(upAxisSign, 0, 0) : upAxis == 1 ? aiVector3D(0, upAxisSign, 0) : aiVector3D(0, 0, upAxisSign);
	aiVector3D forwardVec = frontAxis == 0 ? aiVector3D(frontAxisSign, 0, 0) : frontAxis == 1 ? aiVector3D(0, frontAxisSign, 0) : aiVector3D(0, 0, frontAxisSign);
	aiVector3D rightVec = coordAxis == 0 ? aiVector3D(coordAxisSign, 0, 0) : coordAxis == 1 ? aiVector3D(0, coordAxisSign, 0) : aiVector3D(0, 0, coordAxisSign);

	upVec *= unitScaleFactor;
	forwardVec *= unitScaleFactor;
	rightVec *= unitScaleFactor;

	/*aiMatrix4x4 mat(
		rightVec.x, rightVec.y, rightVec.z, 0.0f,
		-upVec.x, -upVec.y, -upVec.z, 0.0f,
		forwardVec.x, forwardVec.y, forwardVec.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);	*/

	aiMatrix4x4 mat(
		rightVec.x, forwardVec.x, -upVec.x, 0.0f,
		rightVec.y, forwardVec.y, -upVec.y, 0.0f,
		rightVec.z, forwardVec.z, -upVec.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	// create node hierarchy
	Node* rootNode = new Node();
	DirectX::XMMATRIX rootNodeTM = AIMatrix4x4ToXMMatrix(scene->mRootNode->mTransformation * mat);
	rootNode->rootNodeInvTransform = DirectX::XMMatrixInverse(0, rootNodeTM);
	ReadNodeHierarchy(*rootNode, scene->mRootNode, boneInfo);

	_loadedFileInfo[_fileName].node = rootNode;

	Mesh* newMesh = new Mesh(&vertices[0], vertices.size(), &indices[0], indices.size());
	_loadedFileInfo[_fileName].loadedMeshes.push_back(newMesh);

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
		auto iter = _loadedTextures.find(fileName);
		if (iter == _loadedTextures.end())
		{
			const aiTexture* embeddedTexture = scene->GetEmbeddedTexture(str.C_Str());
			if (embeddedTexture != nullptr)
			{
				ID3D11ShaderResourceView* texture = LoadEmbeddedTexture(embeddedTexture);
				_loadedTextures.insert(std::make_pair(fileName, texture));
			}
			else
			{
				LoadTextureFile(fileName);
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
		hr = _device->CreateTexture2D(&desc, &subresourceData, &texture2D);
		if (FAILED(hr))
			MessageBox(nullptr, L"CreateTexture2D failed!", L"Error!", MB_ICONERROR | MB_OK);

		hr = _device->CreateShaderResourceView(texture2D, nullptr, &texture);
		if (FAILED(hr))
			MessageBox(nullptr, L"CreateShaderResourceView failed!", L"Error!", MB_ICONERROR | MB_OK);

		return texture;
	}

	// mHeight is 0, so try to load a compressed texture of mWidth bytes
	const size_t size = embeddedTexture->mWidth;

	hr = CreateWICTextureFromMemory(_device.Get(), _deviceContext.Get(), reinterpret_cast<const unsigned char*>(embeddedTexture->pcData), size, nullptr, &texture);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Texture couldn't be loaded", L"Error!", MB_ICONERROR | MB_OK);
	}

	return texture;
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
		_loadedFileInfo[_fileName].loadedAnimation.insert(std::make_pair(animation->mName.C_Str(), newAnimation));
	}
}
