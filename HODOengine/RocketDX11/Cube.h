#pragma once
#include <d3d11.h>
#include <dxgi.h>
#include <DirectXMath.h>

namespace RocketCore::Graphics
{
	class Cube
	{
		struct Vertex
		{
			DirectX::XMFLOAT3 Pos;
			DirectX::XMFLOAT4 Color;
		};

		struct MatrixBufferType
		{
			DirectX::XMMATRIX world;
			DirectX::XMMATRIX view;
			DirectX::XMMATRIX projection;
		};

	public:
		Cube();
		~Cube();

	public:
		void Initialize();
		void Update(const DirectX::XMMATRIX& world, const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& proj);
		void Render();

	private:
		void BuildGeometryBuffers();

	private:
		ID3D11Device* _device;						// D3D11 디바이스
		ID3D11DeviceContext* _deviceContext;		// 디바이스 컨텍스트

		ID3D11Buffer* _vertexBuffer;
		ID3D11Buffer* _indexBuffer;

		ID3D11InputLayout* _inputLayout;

		DirectX::XMMATRIX _world;	// Transform Matrix
		DirectX::XMMATRIX _view;
		DirectX::XMMATRIX _proj;

		// 와이어로 그리기 위한 RenderState. 쉐이더에서 해도 된다.
		// 예제에서 On/Off로 바꾸는 등의 일을 하는 경우 이것을 바꿔줘도 될 것 같다.
		// 쉐이더에서 하는 경우도 스위칭 비용도 줄일 수 있는 방법은 많다.
		ID3D11RasterizerState* _renderstate;;

		ID3D11VertexShader* _vertexShader;
		ID3D11PixelShader* _pixelShader;
		ID3D11Buffer* _matrixBuffer;

		void CreateShader();
	};
}
