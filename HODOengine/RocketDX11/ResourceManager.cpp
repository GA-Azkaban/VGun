#include "ResourceManager.h"
#include "Camera.h"
#include "CubeMesh.h"
#include "VertexShader.h"
#include "PixelShader.h"

namespace RocketCore::Graphics
{
	ResourceManager::ResourceManager()
	{

	}

	void ResourceManager::Initialize(ID3D11Device* device)
	{
		_device = device;
		_cubeMesh = new CubeMesh();
		_cubeMesh->Initialize(_device.Get());

		_vertexShader = new VertexShader();
		GetDefaultVertexShader()->CreateShader(_device.Get(), "../x64/Debug/VertexShader.cso");

		_pixelShader = new PixelShader();
		GetDefaultPixelShader()->CreateShader(_device.Get(), "../x64/Debug/PixelShader.cso");
	}

	VertexShader* ResourceManager::GetDefaultVertexShader()
	{
		return _vertexShader;
	}

	PixelShader* ResourceManager::GetDefaultPixelShader()
	{
		return _pixelShader;
	}

	RocketCore::Graphics::CubeMesh* ResourceManager::GetCubeMesh()
	{
		return _cubeMesh;
	}

}
