#pragma once
#include <d3d11.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <wrl.h>

#include "StaticMesh.h"

using Microsoft::WRL::ComPtr;

namespace RocketCore::Graphics
{
	class CubeMesh : public StaticMesh
	{
		struct Vertex
		{
			DirectX::XMFLOAT3 Pos;
			DirectX::XMFLOAT4 Color;
		};

	public:
		CubeMesh();
		~CubeMesh();

	public:
		virtual void Initialize(ID3D11Device* device) override;

	private:
		void BuildGeometryBuffers(ID3D11Device* device);

	private:
		ComPtr<ID3D11Buffer> _vertexBuffer;
		ComPtr<ID3D11Buffer> _indexBuffer;
	};
}
