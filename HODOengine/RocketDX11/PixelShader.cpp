#include <sstream>
#include <fstream>
#include <vector>
#include <DirectXMath.h>

#include "PixelShader.h"
#include "RocketMacroDX11.h"
#include "GraphicsStruct.h"

namespace RocketCore::Graphics
{
	PixelShader::PixelShader()
	{

	}

	PixelShader::~PixelShader()
	{
		_pixelShader.Reset();
	}

	void PixelShader::Initialize(ID3D11Device* device, const std::string& path)
	{
		CreatePixelShader(device, path);
	}

	ID3D11PixelShader* PixelShader::GetPixelShader() const
	{
		return _pixelShader.Get();
	}

	void PixelShader::CreatePixelShader(ID3D11Device* device, const std::string& path)
	{
		std::ifstream psFile(path, std::ios::binary);
		std::vector<char> psData = { std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>() };

		device->CreatePixelShader(psData.data(), psData.size(), nullptr, &_pixelShader);
	}

}
