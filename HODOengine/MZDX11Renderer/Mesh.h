#pragma once
#include <d3d11.h>
#include "Vertex.h"
#include "GeometryGenerator.h"

class Mesh
{
public:
	Mesh(VertexStruct::Vertex* vertexArray, int vertexNum, unsigned int* indexArray, int indicesNum, ID3D11Device* device);
	Mesh(VertexStruct::PosColor* vertexArray, int vertexNum, unsigned int* indexArray, int indicesNum, ID3D11Device* device);
	Mesh(const char* objFile, ID3D11Device* device);
	~Mesh();

	ID3D11Buffer* GetVertexBuffer() { return m_vertexBuffer; }
	ID3D11Buffer* GetIndexBuffer() { return m_indexBuffer; }
	int GetIndexCount() { return m_numIndices; }

private:
	void CalculateTangents(VertexStruct::Vertex* vertex, int vertexNum, unsigned int* indices, int indicesNum);
	void CreateBuffers(VertexStruct::Vertex* vertexArray, int vertexNum, unsigned int* indexArray, int indicesNum, ID3D11Device* device);
	void CreateBuffers(VertexStruct::PosColor* vertexArray, int vertexNum, unsigned int* indexArray, int indicesNum, ID3D11Device* device);

private:
	ID3D11Buffer* m_vertexBuffer;
	ID3D11Buffer* m_indexBuffer;
	int m_numIndices;
};

