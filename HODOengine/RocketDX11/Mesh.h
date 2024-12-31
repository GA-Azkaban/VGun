#pragma once
#include <d3d11.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <wrl.h>

#include "IResource.h"
#include "GraphicsStruct.h"

using Microsoft::WRL::ComPtr;

namespace RocketCore::Graphics
{
	class Mesh : public IResource
	{
	public:
		Mesh() {};
		virtual void Initialize(ID3D11Device* device);

		int GetVertexCount() const;
		int GetIndexCount() const;

		ID3D11Buffer* GetVertexBuffer() const;
		ID3D11Buffer* GetIndexBuffer() const;
		ID3D11Buffer** GetAddressOfVertexBuffer();
		ID3D11Buffer** GetAddressOfIndexBuffer();

		void SetVertexType(VertexType type);
		VertexType GetVertexType() const;

	protected:
		ComPtr<ID3D11Buffer> _vertexBuffer;
		ComPtr<ID3D11Buffer> _indexBuffer;
		int vertexCount;
		int indexCount;
		VertexType _vertexType;


		/// 2024.01.15 김민정
	public:
		Mesh(PosColor* vertexArray, int vertexNum, unsigned int* indexArray, int indicesNum);
		Mesh(Vertex* vertexArray, int vertexNum, unsigned int* indexArray, int indicesNum, bool tangentCalculated = false);
		Mesh(VertexSkinning* vertexArray, int vertexNum, unsigned int* indexArray, int indicesNum);
		Mesh(const char* objFile);
		~Mesh();

		UINT GetSingleVertexSize() { return m_singleVertexSize; }

		void BindBuffers();
		void Draw();
		void DrawInstanced(UINT instanceCount);

	private:
		void CalculateTangents(Vertex* vertex, int vertexNum, unsigned int* indices, int indicesNum);
		void CalculateTangents(VertexSkinning* vertex, int vertexNum, unsigned int* indices, int indicesNum);

		void CreateBuffers(PosColor* vertexArray, int vertexNum, unsigned int* indexArray, int indicesNum);
		void CreateBuffers(Vertex* vertexArray, int vertexNum, unsigned int* indexArray, int indicesNum);
		void CreateBuffers(VertexSkinning* vertexArray, int vertexNum, unsigned int* indexArray, int indicesNum);

	private:
		ComPtr<ID3D11Device> m_device;
		ComPtr<ID3D11DeviceContext> m_deviceContext;
		ComPtr<ID3D11Buffer> m_vertexBuffer;
		ComPtr<ID3D11Buffer> m_indexBuffer;
		UINT m_singleVertexSize;
		int m_numIndices;
	};
}
