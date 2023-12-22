#pragma once
#include <d3d11_2.h>
#include <dxgi1_3.h>
#include <wrl.h>
#include <unordered_map>

#include "Singleton.h"

using Microsoft::WRL::ComPtr;

namespace RocketCore::Graphics
{
	class CubeMesh;
	class VertexShader;
	class PixelShader;

	class ResourceManager : public Singleton<ResourceManager>
	{
		friend Singleton;
	private:
		ResourceManager();

	public:
		void Initialize(ID3D11Device* device);

		CubeMesh* GetCubeMesh();
		VertexShader* GetDefaultVertexShader();
		PixelShader* GetDefaultPixelShader();
	private:
		ComPtr<ID3D11Device> _device;

		// 큐브 메쉬는 기본적으로 들고있게 했음
		CubeMesh* _cubeMesh;

		// 우선 기본 셰이더들 들고있음
		VertexShader* _vertexShader;
		PixelShader* _pixelShader;
		

		// TODO : map을 이용해서 Path를 key로 각각의 리소스를 value 로 해서 관리해서 넣어줘야하려나?
	};
}
