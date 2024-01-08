#pragma once
#include <d3d11.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <wrl.h>

#include "IResource.h"

using Microsoft::WRL::ComPtr;

namespace RocketCore::Graphics
{
	class Model : public IResource
	{
		virtual void Initialize(ID3D11Device* device) override;
	};
}
