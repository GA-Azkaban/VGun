#pragma once
#include <d3d11_2.h>
#include <dxgi1_3.h>
#include <wrl.h>
#include <unordered_map>
#include <DXTK/SpriteFont.h>
#include "string"

#include "Singleton.h"

using Microsoft::WRL::ComPtr;

namespace RocketCore::Graphics
{
	class Model;
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

		Model* GetCubeModel();
		VertexShader* GetVertexShader(const std::string& name);
		PixelShader* GetPixelShader(const std::string& name);
		DirectX::SpriteFont* GetDefaultFont();
		ImageRenderer* GetImage();

	public:
		ID3D11Device* GetDevice();
		ID3D11DeviceContext* GetDeviceContext();

	private:
		ComPtr<ID3D11Device> _device;
		ComPtr<ID3D11DeviceContext> _deviceContext;

		// 기본 큐브 모델
		Model* _cubeModel;

		// 기본 폰트 들고있음
		DirectX::SpriteFont* _defaultFont;

		std::unordered_map<std::string, VertexShader*> _vertexShaders;
		std::unordered_map<std::string, PixelShader*> _pixelShaders;
	};
}
