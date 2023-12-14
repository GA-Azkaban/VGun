#include <DirectXColors.h>	// 이미 많은 부분 DX11과 겹친다.
#include "Cube.h"
#include "RocketMacroDX11.h"

namespace RocketCore::Graphics
{
	Cube::Cube()
		: _vertexBuffer(nullptr), _indexBuffer(nullptr),
		_world(), _view(), _proj()
	{

	}

	Cube::~Cube()
	{
		_vertexBuffer.Reset();
		_indexBuffer.Reset();
	}

	void Cube::Initialize(ID3D11Device* device)
	{
		BuildGeometryBuffers(device);
	}

	void Cube::Update(const DirectX::XMMATRIX& world, const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& proj)
	{
		_world = world;
		_view = view;
		_proj = proj;
	}

	void Cube::Render(ID3D11DeviceContext* deviceContext, ID3D11VertexShader* vertexShader, ID3D11PixelShader* pixelShader, ID3D11Buffer* matrixBuffer, ID3D11InputLayout* inputLayout, ID3D11RasterizerState* renderstate)
	{
		// Grid가 쓰는 Shader deviceContext 이용해 연결.
		deviceContext->VSSetShader(vertexShader, nullptr, 0);
		deviceContext->PSSetShader(pixelShader, nullptr, 0);

		// 입력 배치 객체 셋팅
		deviceContext->IASetInputLayout(inputLayout);
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

		// 인덱스버퍼와 버텍스버퍼 셋팅
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		deviceContext->IASetVertexBuffers(0, 1, _vertexBuffer.GetAddressOf(), &stride, &offset);
		deviceContext->IASetIndexBuffer(_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

		D3D11_MAPPED_SUBRESOURCE mappedResource;
		MatrixBufferType* dataPtr;
		unsigned int bufferNumber;

		_world = DirectX::XMMatrixTranspose(_world);
		_view = DirectX::XMMatrixTranspose(_view);
		_proj = DirectX::XMMatrixTranspose(_proj);

		HR(deviceContext->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));

		dataPtr = (MatrixBufferType*)mappedResource.pData;

		dataPtr->world = _world;
		dataPtr->view = _view;
		dataPtr->projection = _proj;

		deviceContext->Unmap(matrixBuffer, 0);

		bufferNumber = 0;

		deviceContext->VSSetConstantBuffers(bufferNumber, 1, &matrixBuffer);

		// 렌더스테이트
		deviceContext->RSSetState(renderstate);

		deviceContext->DrawIndexed(36, 0, 0);
	}

	void Cube::BuildGeometryBuffers(ID3D11Device* device)
	{
		Vertex vertices[8];

		vertices[0].Pos = DirectX::XMFLOAT3(-0.5f, 0.5f, -0.5f);
		vertices[1].Pos = DirectX::XMFLOAT3(0.5f, 0.5f, -0.5f);
		vertices[2].Pos = DirectX::XMFLOAT3(-0.5f, 0.5f, 0.5f);
		vertices[3].Pos = DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f);
		vertices[4].Pos = DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f);
		vertices[5].Pos = DirectX::XMFLOAT3(0.5f, -0.5f, -0.5f);
		vertices[6].Pos = DirectX::XMFLOAT3(-0.5f, -0.5f, 0.5f);
		vertices[7].Pos = DirectX::XMFLOAT3(0.5f, -0.5f, 0.5f);


		for (int i = 0; i < 8; i++)
		{
			vertices[i].Color = DirectX::XMFLOAT4((const float*)&DirectX::Colors::Green);
		}

		D3D11_BUFFER_DESC vbd;
		vbd.Usage = D3D11_USAGE_IMMUTABLE;
		vbd.ByteWidth = sizeof(Vertex) * 8;
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbd.CPUAccessFlags = 0;
		vbd.MiscFlags = 0;
		vbd.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA vinitData;
		vinitData.pSysMem = vertices;
		HR(device->CreateBuffer(&vbd, &vinitData, &_vertexBuffer));


		UINT indices[36] =
		{
			0,2,1,
			2,3,1,
			0,1,4,
			1,5,4,
			2,0,6,
			0,4,6,
			7,3,2,
			6,7,2,
			4,5,7,
			4,7,6,
			1,3,5,
			5,3,7
		};

		

		D3D11_BUFFER_DESC ibd;
		ibd.Usage = D3D11_USAGE_IMMUTABLE;
		ibd.ByteWidth = sizeof(UINT) * ARRAYSIZE(indices);
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags = 0;
		ibd.MiscFlags = 0;
		ibd.StructureByteStride = 0;
		D3D11_SUBRESOURCE_DATA iinitData;
		iinitData.pSysMem = indices;
		HR(device->CreateBuffer(&ibd, &iinitData, &_indexBuffer));
	}

}

