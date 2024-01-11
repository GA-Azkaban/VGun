#include "ResourceManager.h"
#include "Camera.h"
#include "CubeMesh.h"
#include "Model.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "TextRenderer.h"

namespace RocketCore::Graphics
{
	ResourceManager::ResourceManager()
	{

	}

	void ResourceManager::Initialize(ID3D11Device* device)
	{
		_device = device;
		_cubeModel = new Model();
		_cubeModel->AddMesh(new CubeMesh());
		_cubeModel->Initialize(device);
		_cubeModel->LoadTexture(device, L"../Resource/darkbrickdxt1.dds");

		_defaultFont = new DirectX::SpriteFont(_device.Get(), L"..\\Font\\NotoSansKR.spritefont");

		VertexShader* colorVS = new VertexShader();
		D3D11_INPUT_ELEMENT_DESC colorDesc[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		colorVS->SetVertexDesc(colorDesc);
		colorVS->Initialize(_device.Get(), "../x64/Debug/ColorVS.cso");
		colorVS->SetVertexType(VertexType::COLOR_VERTEX);
		_vertexShaders["ColorVS"] = colorVS;

		PixelShader* colorPS = new PixelShader();
		colorPS->Initialize(_device.Get(), "../x64/Debug/ColorPS.cso");
		_pixelShaders["ColorPS"] = colorPS;

		VertexShader* textureVS = new VertexShader();
		D3D11_INPUT_ELEMENT_DESC textureDesc[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		textureVS->SetVertexDesc(textureDesc);
		textureVS->Initialize(_device.Get(), "../x64/Debug/TextureVS.cso");
		textureVS->SetVertexType(VertexType::TEXTURE_VERTEX);
		_vertexShaders["TextureVS"] = textureVS;

		PixelShader* texturePS = new PixelShader();
		texturePS->Initialize(_device.Get(), "../x64/Debug/TexturePS.cso");
		_pixelShaders["TexturePS"] = texturePS;
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

	DirectX::SpriteFont* ResourceManager::GetDefaultFont()
	{
		return _defaultFont;
	}

	Model* ResourceManager::GetCubeModel()
	{
		return _cubeModel;
	}

}
