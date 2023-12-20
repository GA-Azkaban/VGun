#include "StaticMesh.h"

namespace RocketCore::Graphics
{

	void StaticMesh::Initialize(ID3D11Device* device)
	{

	}

	ID3D11Buffer* StaticMesh::GetVertexBuffer() const
	{
		return _vertexBuffer.Get();
	}

	ID3D11Buffer* StaticMesh::GetIndexBuffer() const
	{
		return _indexBuffer.Get();
	}

	ID3D11Buffer** StaticMesh::GetAddressOfVertexBuffer()
	{
		return _vertexBuffer.GetAddressOf();
	}

	ID3D11Buffer** StaticMesh::GetAddressOfIndexBuffer()
	{
		return _indexBuffer.GetAddressOf();
	}

	int StaticMesh::GetVertexCount() const
	{
		return vertexCount;
	}

	int StaticMesh::GetIndexCount() const
	{
		return indexCount;
	}

}