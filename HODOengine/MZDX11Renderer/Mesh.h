#pragma once
#include "DX11Define.h"
#include "Vertex.h"
#include "GeometryGenerator.h"
#include "Animation.h"
#include <unordered_map>
#include <string>

class Mesh
{
public:
	Mesh(VertexStruct::PosColor* vertexArray, int vertexNum, unsigned int* indexArray, int indicesNum);
	Mesh(VertexStruct::Vertex* vertexArray, int vertexNum, unsigned int* indexArray, int indicesNum);
	Mesh(VertexStruct::VertexSkinning* vertexArray, int vertexNum, unsigned int* indexArray, int indicesNum);
	Mesh(const char* objFile);
	~Mesh();

	UINT GetSingleVertexSize() { return m_singleVertexSize; }

	void BindBuffers();
	void Draw();

private:
	void CalculateTangents(VertexStruct::Vertex* vertex, int vertexNum, unsigned int* indices, int indicesNum);
	void CalculateTangents(VertexStruct::VertexSkinning* vertex, int vertexNum, unsigned int* indices, int indicesNum);
	
	void CreateBuffers(VertexStruct::PosColor* vertexArray, int vertexNum, unsigned int* indexArray, int indicesNum);
	void CreateBuffers(VertexStruct::Vertex* vertexArray, int vertexNum, unsigned int* indexArray, int indicesNum);
	void CreateBuffers(VertexStruct::VertexSkinning* vertexArray, int vertexNum, unsigned int* indexArray, int indicesNum);

private:
	ComPtr<ID3D11Device> m_device;
	ComPtr<ID3D11DeviceContext> m_deviceContext;
	ComPtr<ID3D11Buffer> m_vertexBuffer;
	ComPtr<ID3D11Buffer> m_indexBuffer;
	UINT m_singleVertexSize;
	int m_numIndices;
};

