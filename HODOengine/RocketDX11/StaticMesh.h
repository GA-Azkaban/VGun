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

		virtual void BuildGeometryBuffers(ID3D11Device* device) = 0;

	private:
		ComPtr<ID3D11Buffer> _vertexBuffer;
		ComPtr<ID3D11Buffer> _indexBuffer;
	};
}
