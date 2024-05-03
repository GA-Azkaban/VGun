#pragma once
#include <d3d11.h>
#include <wrl.h>
#include <DirectXMath.h>

namespace RocketCore::Graphics
{
	class Mesh;
	class Material;
	class VertexShader;
	class PixelShader;

	class BillboardObject
	{		
	public:
		BillboardObject();
		~BillboardObject();

		void Update(float deltaTime);
		void Render();

	private:
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> _rasterizerState;
		Mesh* _mesh;
		Material* _material;
		VertexShader* _vertexShader;
		PixelShader* _pixelShader;

		DirectX::XMMATRIX _world;
		bool _isActive;
	};

}
