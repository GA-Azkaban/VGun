#pragma once
#include <d3d11.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <wrl.h>

#include "Mesh.h"
#include "GraphicsStruct.h"

using Microsoft::WRL::ComPtr;

namespace RocketCore::Graphics
{
	class CubeMesh : public Mesh
	{
	public:
		CubeMesh();
		~CubeMesh();

	public:
		virtual void Initialize(ID3D11Device* device) override;

	private:
		void BuildGeometryBuffers(ID3D11Device* device);
	};
}
