#include <DirectXColors.h>	// 이미 많은 부분 DX11과 겹친다.
#include "CubeMesh.h"
#include "RocketMacroDX11.h"

namespace RocketCore::Graphics
{
	CubeMesh::CubeMesh()
	{

	}

	CubeMesh::~CubeMesh()
	{
		_vertexBuffer.Reset();
		_indexBuffer.Reset();
	}

	void CubeMesh::Initialize(ID3D11Device* device)
	{
		BuildGeometryBuffers(device);
	}

	void CubeMesh::BuildGeometryBuffers(ID3D11Device* device)
	{
		ColorVertex vertices[8] =
		{
			{DirectX::XMFLOAT3(-0.5f,0.5f,-0.5f), DirectX::XMFLOAT4((const float*)&DirectX::Colors::Red)},
			{DirectX::XMFLOAT3(0.5f,0.5f,-0.5f), DirectX::XMFLOAT4((const float*)&DirectX::Colors::Yellow)},
			{DirectX::XMFLOAT3(-0.5f,0.5f,0.5f), DirectX::XMFLOAT4((const float*)&DirectX::Colors::Pink)},
			{DirectX::XMFLOAT3(0.5f,0.5f,0.5f), DirectX::XMFLOAT4((const float*)&DirectX::Colors::Green)},
			{DirectX::XMFLOAT3(-0.5f,-0.5f,-0.5f), DirectX::XMFLOAT4((const float*)&DirectX::Colors::Purple)},
			{DirectX::XMFLOAT3(0.5f,-0.5f,-0.5f), DirectX::XMFLOAT4((const float*)&DirectX::Colors::Orange)},
			{DirectX::XMFLOAT3(-0.5f,-0.5f,0.5f), DirectX::XMFLOAT4((const float*)&DirectX::Colors::Blue)},
			{DirectX::XMFLOAT3(0.5f,-0.5f,0.5f), DirectX::XMFLOAT4((const float*)&DirectX::Colors::White)}
		};

		D3D11_BUFFER_DESC vbd;
		vbd.Usage = D3D11_USAGE_IMMUTABLE;    
		vbd.ByteWidth = sizeof(ColorVertex) * ARRAYSIZE(vertices);
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbd.CPUAccessFlags = 0;
		vbd.MiscFlags = 0;
		vbd.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA vinitData;
		vinitData.pSysMem = vertices;
		HR(device->CreateBuffer(&vbd, &vinitData, &_vertexBuffer));


		UINT indices[36] =
		{
			0,2,3,
			0,3,1,
			2,6,7,
			2,7,3,
			6,4,5,
			6,5,7,
			0,5,4,
			0,1,5,
			1,3,7,
			1,7,5,
			0,4,6,
			0,6,2
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

		vertexCount = ARRAYSIZE(vertices);
		indexCount = ARRAYSIZE(indices);
	}
}

