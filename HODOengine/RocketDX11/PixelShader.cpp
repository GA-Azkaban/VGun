#include <sstream>
#include <fstream>
#include <vector>
#include <DirectXMath.h>

#include "PixelShader.h"
#include "RocketMacroDX11.h"

namespace RocketCore::Graphics
{
	struct MatrixBufferType
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};

	PixelShader::PixelShader()
	{

	}

	PixelShader::~PixelShader()
	{
		_inputLayout.Reset();
		_matrixBuffer.Reset();
		_pixelShader.Reset();
	}

	void PixelShader::CreateShader(ID3D11Device* device, const std::string& path)
	{
		std::ifstream psFile(path, std::ios::binary);
		std::vector<char> psData = { std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>() };

		device->CreatePixelShader(psData.data(), psData.size(), nullptr, &_pixelShader);

		// Create the vertex input layout.
		D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			//{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
			{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		D3D11_BUFFER_DESC matrixBufferDesc;
		matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
		matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		matrixBufferDesc.MiscFlags = 0;
		matrixBufferDesc.StructureByteStride = 0;

		HR(device->CreateBuffer(&matrixBufferDesc, NULL, &_matrixBuffer));

		device->CreateInputLayout(vertexDesc, 2, psData.data(), psData.size(), &_inputLayout);
	}

	ID3D11PixelShader* PixelShader::GetPixelShader() const
	{
		return _pixelShader.Get();
	}

	ID3D11Buffer* PixelShader::GetMatrixBuffer() const
	{
		return _matrixBuffer.Get();
	}

	ID3D11InputLayout* PixelShader::GetInputLayout() const
	{
		return _inputLayout.Get();
	}

}
