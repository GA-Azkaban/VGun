#pragma once
#include <d3d11_2.h>
#include <dxgi1_3.h>
#include <wrl.h>

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

		VertexShader* GetVertexShader();
		PixelShader* GetPixelShader();
	private:
		ComPtr<ID3D11Device> _device;

		// 큐브 메쉬는 그냥 기본적으로 들고있게 했음
		CubeMesh* _cubeMesh;

		// 우선 기본 셰이더들 들고있음
		VertexShader* _vertexShader;
		PixelShader* _pixelShader;
	};
}
