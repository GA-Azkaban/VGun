#pragma once
#include <d3d11_2.h>
#include <dxgi1_3.h>
#include <wrl.h>
#include <unordered_map>
#include <DXTK/SpriteFont.h>

#include "Singleton.h"

using Microsoft::WRL::ComPtr;

namespace RocketCore::Graphics
{
	class CubeMesh;
	class VertexShader;
	class PixelShader;
	class ImageRenderer;

	class ResourceManager : public Singleton<ResourceManager>
	{
		friend Singleton;
	private:
		ResourceManager();

	public:
		void Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext);

		CubeMesh* GetCubeMesh();
		VertexShader* GetDefaultVertexShader();
		PixelShader* GetDefaultPixelShader();
		DirectX::SpriteFont* GetDefaultFont();
		ImageRenderer* GetImage();

	public:
		ID3D11Device* GetDevice();
		ID3D11DeviceContext* GetDeviceContext();

	private:
		ComPtr<ID3D11Device> _device;
		ComPtr<ID3D11DeviceContext> _deviceContext;

		// 큐브 메쉬는 기본적으로 들고있게 했음
		CubeMesh* _cubeMesh;

		// 기본 폰트 들고있음
		DirectX::SpriteFont* _spriteFont;

		// 이미지
		ImageRenderer* _image;

		// 우선 기본 셰이더들 들고있음
		VertexShader* _vertexShader;
		PixelShader* _pixelShader;
		

		// TODO : map을 이용해서 Path를 key로 각각의 리소스를 value 로 해서 관리해서 넣어줘야하려나?
	};
}
