#include "StaticMeshObject.h"
#include "..\\HODOmath\\HODOmath.h"
#include "RocketMacroDX11.h"
#include "ResourceManager.h"

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

namespace RocketCore::Graphics
{
	void StaticMeshObject::SetMesh(StaticMesh* mesh)
	{
		_mesh = mesh;
	}

	void StaticMeshObject::SetWorldTM(const HDMath::HDFLOAT4X4& worldTM)
	{
		DirectX::XMFLOAT4X4 temp;

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				temp.m[i][j] = worldTM.element[i][j];
			}
		}

		_worldTM = XMLoadFloat4x4(&temp);
	}

	void StaticMeshObject::SetActive(bool isActive)
	{

	}

	void StaticMeshObject::SetVertexShader(VertexShader* shader)
	{
		_vertexShader = shader;
	}

	void StaticMeshObject::SetPixelShader(PixelShader* shader)
	{
		_pixelShader = shader;
	}

	void StaticMeshObject::LoadMesh(const std::string& fileName)
	{
	}

	void StaticMeshObject::LoadNormalMap(const std::string& fileName)
	{
	}

	void StaticMeshObject::LoadDiffuseMap(const std::string& fileName)
	{
	}

	void StaticMeshObject::Render(ID3D11DeviceContext* deviceContext, ID3D11RasterizerState* renderstate, const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& proj)
	{
		// Grid가 쓰는 Shader deviceContext 이용해 연결.
		deviceContext->VSSetShader(_vertexShader->GetVertexShader(), nullptr, 0);
		deviceContext->PSSetShader(_pixelShader->GetPixelShader(), nullptr, 0);

		// 입력 배치 객체 셋팅
		deviceContext->IASetInputLayout(_vertexShader->GetInputLayout());
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// 인덱스버퍼와 버텍스버퍼 셋팅
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		deviceContext->IASetVertexBuffers(0, 1, _mesh->GetAddressOfVertexBuffer(), &stride, &offset);
		deviceContext->IASetIndexBuffer(_mesh->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

		D3D11_MAPPED_SUBRESOURCE mappedResource;
		MatrixBufferType* dataPtr;
		unsigned int bufferNumber;

		DirectX::XMMATRIX w;
		DirectX::XMMATRIX v;
		DirectX::XMMATRIX p;

		w = DirectX::XMMatrixTranspose(_worldTM);
		v = DirectX::XMMatrixTranspose(view);
		p = DirectX::XMMatrixTranspose(proj);

		HR(deviceContext->Map(_vertexShader->GetMatrixBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));

		dataPtr = (MatrixBufferType*)mappedResource.pData;

		dataPtr->world = w;
		dataPtr->view = v;
		dataPtr->projection = p;

		deviceContext->Unmap(_vertexShader->GetMatrixBuffer(), 0);

		bufferNumber = 0;

		deviceContext->VSSetConstantBuffers(bufferNumber, 1, _vertexShader->GetAddressOfMatrixBuffer());

		// 렌더스테이트
		deviceContext->RSSetState(renderstate);

		deviceContext->DrawIndexed(_mesh->GetIndexCount(), 0, 0);
	}

}