#pragma once
#include "DX11Define.h"
#include "Vertex.h"
#include "GeometryGenerator.h"

class Mesh
{
public:
	Mesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, VertexStruct::Vertex* vertexArray, int vertexNum, unsigned int* indexArray, int indicesNum);
	Mesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, VertexStruct::PosColor* vertexArray, int vertexNum, unsigned int* indexArray, int indicesNum);
	Mesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, VertexStruct::VertexSkinning* vertexArray, int vertexNum, unsigned int* indexArray, int indicesNum);
	Mesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const char* objFile);
	~Mesh();

	UINT GetSingleVertexSize() { return m_singleVertexSize; }

	void BindBuffers();
	void Draw();

private:
	void CalculateTangents(VertexStruct::Vertex* vertex, int vertexNum, unsigned int* indices, int indicesNum);
	void CreateBuffers(VertexStruct::Vertex* vertexArray, int vertexNum, unsigned int* indexArray, int indicesNum, ID3D11Device* device);
	void CreateBuffers(VertexStruct::PosColor* vertexArray, int vertexNum, unsigned int* indexArray, int indicesNum, ID3D11Device* device);
	void CreateBuffers(VertexStruct::VertexSkinning* vertexArray, int vertexNum, unsigned int* indexArray, int indicesNum, ID3D11Device* device);

private:
	ComPtr<ID3D11Device> m_device;
	ComPtr<ID3D11DeviceContext> m_deviceContext;
	ComPtr<ID3D11Buffer> m_vertexBuffer;
	ComPtr<ID3D11Buffer> m_indexBuffer;
	UINT m_singleVertexSize;
	int m_numIndices;
};

