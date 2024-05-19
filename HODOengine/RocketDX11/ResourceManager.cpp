#include "ResourceManager.h"
#include "CubeMesh.h"
#include "Mesh.h"
#include "Material.h"
#include "Model.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "TextRenderer.h"
#include "ImageRenderer.h"
#include "RocketMacroDX11.h"
#include "DDSTextureLoader.h"
#include "WICTextureLoader.h"
#include "GeometryGenerator.h"
#include "AssimpMathConverter.h"
#include "SamplerState.h"
#include "ObjectManager.h"
#include "../HODO3DGraphicsInterface/IMaterial.h"

#define MODELS_DIRECTORY_NAME "Resources/Models/"
#define TEXTURES_DIRECTORY_NAME "Resources/Textures/"
#define CUBEMAPS_DIRECTORY_NAME "Resources/Textures/Skybox/"

using namespace DirectX;
using namespace DirectX::DX11;

namespace RocketCore::Graphics
{
	XMVECTOR forward[6] =
	{
		{ 1, 0, 0, 0 },
		{ -1, 0, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, -1, 0, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, -1, 0 },
	};

	XMVECTOR up[6] =
	{
		{ 0, 1, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 0, -1, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 1, 0,  0 },
		{ 0, 1, 0, 0 },
	};

	ResourceManager::ResourceManager()
		: _fileInfoKeyName("")
	{

	}

	void ResourceManager::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
	{
		_device = device;
		_deviceContext = deviceContext;

		_defaultFont = new DirectX::SpriteFont(_device.Get(), L"Resources/Font/NotoSansKR.spritefont");
		//_defaultFont = new DirectX::SpriteFont(_device.Get(), L"Resources/Font/KRAFTON_FontIncludeKR_10.spritefont");

		LoadShaders();
		CreateRasterizerStates();
		CreateSamplerStates();
		CreatePrimitiveMeshes();

		HDEngine::MaterialDesc defaultParticleDesc;
		defaultParticleDesc.materialName = "Default-ParticleSystem";
		defaultParticleDesc.color = { 255, 0, 255, 255 };
		ObjectManager::Instance().CreateMaterial(defaultParticleDesc);
	}

	void ResourceManager::LoadFBXFile(std::string path)
	{
		// 로드하는 모든 FBX 파일들은 팀에서 정한 파일의 명명 규칙을 따라야 한다.
		// 그 규칙에 맞는 파일들만 불러올 수 있도록 한다.
		// Resources/Models/ 경로에 있는 폴더 경로를 통째로 넣어줘야 한다.
		UINT slashIndex = path.find_last_of("/\\");
		if (slashIndex != std::string::npos)
		{
			// 파일 경로를 잘라서 파일 이름 자체만 들고 있는다.
			_fileName = path.substr(slashIndex + 1, path.length() - slashIndex);
		}
		else
		{
			_fileName = path;
		}

		// 확장자를 잘라낸다.
		UINT dotIndex = _fileName.find_last_of(".");
		_fileName = _fileName.substr(0, dotIndex);

		// SKM_TP_X_Breathing.fbx 나 SKM_Player_Breathing.fbx 이나 
		// SM_watch_tower.fbx 같은 파일명일 때
		// 우선 SM인지 SKM인지 구분한다.
		// SM이나 SKM이 없는 파일명이면 에러를 반환한다.
		UINT firstBarIndex = _fileName.find_first_of("_");
		if (firstBarIndex == std::string::npos)
		{
			MessageBox(NULL, L"Model file name error.", L"Error!", MB_ICONERROR | MB_OK);
		}
		std::string meshType = _fileName.substr(0, firstBarIndex);
		_fileName = _fileName.substr(firstBarIndex + 1, _fileName.length() - firstBarIndex);
		std::string animName = "";
		if (meshType == "SM")
		{
			_fileInfoKeyName = _fileName;
		}
		else if (meshType == "SKM")
		{
			// TP_X_Breathing 이나 Player_Breathing 형태로 나옴
			// 이 형태에서 첫번째 언더바를 기준으로 왼쪽 것이 파일명, 오른쪽 것이 애니메이션명
			UINT firstBarIndex2 = _fileName.find_first_of("_");
			if (firstBarIndex2 != std::string::npos)
			{
				_fileInfoKeyName = _fileName.substr(0, firstBarIndex2);
				animName = _fileName.substr(firstBarIndex2 + 1, _fileName.length() - firstBarIndex2);
			}
			else // 언더바 없는 파일명
			{
				_fileInfoKeyName = _fileName;
			}
		}
		else
		{
			MessageBox(NULL, L"Model file name error.", L"Error!", MB_ICONERROR | MB_OK);
		}

		std::string filePath = std::string(MODELS_DIRECTORY_NAME) + path;

		Assimp::Importer importer;

		const aiScene* _scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded | aiProcess_CalcTangentSpace);

		if (_scene == nullptr || _scene->mRootNode == nullptr)
		{
			MessageBox(NULL, L"Model file couldn't be loaded", L"Error!", MB_ICONERROR | MB_OK);
		}

		// SKM_ 으로 시작하는 파일명으로 받아온 FBX 파일은 메시랑 노드 한번만 로드한다.
		if (_scene->HasMeshes())
		{
			ProcessNode(_scene->mRootNode, _scene);
		}
		LoadAnimation(_scene, animName);
	}

	void ResourceManager::LoadTextureFile(std::string path)
	{
		// 경로를 제외한 파일 이름만 들고 있는다. 확장자는 포함해서 들고 있는다.
		std::string fileName = path;
		UINT slashIndex = fileName.find_last_of("/\\");
		if (slashIndex != std::string::npos)
		{
			fileName = fileName.substr(slashIndex + 1, fileName.length() - slashIndex);
		}
		ID3D11ShaderResourceView* srv;
		std::string filePath = std::string(TEXTURES_DIRECTORY_NAME) + path;
		std::string extension = fileName.substr(fileName.find_last_of(".") + 1, fileName.length() - fileName.find_last_of("."));
		std::wstring pathWS = std::wstring(filePath.begin(), filePath.end());

		HRESULT hr = S_FALSE;

		if (extension == "dds")
		{
			hr = CreateDDSTextureFromFile(_device.Get(), pathWS.c_str(), nullptr, &srv);
		}
		else
		{
			//hr = CreateWICTextureFromFile(_device.Get(), _deviceContext.Get(), pathWS.c_str(), nullptr, &srv);
			hr = CreateWICTextureFromFileEx(_device.Get(), _deviceContext.Get(),
				pathWS.c_str(), 0, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE,
				0, 0, WIC_LOADER_IGNORE_SRGB | WIC_LOADER_MAKE_SQUARE, nullptr, &srv);
		}

		/*if (FAILED(hr))
		{
			MessageBox(nullptr, L"Texture couldn't be loaded", L"Error!", MB_ICONERROR | MB_OK);
		}*/

		_loadedTextureFiles.insert(std::make_pair(fileName, srv));
	}

	void ResourceManager::LoadCubeMapTextureFile(std::string fileName)
	{
		UINT slashIndex = fileName.find_last_of("/\\");
		if (slashIndex != std::string::npos)
		{
			// 파일 경로를 잘라서 파일 이름 자체만 들고 있는다.
			fileName = fileName.substr(slashIndex + 1, fileName.length() - slashIndex);
		}
		else
		{
			fileName = fileName;
		}

		EnvMapInfo envMapInfo;

		ID3D11ShaderResourceView* skyboxTexture;
		std::string path = std::string(CUBEMAPS_DIRECTORY_NAME) + fileName;
		std::string extension = fileName.substr(fileName.find_last_of(".") + 1, fileName.length() - fileName.find_last_of("."));
		std::wstring pathWS = std::wstring(path.begin(), path.end());

		HRESULT hr = S_FALSE;

		if (extension == "dds")
		{
			hr = CreateDDSTextureFromFile(_device.Get(), pathWS.c_str(), nullptr, &skyboxTexture);
			envMapInfo.cubeMapTexture.shaderResourceView = skyboxTexture;
		}
		else
		{
			hr = CreateWICTextureFromFile(_device.Get(), _deviceContext.Get(), pathWS.c_str(), nullptr, &skyboxTexture);

			D3D11_TEXTURE2D_DESC textureDesc;
			ZeroMemory(&textureDesc, sizeof(textureDesc));
			textureDesc.Width = cubeMapSize;
			textureDesc.Height = cubeMapSize;
			textureDesc.MipLevels = 1;
			textureDesc.ArraySize = 6;
			textureDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
			textureDesc.SampleDesc.Count = 1;
			textureDesc.Usage = D3D11_USAGE_DEFAULT;
			textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
			textureDesc.CPUAccessFlags = 0;
			textureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

			HR(_device->CreateTexture2D(&textureDesc, nullptr, envMapInfo.cubeMapTexture.texture.GetAddressOf()));

			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
			srvDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
			srvDesc.TextureCube.MostDetailedMip = 0;
			srvDesc.TextureCube.MipLevels = 1;

			HR(_device->CreateShaderResourceView(envMapInfo.cubeMapTexture.texture.Get(), &srvDesc, envMapInfo.cubeMapTexture.shaderResourceView.GetAddressOf()));

			D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
			rtvDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
			rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
			rtvDesc.Texture2DArray.MipSlice = 0;
			rtvDesc.Texture2DArray.ArraySize = 1;
			for (UINT i = 0; i < 6; ++i)
			{
				rtvDesc.Texture2DArray.FirstArraySlice = i;
				ID3D11RenderTargetView* rtv;
				HR(_device->CreateRenderTargetView(envMapInfo.cubeMapTexture.texture.Get(), &rtvDesc, &rtv));
				envMapInfo.cubeMapTexture.renderTargetViews.push_back(rtv);
			}

			D3D11_VIEWPORT viewport{
				.TopLeftX = 0.0f,
				.TopLeftY = 0.0f,
				.Width = cubeMapSize,
				.Height = cubeMapSize,
				.MinDepth = 0.0f,
				.MaxDepth = 1.0f
			};
			_deviceContext->RSSetViewports(1, &viewport);

			VertexShader* vs = GetVertexShader("CubeMapVertexShader.cso");
			PixelShader* ps = GetPixelShader("RectToCubeMapPS.cso");

			for (UINT i = 0; i < 6; ++i)
			{
				_deviceContext->OMSetRenderTargets(1, envMapInfo.cubeMapTexture.renderTargetViews[i].GetAddressOf(), nullptr);
				XMVECTOR eyePos{ 0, 0, 0, 0 };
				XMVECTOR lookAt = forward[i];
				XMVECTOR upVec = up[i];
				float nearZ = 0.0f;
				float farZ = 10.0f;
				float viewWidth = 2.0f;
				float viewHeight = 2.0f;

				XMMATRIX viewMatrix = XMMatrixLookAtLH(eyePos, lookAt, upVec);
				XMMATRIX projMatrix = XMMatrixOrthographicLH(viewWidth, viewHeight, nearZ, farZ);
				XMMATRIX viewProj = viewMatrix * projMatrix;

				vs->SetMatrix4x4("world", XMMatrixIdentity());
				vs->SetMatrix4x4("viewProjection", XMMatrixTranspose(viewProj));
				vs->CopyAllBufferData();
				vs->SetShader();

				ps->SetShaderResourceView("CubeMap", skyboxTexture);
				ps->CopyAllBufferData();
				ps->SetShader();

				_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

				Mesh* mesh = GetMeshes("skybox")[0];
				mesh->BindBuffers();
				mesh->Draw();

				_deviceContext->Flush();
			}

			ID3D11ShaderResourceView* nullSRV = nullptr;
			_deviceContext->PSSetShaderResources(0, 1, &nullSRV);
			ID3D11RenderTargetView* nullRTV = nullptr;
			_deviceContext->OMSetRenderTargets(1, &nullRTV, nullptr);
		}

		/*if (FAILED(hr))
		{
			MessageBox(nullptr, L"Texture couldn't be loaded", L"Error!", MB_ICONERROR | MB_OK);
		}*/

		GenerateEnvMap(envMapInfo.envMapTexture, envMapInfo.cubeMapTexture.shaderResourceView.Get());
		GenerateEnvPreFilter(envMapInfo.envPreFilterMapTexture, envMapInfo.cubeMapTexture.shaderResourceView.Get());
		GenerateBrdfLUT(envMapInfo.brdfLUT);

		_loadedEnvMaps.insert(std::make_pair(fileName, envMapInfo));
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

	RocketCore::Graphics::EnvMapInfo& ResourceManager::GetEnvMapInfo(const std::string& fileName)
	{
		if (_loadedEnvMaps.find(fileName) == _loadedEnvMaps.end())
		{
			LoadCubeMapTextureFile(fileName);
		}
		return _loadedEnvMaps[fileName];
	}

	DirectX::SpriteFont* ResourceManager::GetDefaultFont()
	{
		return _defaultFont;
	}

	ID3D11Device* ResourceManager::GetDevice()
	{
		return _device.Get();
	}

	ID3D11DeviceContext* ResourceManager::GetDeviceContext()
	{
		return _deviceContext.Get();
	}

	std::vector<Mesh*>& ResourceManager::GetMeshes(const std::string& fileName)
	{
		std::string name = GetFileInfoKey(fileName);

		// 엔진에 저장되어 있지 않다면 빈 벡터 반환
		auto iter = _loadedFileInfo.find(name);
		if (iter == _loadedFileInfo.end())
		{
			std::vector<Mesh*> ret;
			return ret;
		}
		return iter->second.loadedMeshes;
	}

	std::vector<Material*>& ResourceManager::GetMaterials(const std::string& fileName)
	{
		std::string name = GetFileInfoKey(fileName);

		// 엔진에 저장되어 있지 않다면 빈 벡터 반환
		auto iter = _loadedFileInfo.find(name);
		if (iter == _loadedFileInfo.end())
		{
			std::vector<Material*> ret;
			return ret;
		}
		return iter->second.loadedMaterials;
	}

	ID3D11ShaderResourceView* ResourceManager::GetTexture(const std::string& fileName)
	{
		auto iter = _loadedTextureFiles.find(fileName);
		if (iter == _loadedTextureFiles.end())
		{
			return nullptr;
		}
		return iter->second;
	}

	Node* ResourceManager::GetNode(const std::string& fileName)
	{
		std::string name = GetFileInfoKey(fileName);

		auto iter = _loadedFileInfo.find(name);
		if (iter == _loadedFileInfo.end())
		{
			return nullptr;
		}
		return iter->second.node;
	}

	std::unordered_map<std::string, Animation*>& ResourceManager::GetAnimations(const std::string& fileName)
	{
		std::string name = GetFileInfoKey(fileName);

		// 엔진에 저장되어 있지 않다면 빈 맵 반환
		auto iter = _loadedFileInfo.find(name);
		if (iter == _loadedFileInfo.end())
		{
			std::unordered_map<std::string, Animation*> ret;
			return ret;
		}
		return iter->second.loadedAnimation;
	}

	Material* ResourceManager::GetLoadedMaterial(const std::string& materialName)
	{
		auto iter = _loadedMaterials.find(materialName);
		if (iter == _loadedMaterials.end())
		{
			return nullptr;
		}

		return iter->second;
	}

	std::unordered_map<std::string, Material*>& ResourceManager::GetLoadedMaterials()
	{
		return _loadedMaterials;
	}

	void ResourceManager::CreateRasterizerStates()
	{
		// Render State 중 Rasterizer State
		D3D11_RASTERIZER_DESC solidDesc;
		ZeroMemory(&solidDesc, sizeof(D3D11_RASTERIZER_DESC));
		solidDesc.FillMode = D3D11_FILL_SOLID;
		solidDesc.CullMode = D3D11_CULL_BACK;
		solidDesc.FrontCounterClockwise = false;
		solidDesc.DepthClipEnable = true;
		ID3D11RasterizerState* solid;
		HR(_device->CreateRasterizerState(&solidDesc, &solid));
		_rasterizerStates.emplace_back(solid);

		D3D11_RASTERIZER_DESC wireframeDesc;
		ZeroMemory(&wireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
		wireframeDesc.FillMode = D3D11_FILL_WIREFRAME;
		wireframeDesc.CullMode = D3D11_CULL_BACK;
		wireframeDesc.FrontCounterClockwise = false;
		wireframeDesc.DepthClipEnable = true;
		ID3D11RasterizerState* wireframe;
		HR(_device->CreateRasterizerState(&wireframeDesc, &wireframe));
		_rasterizerStates.emplace_back(wireframe);

		D3D11_RASTERIZER_DESC shadowMapDesc;
		ZeroMemory(&shadowMapDesc, sizeof(D3D11_RASTERIZER_DESC));
		shadowMapDesc.FillMode = D3D11_FILL_SOLID;
		shadowMapDesc.CullMode = D3D11_CULL_FRONT;
		shadowMapDesc.FrontCounterClockwise = false;
		shadowMapDesc.DepthClipEnable = true;
		ID3D11RasterizerState* shadowMapRS;
		HR(_device->CreateRasterizerState(&shadowMapDesc, &shadowMapRS));
		_rasterizerStates.emplace_back(shadowMapRS);

		D3D11_RASTERIZER_DESC cubeMapDesc;
		ZeroMemory(&cubeMapDesc, sizeof(D3D11_RASTERIZER_DESC));
		cubeMapDesc.FillMode = D3D11_FILL_SOLID;
		cubeMapDesc.CullMode = D3D11_CULL_NONE;
		cubeMapDesc.FrontCounterClockwise = false;
		cubeMapDesc.DepthClipEnable = true;
		ID3D11RasterizerState* cubemapRS;
		HR(_device->CreateRasterizerState(&cubeMapDesc, &cubemapRS));
		_rasterizerStates.emplace_back(cubemapRS);
	}

	ID3D11RasterizerState* ResourceManager::GetRasterizerState(eRasterizerState eState)
	{
		return _rasterizerStates[static_cast<int>(eState)];
	}

	void ResourceManager::CreateSamplerStates()
	{
		_samplerState = new SamplerState();
		_samplerState->CreateSamplerStates(_device.Get(), _deviceContext.Get());
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

		VertexShader* staticMeshShadowMapVS = new VertexShader(_device.Get(), _deviceContext.Get());
		if (staticMeshShadowMapVS->LoadShaderFile(L"Resources/Shaders/StaticMeshShadowMapVS.cso"))
			_vertexShaders.insert(std::make_pair("StaticMeshShadowMapVS.cso", staticMeshShadowMapVS));

		VertexShader* skinningMeshShadowMapVS = new VertexShader(_device.Get(), _deviceContext.Get());
		if (skinningMeshShadowMapVS->LoadShaderFile(L"Resources/Shaders/SkinningMeshShadowMapVS.cso"))
			_vertexShaders.insert(std::make_pair("SkinningMeshShadowMapVS.cso", skinningMeshShadowMapVS));

		PixelShader* shadowMapPS = new PixelShader(_device.Get(), _deviceContext.Get());
		if (shadowMapPS->LoadShaderFile(L"Resources/Shaders/ShadowMapPS.cso"))
			_pixelShaders.insert(std::make_pair("ShadowMapPS.cso", shadowMapPS));

		VertexShader* debugVertexShader = new VertexShader(_device.Get(), _deviceContext.Get());
		if (debugVertexShader->LoadShaderFile(L"Resources/Shaders/DebugVertexShader.cso"))
			_vertexShaders.insert(std::make_pair("DebugVertexShader.cso", debugVertexShader));

		PixelShader* debugPixelShader = new PixelShader(_device.Get(), _deviceContext.Get());
		if (debugPixelShader->LoadShaderFile(L"Resources/Shaders/DebugPixelShader.cso"))
			_pixelShaders.insert(std::make_pair("DebugPixelShader.cso", debugPixelShader));

		PixelShader* outlineStecilColorPS = new PixelShader(_device.Get(), _deviceContext.Get());
		if (outlineStecilColorPS->LoadShaderFile(L"Resources/Shaders/Outline_StencilColorPS.cso"))
			_pixelShaders.insert(std::make_pair("Outline_StencilColorPS.cso", outlineStecilColorPS));

		PixelShader* outlineSobelDetectionPS = new PixelShader(_device.Get(), _deviceContext.Get());
		if (outlineSobelDetectionPS->LoadShaderFile(L"Resources/Shaders/Outline_SobelDetectionPS.cso"))
			_pixelShaders.insert(std::make_pair("Outline_SobelDetectionPS.cso", outlineSobelDetectionPS));

		PixelShader* outlineFullScreenQuadPS = new PixelShader(_device.Get(), _deviceContext.Get());
		if (outlineFullScreenQuadPS->LoadShaderFile(L"Resources/Shaders/Outline_FullScreenQuadPS.cso"))
			_pixelShaders.insert(std::make_pair("Outline_FullScreenQuadPS.cso", outlineFullScreenQuadPS));

		VertexShader* cubeMapVertexShader = new VertexShader(_device.Get(), _deviceContext.Get());
		if (cubeMapVertexShader->LoadShaderFile(L"Resources/Shaders/CubeMapVertexShader.cso"))
			_vertexShaders.insert(std::make_pair("CubeMapVertexShader.cso", cubeMapVertexShader));

		PixelShader* cubeMapPixelShader = new PixelShader(_device.Get(), _deviceContext.Get());
		if (cubeMapPixelShader->LoadShaderFile(L"Resources/Shaders/CubeMapPixelShader.cso"))
			_pixelShaders.insert(std::make_pair("CubeMapPixelShader.cso", cubeMapPixelShader));

		PixelShader* rectToCubeMapPS = new PixelShader(_device.Get(), _deviceContext.Get());
		if (rectToCubeMapPS->LoadShaderFile(L"Resources/Shaders/RectToCubeMapPS.cso"))
			_pixelShaders.insert(std::make_pair("RectToCubeMapPS.cso", rectToCubeMapPS));

		PixelShader* irradiancePS = new PixelShader(_device.Get(), _deviceContext.Get());
		if (irradiancePS->LoadShaderFile(L"Resources/Shaders/IrradianceMapPS.cso"))
			_pixelShaders.insert(std::make_pair("IrradianceMapPS.cso", irradiancePS));

		PixelShader* specularPreFilterPS = new PixelShader(_device.Get(), _deviceContext.Get());
		if (specularPreFilterPS->LoadShaderFile(L"Resources/Shaders/SpecularPreFilterPS.cso"))
			_pixelShaders.insert(std::make_pair("SpecularPreFilterPS.cso", specularPreFilterPS));

		PixelShader* integrateBRDF = new PixelShader(_device.Get(), _deviceContext.Get());
		if (integrateBRDF->LoadShaderFile(L"Resources/Shaders/IntegrateBRDF.cso"))
			_pixelShaders.insert(std::make_pair("IntegrateBRDF.cso", integrateBRDF));

		VertexShader* fullScreenQuadVS = new VertexShader(_device.Get(), _deviceContext.Get());
		if (fullScreenQuadVS->LoadShaderFile(L"Resources/Shaders/FullScreenQuadVS.cso"))
			_vertexShaders.insert(std::make_pair("FullScreenQuadVS.cso", fullScreenQuadVS));

		PixelShader* fullScreenQuadPS = new PixelShader(_device.Get(), _deviceContext.Get());
		if (fullScreenQuadPS->LoadShaderFile(L"Resources/Shaders/FullScreenQuadPS.cso"))
			_pixelShaders.insert(std::make_pair("FullScreenQuadPS.cso", fullScreenQuadPS));

		PixelShader* SSAOPixelShader = new PixelShader(_device.Get(), _deviceContext.Get());
		if (SSAOPixelShader->LoadShaderFile(L"Resources/Shaders/SSAOPixelShader.cso"))
			_pixelShaders.insert(std::make_pair("SSAOPixelShader.cso", SSAOPixelShader));

		PixelShader* toneMapReinhardPS = new PixelShader(_device.Get(), _deviceContext.Get());
		if (toneMapReinhardPS->LoadShaderFile(L"Resources/Shaders/ToneMapReinhardPS.cso"))
			_pixelShaders.insert(std::make_pair("ToneMapReinhardPS.cso", toneMapReinhardPS));

		PixelShader* toneMapAcesPS = new PixelShader(_device.Get(), _deviceContext.Get());
		if (toneMapAcesPS->LoadShaderFile(L"Resources/Shaders/ToneMapAcesPS.cso"))
			_pixelShaders.insert(std::make_pair("ToneMapAcesPS.cso", toneMapAcesPS));

		PixelShader* blitPixelShader = new PixelShader(_device.Get(), _deviceContext.Get());
		if (blitPixelShader->LoadShaderFile(L"Resources/Shaders/BlitPixelShader.cso"))
			_pixelShaders.insert(std::make_pair("BlitPixelShader.cso", blitPixelShader));

		VertexShader* paricleVS = new VertexShader(_device.Get(), _deviceContext.Get());
		if (paricleVS->LoadShaderFile(L"Resources/Shaders/ParticleVertexShader.cso"))
			_vertexShaders.insert(std::make_pair("ParticleVertexShader.cso", paricleVS));

		PixelShader* pariclePS = new PixelShader(_device.Get(), _deviceContext.Get());
		if (pariclePS->LoadShaderFile(L"Resources/Shaders/ParticlePixelShader.cso"))
			_pixelShaders.insert(std::make_pair("ParticlePixelShader.cso", pariclePS));
	}

	void ResourceManager::CreatePrimitiveMeshes()
	{
		// 그리드 메쉬
		GeometryGenerator::DebugMeshData gridMesh;
		_geometryGen->CreateGrid(gridMesh);

		Mesh* _gridMesh = new Mesh(&gridMesh.Vertices[0], gridMesh.Vertices.size(), &gridMesh.Indices[0], gridMesh.Indices.size());
		_loadedFileInfo["grid"].loadedMeshes.push_back(_gridMesh);

		// 축 메쉬
		GeometryGenerator::DebugMeshData axisMesh;
		_geometryGen->CreateAxis(axisMesh);

		Mesh* _axisMesh = new Mesh(&axisMesh.Vertices[0], axisMesh.Vertices.size(), &axisMesh.Indices[0], axisMesh.Indices.size());
		_loadedFileInfo["axis"].loadedMeshes.push_back(_axisMesh);

		// 큐브맵 메쉬
		GeometryGenerator::MeshData skybox;
		_geometryGen->CreateSkybox(skybox);

		Mesh* _skybox = new Mesh(&skybox.Vertices[0], skybox.Vertices.size(), &skybox.Indices[0], skybox.Indices.size(), true);
		_loadedFileInfo["skybox"].loadedMeshes.push_back(_skybox);
		HDEngine::MaterialDesc skyboxDesc;
		skyboxDesc.materialName = "skybox";
		Material* _skyboxMat = ObjectManager::Instance().CreateMaterial(skyboxDesc);
		_skyboxMat->SetVertexShader(GetVertexShader("CubeMapVertexShader.cso"));
		_skyboxMat->SetPixelShader(GetPixelShader("CubeMapPixelShader.cso"));
		_loadedFileInfo["skybox"].loadedMaterials.push_back(_skyboxMat);

		// 큐브 모양 메쉬
		GeometryGenerator::MeshData cube;
		_geometryGen->CreateBox(10, 2, 10, cube);

		Mesh* _cube = new Mesh(&cube.Vertices[0], cube.Vertices.size(), &cube.Indices[0], cube.Indices.size(), true);
		_loadedFileInfo["primitiveCube"].loadedMeshes.push_back(_cube);
		HDEngine::MaterialDesc cubeDesc;
		cubeDesc.materialName = "cubeMaterial";
		Material* _cubeMaterial = ObjectManager::Instance().CreateMaterial(cubeDesc);
		_cubeMaterial->SetVertexShader(GetVertexShader("VertexShader.cso"));
		_cubeMaterial->SetPixelShader(GetPixelShader("PixelShader.cso"));
		_loadedFileInfo["primitiveCube"].loadedMaterials.push_back(_cubeMaterial);

		// 구체 모양 메쉬
		GeometryGenerator::MeshData sphere;
		_geometryGen->CreateSphere(2, 30, 30, sphere);

		Mesh* _sphere = new Mesh(&sphere.Vertices[0], sphere.Vertices.size(), &sphere.Indices[0], sphere.Indices.size(), true);
		_loadedFileInfo["primitiveSphere"].loadedMeshes.push_back(_sphere);
		Material* _sphereMaterial = ObjectManager::Instance().CreateMaterial(cubeDesc);
		_sphereMaterial->SetVertexShader(GetVertexShader("VertexShader.cso"));
		_sphereMaterial->SetPixelShader(GetPixelShader("PixelShader.cso"));
		_loadedFileInfo["primitiveSphere"].loadedMaterials.push_back(_sphereMaterial);

		// 쿼드 메쉬
		GeometryGenerator::MeshData quad;
		_geometryGen->CreateFullscreenQuad(quad);

		Mesh* _quad = new Mesh(&quad.Vertices[0], quad.Vertices.size(), &quad.Indices[0], quad.Indices.size(), true);
		_loadedFileInfo["primitiveQuad"].loadedMeshes.push_back(_quad);
		Material* _quadMaterial = ObjectManager::Instance().CreateMaterial(cubeDesc);
		_quadMaterial->SetVertexShader(GetVertexShader("VertexShader.cso"));
		_quadMaterial->SetPixelShader(GetPixelShader("PixelShader.cso"));
		_loadedFileInfo["primitiveQuad"].loadedMaterials.push_back(_quadMaterial);
		// 쿼드 메쉬
		/*std::vector<Vertex> quadVertices(6);
		std::vector<UINT> quadIndices(6);

		Vertex v1(XMFLOAT3{ -0.1, 0.1, 0 }, XMFLOAT2{ 0, 0 });
		quadVertices[0] = v1;
		Vertex v2(XMFLOAT3{ 0.1, 0.1, 0 }, XMFLOAT2{ 1, 0 });
		quadVertices[1] = v2;
		Vertex v3(XMFLOAT3{ -0.1, -0.1, 0 }, XMFLOAT2{ 0, 1 });
		quadVertices[2] = v3;
		Vertex v4(XMFLOAT3{ -0.1, -0.1, 0 }, XMFLOAT2{ 0, 1 });
		quadVertices[3] = v4;
		Vertex v5(XMFLOAT3{ 0.1, 0.1, 0 }, XMFLOAT2{ 1, 0 });
		quadVertices[4] = v5;
		Vertex v6(XMFLOAT3{ 0.1, -0.1, 0 }, XMFLOAT2{ 1, 1 });
		quadVertices[5] = v6;

		// 빌보드 파티클 메쉬
		std::vector<Vertex> quadVertices(6);
		std::vector<UINT> quadIndices(6);

		Vertex v1(XMFLOAT3{ -1, 1, 0 }, XMFLOAT2{ 0, 0 });
		quadVertices[0] = v1;
		Vertex v2(XMFLOAT3{ 1, 1, 0 }, XMFLOAT2{ 1, 0 });
		quadVertices[1] = v2;
		Vertex v3(XMFLOAT3{ -1, -1, 0 }, XMFLOAT2{ 0, 1 });
		quadVertices[2] = v3;
		Vertex v4(XMFLOAT3{ -1, -1, 0 }, XMFLOAT2{ 0, 1 });
		quadVertices[3] = v4;
		Vertex v5(XMFLOAT3{ 1, 1, 0 }, XMFLOAT2{ 1, 0 });
		quadVertices[4] = v5;
		Vertex v6(XMFLOAT3{ 1, -1, 0 }, XMFLOAT2{ 1, 1 });
		quadVertices[5] = v6;

		for (UINT i = 0; i < 6; ++i)
		{
			quadIndices[i] = i;
		}

		Mesh* quadMesh = new Mesh(&quadVertices[0], 6, &quadIndices[0], 6);
		_loadedFileInfo["quadMesh"].loadedMeshes.push_back(quadMesh);*/

		// 디버그 메쉬
		_cubePrimitive = GeometricPrimitive::CreateCube(_deviceContext.Get(), 1.0f, false);
		_spherePrimitive = GeometricPrimitive::CreateSphere(_deviceContext.Get(), 1.0f, 8, false, false);
		_cylinderPrimitive = GeometricPrimitive::CreateCylinder(_deviceContext.Get(), 2.0f, 1.0f, 8, false);

		// Create capsule debug mesh
		float radius = 0.5f; // 캡슐의 반지름
		float height = 1.0f; // 캡슐의 높이
		int stackCount = 5; // 수평 분할
		int sliceCount = 20; // 수직 분할

		DirectX::GeometricPrimitive::VertexCollection vertices;
		DirectX::GeometricPrimitive::IndexCollection indices;

		// 상단 반구 정점
		vertices.push_back(DirectX::VertexPositionNormalTexture{
		   DirectX::SimpleMath::Vector3{0.0f, radius + height * 0.5f, 0.0f},
		   DirectX::SimpleMath::Vector3{1.f, 1.f, 1.f},DirectX::SimpleMath::Vector2{0.f,0.f} });

		for (int i = 1; i <= stackCount; i++)
		{
			// 윗방향 벡터와의 각도
			float upTheta = DirectX::XM_PI * 0.5f * (i / static_cast<float>(stackCount));

			float xzsize = radius * sinf(upTheta);
			float ysize = radius * cosf(upTheta);

			for (int j = 0; j < sliceCount; j++)
			{
				float zTheta = DirectX::XM_PI * 2.0f * (j / static_cast<float>(sliceCount));

				float x = xzsize * sinf(zTheta);
				float y = ysize + height * 0.5f;
				float z = xzsize * cosf(zTheta);

				vertices.push_back(DirectX::VertexPositionNormalTexture{
				   DirectX::SimpleMath::Vector3{x, y, z},
				   DirectX::SimpleMath::Vector3{1.f, 1.f, 1.f},
				   DirectX::SimpleMath::Vector2{0.f,0.f} }
				);
			}
		}

		size_t middleIdx = vertices.size();

		// 하단 반구 정점
		for (int i = stackCount; i >= 1; i--)
		{
			// 윗방향 벡터와의 각도
			float upTheta = DirectX::XM_PI * 0.5f * (i / static_cast<float>(stackCount));

			float xzsize = radius * sinf(upTheta);
			float ysize = radius * cosf(upTheta);

			for (int j = 0; j < sliceCount; j++)
			{
				float zTheta = DirectX::XM_PI * 2.0f * (j / static_cast<float>(sliceCount));

				float x = xzsize * sinf(zTheta);
				float y = ysize + height * 0.5f;
				float z = xzsize * cosf(zTheta);

				vertices.push_back(DirectX::VertexPositionNormalTexture{
				   DirectX::SimpleMath::Vector3(x, -y, z),
				   DirectX::SimpleMath::Vector3{1.f, 1.f, 1.f},
				   DirectX::SimpleMath::Vector2{0.f,0.f} }
				);
			}
		}

		vertices.push_back(DirectX::VertexPositionNormalTexture{
		   DirectX::SimpleMath::Vector3{0.0f, -(radius + height * 0.5f), 0.0f},
		   DirectX::SimpleMath::Vector3{1.f, 1.f, 1.f},
				 DirectX::SimpleMath::Vector2{0.f,0.f} }
		);

		// 상단 반구 인덱스
		for (int i = 0; i < sliceCount; i++) {
			int a = 0;
			int b = 1 + i;
			int c = 1 + ((i + 1) % sliceCount);

			indices.push_back(a);
			indices.push_back(b);
			indices.push_back(c);
		}

		for (int i = 1; i < stackCount; i++) {
			for (int j = 0; j < sliceCount; j++) {
				int a = 1 + (i - 1) * sliceCount + j;
				int b = 1 + (i - 1) * sliceCount + ((j + 1) % sliceCount);
				int c = 1 + i * sliceCount + j;
				int d = 1 + i * sliceCount + ((j + 1) % sliceCount);

				indices.push_back(a);
				indices.push_back(c);
				indices.push_back(d);

				indices.push_back(a);
				indices.push_back(d);
				indices.push_back(b);
			}
		}

		// 실린더 부분 인덱스
		for (int i = 0; i < sliceCount; i++)
		{
			int a = middleIdx - sliceCount + i;
			int b = middleIdx - sliceCount + ((i + 1) % sliceCount);
			int c = middleIdx + i;
			int d = middleIdx + ((i + 1) % sliceCount);

			indices.push_back(a);
			indices.push_back(c);
			indices.push_back(d);

			indices.push_back(a);
			indices.push_back(d);
			indices.push_back(b);
		}

		// 하단 반구 인덱스
		for (int i = 1; i < stackCount; i++) {
			for (int j = 0; j < sliceCount; j++) {
				int a = middleIdx + (i - 1) * sliceCount + j;
				int b = middleIdx + (i - 1) * sliceCount + ((j + 1) % sliceCount);
				int c = middleIdx + i * sliceCount + j;
				int d = middleIdx + i * sliceCount + ((j + 1) % sliceCount);

				indices.push_back(a);
				indices.push_back(c);
				indices.push_back(d);

				indices.push_back(a);
				indices.push_back(d);
				indices.push_back(b);
			}
		}

		for (int i = 0; i < sliceCount; i++) {
			int a = vertices.size() - 1;
			int b = vertices.size() - 1 - sliceCount + i;
			int c = vertices.size() - 1 - sliceCount + ((i + 1) % sliceCount);

			indices.push_back(b);
			indices.push_back(a);
			indices.push_back(c);
		}

		_capsulePrimitive = GeometricPrimitive::CreateCustom(_deviceContext.Get(), vertices, indices);
	}

	void ResourceManager::ProcessNode(aiNode* node, const aiScene* scene)
	{
		for (UINT i = 0; i < node->mNumMeshes; ++i)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
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
		std::vector<Vertex> vertices;
		std::vector<UINT> indices;

		// Load position, normal, uv
		for (UINT i = 0; i < mesh->mNumVertices; ++i)
		{
			Vertex vertex;

			// process position
			vertex.Position.x = mesh->mVertices[i].x;
			vertex.Position.y = mesh->mVertices[i].y;
			vertex.Position.z = mesh->mVertices[i].z;

			// process normal
			vertex.Normal.x = mesh->mNormals[i].x;
			vertex.Normal.y = mesh->mNormals[i].y;
			vertex.Normal.z = mesh->mNormals[i].z;

			// process uv
			if (mesh->HasTextureCoords(i))
			{
				vertex.UV.x = mesh->mTextureCoords[0][i].x;
				vertex.UV.y = mesh->mTextureCoords[0][i].y;
			}

			vertex.Tangent.x = mesh->mTangents[i].x;
			vertex.Tangent.y = mesh->mTangents[i].y;
			vertex.Tangent.z = mesh->mTangents[i].z;

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
		_loadedFileInfo[_fileInfoKeyName].loadedMeshes.push_back(newMesh);

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

		//unitScaleFactor = 1.0f / unitScaleFactor;
		unitScaleFactor = 0.01f;
		upVec *= unitScaleFactor;
		forwardVec *= unitScaleFactor;
		rightVec *= unitScaleFactor;

		aiMatrix4x4 mat(
			rightVec.x, upVec.x, forwardVec.x, 0.0f,
			rightVec.y, upVec.y, forwardVec.y, 0.0f,
			rightVec.z, upVec.z, forwardVec.z, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);

		/*aiMatrix4x4 mat(
			rightVec.x, forwardVec.x, -upVec.x, 0.0f,
			rightVec.y, forwardVec.y, -upVec.y, 0.0f,
			rightVec.z, forwardVec.z, -upVec.z, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f); */

			// create node hierarchy
		Node* rootNode = new Node();
		DirectX::XMMATRIX rootNodeTM = AIMatrix4x4ToXMMatrix(scene->mRootNode->mTransformation * mat);
		rootNode->rootNodeInvTransform = DirectX::XMMatrixTranspose(rootNodeTM);
		//rootNode->rootNodeInvTransform = DirectX::XMMatrixInverse(nullptr, rootNodeTM);
		ReadNodeHierarchy(*rootNode, scene->mRootNode);

		_loadedFileInfo[_fileInfoKeyName].node = rootNode;

		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			HDEngine::MaterialDesc desc;
			desc.materialName = mesh->mName.C_Str();
			Material* newMaterial = ObjectManager::Instance().CreateMaterial(desc);
			for (UINT i = 0; i <= aiTextureType_UNKNOWN; ++i)
			{
				LoadMaterialTextures(material, (aiTextureType)i, scene, newMaterial);
			}
			_loadedFileInfo[_fileInfoKeyName].loadedMaterials.push_back(newMaterial);
		}
	}

	void ResourceManager::ProcessSkinningMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<VertexSkinning> vertices;
		std::vector<UINT> indices;

		// Load position, normal, uv
		for (UINT i = 0; i < mesh->mNumVertices; ++i)
		{
			VertexSkinning vertex;

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

			vertex.Tangent.x = mesh->mTangents[i].x;
			vertex.Tangent.y = mesh->mTangents[i].y;
			vertex.Tangent.z = mesh->mTangents[i].z;

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

		/*UINT metaDataCount = scene->mMetaData->mNumProperties;
		for (UINT i = 0; i < metaDataCount; ++i)
		{
			const aiString* key;
			const aiMetadataEntry* entry;
			auto res = scene->mMetaData->Get(i, key, entry);
			auto type = entry->mType;
		}*/

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

		unitScaleFactor = 0.01f / unitScaleFactor;
		upVec *= unitScaleFactor;
		forwardVec *= unitScaleFactor;
		rightVec *= unitScaleFactor;

		aiMatrix4x4 mat(
			rightVec.x, forwardVec.x, -upVec.x, 0.0f,
			rightVec.y, forwardVec.y, -upVec.y, 0.0f,
			rightVec.z, forwardVec.z, -upVec.z, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);

		// create node hierarchy
		Node* rootNode = new Node();
		DirectX::XMMATRIX rootNodeTM = AIMatrix4x4ToXMMatrix(scene->mRootNode->mTransformation * mat);
		rootNode->rootNodeInvTransform = DirectX::XMMatrixTranspose(rootNodeTM);
		ReadNodeHierarchy(*rootNode, scene->mRootNode, boneInfo);

		_loadedFileInfo[_fileInfoKeyName].node = rootNode;

		Mesh* newMesh = new Mesh(&vertices[0], vertices.size(), &indices[0], indices.size());
		_loadedFileInfo[_fileInfoKeyName].loadedMeshes.push_back(newMesh);

		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			HDEngine::MaterialDesc desc;
			desc.materialName = mesh->mName.C_Str();
			Material* newMaterial = ObjectManager::Instance().CreateMaterial(desc);
			newMaterial->SetVertexShader(GetVertexShader("SkeletonVertexShader.cso"));
			newMaterial->SetPixelShader(GetPixelShader("SkeletonPixelShader.cso"));
			for (UINT i = 0; i <= 21; ++i)
			{
				LoadMaterialTextures(material, (aiTextureType)i, scene, newMaterial);
			}
			std::string matName = mesh->mName.C_Str();
			newMaterial->SetMaterialName(matName);
			_loadedFileInfo[_fileInfoKeyName].loadedMaterials.push_back(newMaterial);
		}
	}

	void ResourceManager::LoadMaterialTextures(aiMaterial* material, aiTextureType type, const aiScene* scene, Material* outMaterial)
	{
		UINT textureCount = material->GetTextureCount(type);
		// 우선 첫번째 것만 가져오자.
		if (textureCount > 0)
		{
			textureCount = 1;
		}
		for (UINT i = 0; i < textureCount; ++i)
		{
			aiString str;
			material->GetTexture(type, i, &str);
			std::string s = std::string(str.C_Str());
			std::string fileName = s.substr(s.find_last_of("/\\") + 1, s.length() - s.find_last_of("/\\"));
			// Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
			auto iter = _loadedTextureFiles.find(fileName);
			if (iter == _loadedTextureFiles.end())
			{
				const aiTexture* embeddedTexture = scene->GetEmbeddedTexture(str.C_Str());
				if (embeddedTexture != nullptr)
				{
					ID3D11ShaderResourceView* texture = LoadEmbeddedTexture(embeddedTexture);
					_loadedTextureFiles.insert(std::make_pair(fileName, texture));
				}
				else
				{
					LoadTextureFile(fileName);
					switch (type)
					{
						case aiTextureType_NONE:
							break;
						case aiTextureType_DIFFUSE:
						{
							ID3D11ShaderResourceView* albedoTex = GetTexture(fileName);
							outMaterial->SetAlbedoMap(albedoTex, fileName);
							break;
						}
						case aiTextureType_SPECULAR:
							break;
						case aiTextureType_AMBIENT:
							break;
						case aiTextureType_EMISSIVE:
							break;
						case aiTextureType_HEIGHT:
							break;
						case aiTextureType_NORMALS:
						{
							ID3D11ShaderResourceView* normalTex = GetTexture(fileName);
							outMaterial->SetNormalMap(normalTex, fileName);
							break;
						}
						case aiTextureType_SHININESS:
							break;
						case aiTextureType_OPACITY:
							break;
						case aiTextureType_DISPLACEMENT:
							break;
						case aiTextureType_LIGHTMAP:
							break;
						case aiTextureType_REFLECTION:
							break;
						case aiTextureType_BASE_COLOR:
							break;
						case aiTextureType_NORMAL_CAMERA:
							break;
						case aiTextureType_EMISSION_COLOR:
							break;
						case aiTextureType_METALNESS:
						{
							ID3D11ShaderResourceView* metallicTex = GetTexture(fileName);
							outMaterial->SetMetallicMap(metallicTex, fileName);
							break;
						}
						case aiTextureType_DIFFUSE_ROUGHNESS:
						{
							ID3D11ShaderResourceView* roughnessTex = GetTexture(fileName);
							outMaterial->SetRoughnessMap(roughnessTex, fileName);
							break;
						}
						case aiTextureType_AMBIENT_OCCLUSION:
							break;
						case aiTextureType_SHEEN:
							break;
						case aiTextureType_CLEARCOAT:
							break;
						case aiTextureType_TRANSMISSION:
							break;
						case aiTextureType_UNKNOWN:
							break;
						default:
							break;
					}
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
		/*if (FAILED(hr))
		{
			MessageBox(nullptr, L"Texture couldn't be loaded", L"Error!", MB_ICONERROR | MB_OK);
		}*/

		return texture;
	}

	void ResourceManager::ReadNodeHierarchy(Node& nodeOutput, aiNode* node)
	{
		nodeOutput.name = node->mName.C_Str();
		nodeOutput.nodeTransformOffset = AIMatrix4x4ToXMMatrix(node->mTransformation);

		for (int i = 0; i < node->mNumChildren; ++i)
		{
			Node child;
			ReadNodeHierarchy(child, node->mChildren[i]);
			nodeOutput.children.push_back(child);
		}
	}

	void ResourceManager::ReadNodeHierarchy(Node& nodeOutput, aiNode* node, std::unordered_map<std::string, std::pair<int, DirectX::XMMATRIX>>& boneInfo)
	{
		if (boneInfo.find(node->mName.C_Str()) != boneInfo.end())
		{
			nodeOutput.name = node->mName.C_Str();
			nodeOutput.nodeTransformOffset = AIMatrix4x4ToXMMatrix(node->mTransformation);

			Bone bone;
			bone.id = boneInfo[nodeOutput.name].first;
			bone.offset = boneInfo[nodeOutput.name].second;

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

	void ResourceManager::LoadAnimation(const aiScene* scene, std::string animationName)
	{
		// channel in animation contains aiNodeAnim (aiNodeAnim its transformation for bones)
		// numChannels == numBones
		UINT animCount = scene->mNumAnimations;
		for (UINT i = 0; i < animCount; ++i)
		{
			const aiAnimation* animation = scene->mAnimations[i];
			Animation* newAnimation = new Animation();
			//newAnimation->animName = _fileName.substr(0, _fileName.find_last_of('.'));
			//newAnimation->animName = animationName;
			if (animationName == "")
			{
				newAnimation->animName = animation->mName.C_Str();
			}
			else
			{
				newAnimation->animName = animationName;
			}
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
			_loadedFileInfo[_fileInfoKeyName].loadedAnimation.insert(std::make_pair(newAnimation->animName, newAnimation));
		}
	}

	void ResourceManager::GenerateEnvMap(Texture& envMapTexture, ID3D11ShaderResourceView* cubeMapSRV)
	{
		D3D11_TEXTURE2D_DESC textureDesc;
		ZeroMemory(&textureDesc, sizeof(textureDesc));
		textureDesc.Width = cubeMapSize;
		textureDesc.Height = cubeMapSize;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 6;	// 6 faces
		textureDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

		HR(_device->CreateTexture2D(&textureDesc, nullptr, envMapTexture.texture.GetAddressOf()));

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
		srvDesc.TextureCube.MostDetailedMip = 0;
		srvDesc.TextureCube.MipLevels = 1;

		HR(_device->CreateShaderResourceView(envMapTexture.texture.Get(), &srvDesc, envMapTexture.shaderResourceView.GetAddressOf()));

		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
		rtvDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
		rtvDesc.Texture2DArray.MipSlice = 0;
		rtvDesc.Texture2DArray.ArraySize = 1;
		for (UINT i = 0; i < 6; ++i)
		{
			rtvDesc.Texture2DArray.FirstArraySlice = i;
			ID3D11RenderTargetView* rtv;
			HR(_device->CreateRenderTargetView(envMapTexture.texture.Get(), &rtvDesc, &rtv));
			envMapTexture.renderTargetViews.push_back(rtv);
		}

		D3D11_VIEWPORT viewport
		{
			.TopLeftX = 0.0f,
			.TopLeftY = 0.0f,
			.Width = cubeMapSize,
			.Height = cubeMapSize,
			.MinDepth = 0.0f,
			.MaxDepth = 1.0f
		};
		_deviceContext->RSSetViewports(1, &viewport);

		VertexShader* vs = GetVertexShader("CubeMapVertexShader.cso");
		PixelShader* ps = GetPixelShader("IrradianceMapPS.cso");

		for (int i = 0; i < 6; ++i)
		{
			_deviceContext->OMSetRenderTargets(1, envMapTexture.renderTargetViews[i].GetAddressOf(), nullptr);
			XMVECTOR eyePos{ 0, 0, 0, 0 };
			XMVECTOR lookAt = forward[i];
			XMVECTOR upVec = up[i];
			float nearZ = 0.0f;
			float farZ = 10.0f;
			float viewWidth = 2.0f;
			float viewHeight = 2.0f;

			XMMATRIX viewMatrix = XMMatrixLookAtLH(eyePos, lookAt, upVec);
			XMMATRIX projMatrix = XMMatrixOrthographicLH(viewWidth, viewHeight, nearZ, farZ);
			XMMATRIX viewProj = viewMatrix * projMatrix;

			vs->SetMatrix4x4("world", XMMatrixIdentity());
			vs->SetMatrix4x4("viewProjection", XMMatrixTranspose(viewProj));
			vs->CopyAllBufferData();
			vs->SetShader();

			ps->SetShaderResourceView("CubeMap", cubeMapSRV);
			ps->CopyAllBufferData();
			ps->SetShader();

			_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			Mesh* mesh = GetMeshes("skybox")[0];
			mesh->BindBuffers();
			mesh->Draw();

			_deviceContext->Flush();
		}

		ID3D11ShaderResourceView* nullSRV = nullptr;
		_deviceContext->PSSetShaderResources(0, 1, &nullSRV);
		ID3D11RenderTargetView* nullRTV = nullptr;
		_deviceContext->OMSetRenderTargets(1, &nullRTV, nullptr);
	}

	void ResourceManager::GenerateEnvPreFilter(Texture& envPreFilterMap, ID3D11ShaderResourceView* cubeMapSRV)
	{
		D3D11_TEXTURE2D_DESC textureDesc;
		ZeroMemory(&textureDesc, sizeof(textureDesc));
		textureDesc.Width = cubeMapSize;
		textureDesc.Height = cubeMapSize;
		textureDesc.MipLevels = 6;
		textureDesc.ArraySize = 6;	// 6 faces
		textureDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

		HR(_device->CreateTexture2D(&textureDesc, nullptr, envPreFilterMap.texture.GetAddressOf()));

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
		srvDesc.TextureCube.MostDetailedMip = 0;
		srvDesc.TextureCube.MipLevels = 1;

		HR(_device->CreateShaderResourceView(envPreFilterMap.texture.Get(), &srvDesc, envPreFilterMap.shaderResourceView.GetAddressOf()));

		for (UINT mipSlice = 0; mipSlice < 6; ++mipSlice)
		{
			D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
			rtvDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
			rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
			rtvDesc.Texture2DArray.MipSlice = mipSlice;
			rtvDesc.Texture2DArray.ArraySize = 1;
			for (UINT i = 0; i < 6; ++i)
			{
				rtvDesc.Texture2DArray.FirstArraySlice = i;
				ID3D11RenderTargetView* rtv;
				HR(_device->CreateRenderTargetView(envPreFilterMap.texture.Get(), &rtvDesc, &rtv));
				envPreFilterMap.renderTargetViews.push_back(rtv);
			}
		}

		VertexShader* vs = GetVertexShader("CubeMapVertexShader.cso");
		PixelShader* ps = GetPixelShader("SpecularPreFilterPS.cso");

		int mapSize = cubeMapSize;

		for (UINT i = 0; i < 6; ++i)
		{
			float roughness = (float)i / 5.0;
			ps->SetFloat("roughnessValue", roughness);
			D3D11_VIEWPORT viewport
			{
				.TopLeftX = 0.0f,
				.TopLeftY = 0.0f,
				.Width = (float)mapSize,
				.Height = (float)mapSize,
				.MinDepth = 0.0f,
				.MaxDepth = 1.0f
			};
			_deviceContext->RSSetViewports(1, &viewport);

			for (UINT j = 0; j < 6; ++j)
			{
				_deviceContext->OMSetRenderTargets(1, envPreFilterMap.renderTargetViews[i * 6 + j].GetAddressOf(), nullptr);
				XMVECTOR eyePos{ 0, 0, 0, 0 };
				XMVECTOR lookAt = forward[j];
				XMVECTOR upVec = up[j];
				float nearZ = 0.0f;
				float farZ = 10.0f;
				float viewWidth = 2.0f;
				float viewHeight = 2.0f;

				XMMATRIX viewMatrix = XMMatrixLookAtLH(eyePos, lookAt, upVec);
				XMMATRIX projMatrix = XMMatrixOrthographicLH(viewWidth, viewHeight, nearZ, farZ);
				XMMATRIX viewProj = viewMatrix * projMatrix;
				XMMATRIX transposeViewProj = (viewProj);

				vs->SetMatrix4x4("world", XMMatrixIdentity());
				vs->SetMatrix4x4("viewProjection", XMMatrixTranspose(viewProj));
				vs->CopyAllBufferData();
				vs->SetShader();

				ps->SetShaderResourceView("CubeMap", cubeMapSRV);
				ps->CopyAllBufferData();
				ps->SetShader();

				_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

				Mesh* mesh = GetMeshes("skybox")[0];
				mesh->BindBuffers();
				mesh->Draw();
			}
			mapSize /= 2;
		}

		ID3D11ShaderResourceView* nullSRV = nullptr;
		_deviceContext->PSSetShaderResources(0, 1, &nullSRV);
		ID3D11RenderTargetView* nullRTV = nullptr;
		_deviceContext->OMSetRenderTargets(1, &nullRTV, nullptr);
	}

	void ResourceManager::GenerateBrdfLUT(Texture& brdfLUT)
	{
		D3D11_TEXTURE2D_DESC textureDesc;
		ZeroMemory(&textureDesc, sizeof(textureDesc));
		textureDesc.Width = cubeMapSize;
		textureDesc.Height = cubeMapSize;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		HR(_device->CreateTexture2D(&textureDesc, nullptr, &(brdfLUT.texture)));

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;

		HR(_device->CreateShaderResourceView(brdfLUT.texture.Get(), &srvDesc, brdfLUT.shaderResourceView.GetAddressOf()));

		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
		rtvDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		rtvDesc.Texture2D.MipSlice = 0;
		ID3D11RenderTargetView* rtv;
		HR(_device->CreateRenderTargetView(brdfLUT.texture.Get(), &rtvDesc, &rtv));
		brdfLUT.renderTargetViews.push_back(rtv);

		VertexShader* vs = GetVertexShader("FullScreenQuadVS.cso");
		PixelShader* ps = GetPixelShader("IntegrateBRDF.cso");

		D3D11_VIEWPORT viewport
		{
			.TopLeftX = 0.0f,
			.TopLeftY = 0.0f,
			.Width = cubeMapSize,
			.Height = cubeMapSize,
			.MinDepth = 0.0f,
			.MaxDepth = 1.0f
		};
		_deviceContext->RSSetViewports(1, &viewport);
		_deviceContext->OMSetRenderTargets(1, brdfLUT.renderTargetViews[0].GetAddressOf(), nullptr);

		vs->SetShader();
		ps->SetShader();

		_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		_deviceContext->Draw(4, 0);

		ID3D11ShaderResourceView* nullSRV = nullptr;
		_deviceContext->PSSetShaderResources(0, 1, &nullSRV);
	}

	DirectX::DX11::GeometricPrimitive* ResourceManager::GetCubePrimitive()
	{
		return _cubePrimitive.get();
	}

	DirectX::DX11::GeometricPrimitive* ResourceManager::GetSpherePrimitive()
	{
		return _spherePrimitive.get();
	}

	DirectX::DX11::GeometricPrimitive* ResourceManager::GetCylinderPrimitive()
	{
		return _cylinderPrimitive.get();
	}

	DirectX::DX11::GeometricPrimitive* ResourceManager::GetCapsulePrimitive()
	{
		return _capsulePrimitive.get();
	}

	std::string ResourceManager::GetFileInfoKey(std::string fileName)
	{
		std::string name = fileName;

		// 혹시 경로까지 넣었을 경우에 경로를 빼준다.
		UINT slashIndex = name.find_last_of("/\\");
		if (slashIndex != std::string::npos)
		{
			// 파일 경로를 잘라서 파일 이름 자체만 들고 있는다.
			name = name.substr(slashIndex + 1, name.length() - slashIndex);
		}

		// 확장자를 자른다.
		UINT dotIndex = name.find_last_of(".");
		if (dotIndex != std::string::npos)
		{
			// SKM_TP_X_Breathing 이나 SM_Plane 과 같은 형태로 남긴다.
			name = name.substr(0, dotIndex);
		}

		UINT firstBarIndex = name.find_first_of("_");
		if (firstBarIndex == std::string::npos)
		{
			return name;
		}

		std::string meshType = name.substr(0, firstBarIndex);
		name = name.substr(firstBarIndex + 1, name.length() - firstBarIndex);
		if (meshType == "SM")
		{
			return name;
		}

		UINT firstBarIndex2 = name.find_first_of("_");
		name = name.substr(0, firstBarIndex2);
		return name;
	}

	DirectX::SpriteFont* ResourceManager::SetFont(const std::string& str)
	{
		std::wstring wstr(str.begin(), str.end());
		const wchar_t* wchars = wstr.c_str();
		DirectX::SpriteFont* font = new DirectX::SpriteFont(_device.Get(), wchars);

		return font;

		//return _defaultFont = new DirectX::SpriteFont(_device.Get(), L"Resources/Font/KRAFTON_FontIncludeKR_40.spritefont");
	}

}
