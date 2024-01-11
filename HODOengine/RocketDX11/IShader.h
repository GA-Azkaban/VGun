#pragma once
#include <string>
#include <d3d11.h>
#include <wrl.h>

using Microsoft::WRL::ComPtr;

namespace RocketCore::Graphics
{
	class IShader
	{
	public:
		virtual void Initialize(ID3D11Device* device, const std::string& path) = 0;
	};
}
 