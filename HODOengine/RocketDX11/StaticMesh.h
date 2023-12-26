#pragma once
#include <d3d11.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <wrl.h>

#include "IResource.h"

using Microsoft::WRL::ComPtr;

namespace RocketCore::Graphics
{
	class StaticMesh : public IResource
	{
	public:
		virtual void Initialize(ID3D11Device* device) override;

		int GetVertexCount() const;
		int GetIndexCount() const;

		ID3D11Buffer* GetVertexBuffer() const;
		ID3D11Buffer* GetIndexBuffer() const;
		ID3D11Buffer** GetAddressOfVertexBuffer();
		ID3D11Buffer** GetAddressOfIndexBuffer();

	protected:
		ComPtr<ID3D11Buffer> _vertexBuffer;
		ComPtr<ID3D11Buffer> _indexBuffer;
		int vertexCount;
		int indexCount;
	};
}