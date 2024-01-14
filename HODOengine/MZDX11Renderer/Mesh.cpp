#include "Mesh.h"
#include "MZDX11Renderer.h"
using namespace DirectX;

Mesh::Mesh(VertexStruct::PosColor* vertexArray, int vertexNum, unsigned int* indexArray, int indicesNum)
	: m_device(0), m_deviceContext(0), m_singleVertexSize(0)
{
	m_device = MZDX11Renderer::Instance().GetDevice();
	m_deviceContext = MZDX11Renderer::Instance().GetDeviceContext();
	CreateBuffers(vertexArray, vertexNum, indexArray, indicesNum);
}

Mesh::Mesh(VertexStruct::Vertex* vertexArray, int vertexNum, unsigned int* indexArray, int indicesNum)
	: m_device(0), m_deviceContext(0), m_singleVertexSize(0)
{
	m_device = MZDX11Renderer::Instance().GetDevice();
	m_deviceContext = MZDX11Renderer::Instance().GetDeviceContext();
	// Calcultate the tangents before copying to buffer
	CalculateTangents(vertexArray, vertexNum, indexArray, indicesNum);
	CreateBuffers(vertexArray, vertexNum, indexArray, indicesNum);
}

Mesh::Mesh(VertexStruct::VertexSkinning* vertexArray, int vertexNum, unsigned int* indexArray, int indicesNum)
	: m_device(0), m_deviceContext(0), m_singleVertexSize(0)
{
	m_device = MZDX11Renderer::Instance().GetDevice();
	m_deviceContext = MZDX11Renderer::Instance().GetDeviceContext();
	// Calcultate the tangents before copying to buffer
	CalculateTangents(vertexArray, vertexNum, indexArray, indicesNum);
	CreateBuffers(vertexArray, vertexNum, indexArray, indicesNum);
}

Mesh::Mesh(const char* objFile)
	: m_device(0), m_deviceContext(0), m_singleVertexSize(0)
{
	m_device = MZDX11Renderer::Instance().GetDevice();
	m_deviceContext = MZDX11Renderer::Instance().GetDeviceContext();

	// TODO
}

Mesh::~Mesh()
{
	m_vertexBuffer.Reset();
	m_indexBuffer.Reset();
}

void Mesh::BindBuffers()
{
	UINT offset = 0;
	m_deviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &m_singleVertexSize, &offset);
	m_deviceContext->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
}

void Mesh::Draw()
{
	m_deviceContext->DrawIndexed(m_numIndices, 0, 0);
}

// Calculates the tangents of the vertices in a mesh
// Code adapted from: http://www.terathon.com/code/tangent.html
// https://scahp.tistory.com/13
void Mesh::CalculateTangents(VertexStruct::Vertex* vertex, int vertexNum, unsigned int* indices, int indicesNum)
{
	// Reset tangents
	for (int i = 0; i < vertexNum; ++i)
	{
		vertex[i].Tangent = DirectX::XMFLOAT3{ 0, 0, 0 };
	}

	// Calculate tangents one whole triangle at a time
	for (int i = 0; i < vertexNum;)
	{
		// Grab indices and veritces of first triangle
		unsigned int i1 = indices[i++];
		unsigned int i2 = indices[i++];
		unsigned int i3 = indices[i++];
		VertexStruct::Vertex* v1 = &vertex[i1];
		VertexStruct::Vertex* v2 = &vertex[i2];
		VertexStruct::Vertex* v3 = &vertex[i3];

		// Calculate vectors relative to triangle positions
		float x1 = v2->Position.x - v1->Position.x;
		float y1 = v2->Position.y - v1->Position.y;
		float z1 = v2->Position.z - v1->Position.z;

		float x2 = v3->Position.x - v1->Position.x;
		float y2 = v3->Position.y - v1->Position.y;
		float z2 = v3->Position.z - v1->Position.z;

		// Do the same for vectors relative to triangle uv's
		float s1 = v2->UV.x - v1->UV.x;
		float t1 = v2->UV.y - v1->UV.y;

		float s2 = v3->UV.x - v1->UV.x;
		float t2 = v3->UV.y - v1->UV.y;

		// Create vectors for tangent calculation
		float r = 1.0f / (s1 * t2 - s2 * t1);

		float tx = (t2 * x1 - t1 * x2) * r;
		float ty = (t2 * y1 - t1 * y2) * r;
		float tz = (t2 * z1 - t1 * z2) * r;

		// Adjust tangents of each vertex of the triangle
		v1->Tangent.x += tx;
		v1->Tangent.y += ty;
		v1->Tangent.z += tz;

		v2->Tangent.x += tx;
		v2->Tangent.y += ty;
		v2->Tangent.z += tz;

		v3->Tangent.x += tx;
		v3->Tangent.y += ty;
		v3->Tangent.z += tz;
	}

	// Ensure all of the tangents are orthogonal to the normals
	for (int i = 0; i < vertexNum; ++i)
	{
		// Grab the two vectors
		XMVECTOR normal = XMLoadFloat3(&vertex[i].Normal);
		XMVECTOR tangent = XMLoadFloat3(&vertex[i].Tangent);

		// Use Gram-Schmidt orthogonalize
		tangent = XMVector3Normalize(tangent - normal * XMVector3Dot(normal, tangent));

		// Store the tangent
		XMStoreFloat3(&vertex[i].Tangent, tangent);
	}
}

void Mesh::CalculateTangents(VertexStruct::VertexSkinning* vertex, int vertexNum, unsigned int* indices, int indicesNum)
{
	// Reset tangents
	for (int i = 0; i < vertexNum; ++i)
	{
		vertex[i].Tangent = DirectX::XMFLOAT3{ 0, 0, 0 };
	}

	// Calculate tangents one whole triangle at a time
	for (int i = 0; i < vertexNum;)
	{
		// Grab indices and veritces of first triangle
		unsigned int i1 = indices[i++];
		unsigned int i2 = indices[i++];
		unsigned int i3 = indices[i++];
		VertexStruct::VertexSkinning* v1 = &vertex[i1];
		VertexStruct::VertexSkinning* v2 = &vertex[i2];
		VertexStruct::VertexSkinning* v3 = &vertex[i3];

		// Calculate vectors relative to triangle positions
		float x1 = v2->Position.x - v1->Position.x;
		float y1 = v2->Position.y - v1->Position.y;
		float z1 = v2->Position.z - v1->Position.z;

		float x2 = v3->Position.x - v1->Position.x;
		float y2 = v3->Position.y - v1->Position.y;
		float z2 = v3->Position.z - v1->Position.z;

		// Do the same for vectors relative to triangle uv's
		float s1 = v2->UV.x - v1->UV.x;
		float t1 = v2->UV.y - v1->UV.y;

		float s2 = v3->UV.x - v1->UV.x;
		float t2 = v3->UV.y - v1->UV.y;

		// Create vectors for tangent calculation
		float r = 1.0f / (s1 * t2 - s2 * t1);

		float tx = (t2 * x1 - t1 * x2) * r;
		float ty = (t2 * y1 - t1 * y2) * r;
		float tz = (t2 * z1 - t1 * z2) * r;

		// Adjust tangents of each vertex of the triangle
		v1->Tangent.x += tx;
		v1->Tangent.y += ty;
		v1->Tangent.z += tz;

		v2->Tangent.x += tx;
		v2->Tangent.y += ty;
		v2->Tangent.z += tz;

		v3->Tangent.x += tx;
		v3->Tangent.y += ty;
		v3->Tangent.z += tz;
	}

	// Ensure all of the tangents are orthogonal to the normals
	for (int i = 0; i < vertexNum; ++i)
	{
		// Grab the two vectors
		XMVECTOR normal = XMLoadFloat3(&vertex[i].Normal);
		XMVECTOR tangent = XMLoadFloat3(&vertex[i].Tangent);

		// Use Gram-Schmidt orthogonalize
		tangent = XMVector3Normalize(tangent - normal * XMVector3Dot(normal, tangent));

		// Store the tangent
		XMStoreFloat3(&vertex[i].Tangent, tangent);
	}
}

void Mesh::CreateBuffers(VertexStruct::PosColor* vertexArray, int vertexNum, unsigned int* indexArray, int indicesNum)
{
	// Create the vertex buffer
	D3D11_BUFFER_DESC vbd
	{
		.ByteWidth = sizeof(VertexStruct::PosColor) * vertexNum,
		.Usage = D3D11_USAGE_IMMUTABLE,
		.BindFlags = D3D11_BIND_VERTEX_BUFFER,
		.CPUAccessFlags = 0,
		.MiscFlags = 0,
		.StructureByteStride = 0,
	};
	D3D11_SUBRESOURCE_DATA initialVertexData;
	initialVertexData.pSysMem = vertexArray;
	m_device->CreateBuffer(&vbd, &initialVertexData, m_vertexBuffer.GetAddressOf());

	// Create the index buffer
	D3D11_BUFFER_DESC ibd
	{
		.ByteWidth = sizeof(unsigned int) * indicesNum,
		.Usage = D3D11_USAGE_IMMUTABLE,
		.BindFlags = D3D11_BIND_INDEX_BUFFER,
		.CPUAccessFlags = 0,
		.MiscFlags = 0,
		.StructureByteStride = 0,
	};
	D3D11_SUBRESOURCE_DATA initialIndexData;
	initialIndexData.pSysMem = indexArray;
	m_device->CreateBuffer(&ibd, &initialIndexData, m_indexBuffer.GetAddressOf());

	// Save the indices
	m_numIndices = indicesNum;

	m_singleVertexSize = sizeof(VertexStruct::PosColor);
}

void Mesh::CreateBuffers(VertexStruct::Vertex* vertexArray, int vertexNum, unsigned int* indexArray, int indicesNum)
{
	// Create the vertex buffer
	D3D11_BUFFER_DESC vbd
	{
		.ByteWidth = sizeof(VertexStruct::Vertex) * vertexNum,
		.Usage = D3D11_USAGE_IMMUTABLE,
		.BindFlags = D3D11_BIND_VERTEX_BUFFER,
		.CPUAccessFlags = 0,
		.MiscFlags = 0,
		.StructureByteStride = 0,
	};
	D3D11_SUBRESOURCE_DATA initialVertexData;
	initialVertexData.pSysMem = vertexArray;
	m_device->CreateBuffer(&vbd, &initialVertexData, m_vertexBuffer.GetAddressOf());

	// Create the index buffer
	D3D11_BUFFER_DESC ibd
	{
		.ByteWidth = sizeof(unsigned int) * indicesNum,
		.Usage = D3D11_USAGE_IMMUTABLE,
		.BindFlags = D3D11_BIND_INDEX_BUFFER,
		.CPUAccessFlags = 0,
		.MiscFlags = 0,
		.StructureByteStride = 0,
	};
	D3D11_SUBRESOURCE_DATA initialIndexData;
	initialIndexData.pSysMem = indexArray;
	m_device->CreateBuffer(&ibd, &initialIndexData, m_indexBuffer.GetAddressOf());

	// Save the indices
	m_numIndices = indicesNum;

	m_singleVertexSize = sizeof(VertexStruct::Vertex);
}

void Mesh::CreateBuffers(VertexStruct::VertexSkinning* vertexArray, int vertexNum, unsigned int* indexArray, int indicesNum)
{
	// Create the vertex buffer
	D3D11_BUFFER_DESC vbd
	{
		.ByteWidth = sizeof(VertexStruct::VertexSkinning) * vertexNum,
		.Usage = D3D11_USAGE_IMMUTABLE,
		.BindFlags = D3D11_BIND_VERTEX_BUFFER,
		.CPUAccessFlags = 0,
		.MiscFlags = 0,
		.StructureByteStride = 0,
	};
	D3D11_SUBRESOURCE_DATA initialVertexData;
	initialVertexData.pSysMem = vertexArray;
	m_device->CreateBuffer(&vbd, &initialVertexData, m_vertexBuffer.GetAddressOf());

	// Create the index buffer
	D3D11_BUFFER_DESC ibd
	{
		.ByteWidth = sizeof(unsigned int) * indicesNum,
		.Usage = D3D11_USAGE_IMMUTABLE,
		.BindFlags = D3D11_BIND_INDEX_BUFFER,
		.CPUAccessFlags = 0,
		.MiscFlags = 0,
		.StructureByteStride = 0,
	};
	D3D11_SUBRESOURCE_DATA initialIndexData;
	initialIndexData.pSysMem = indexArray;
	m_device->CreateBuffer(&ibd, &initialIndexData, m_indexBuffer.GetAddressOf());

	// Save the indices
	m_numIndices = indicesNum;

	m_singleVertexSize = sizeof(VertexStruct::VertexSkinning);
}
