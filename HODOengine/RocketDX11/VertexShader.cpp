#include <sstream>
#include <fstream>
#include <vector>
#include <DirectXMath.h>

#include "VertexShader.h"
#include "RocketMacroDX11.h"
#include "GraphicsStruct.h"

namespace RocketCore::Graphics
{
	VertexShader::VertexShader()
		: _vertexDesc(nullptr)
	{

	}

	VertexShader::~VertexShader()
	{
		_inputLayout.Reset();
		_matrixBuffer.Reset();
		_vertexShader.Reset();
	}

	void VertexShader::Initialize(ID3D11Device* device, const std::string& path)
	{
		CreateShaderAndInputLayout(device, path);
		CreateMatrixBuffer(device);
	}

	ID3D11VertexShader* VertexShader::GetVertexShader() const
	{
		return _vertexShader.Get();
	}

	ID3D11Buffer* VertexShader::GetMatrixBuffer() const
	{
		return _matrixBuffer.Get();
	}

	ID3D11InputLayout* VertexShader::GetInputLayout() const
	{
		return _inputLayout.Get();
	}

	ID3D11Buffer** VertexShader::GetAddressOfMatrixBuffer()
	{
		return _matrixBuffer.GetAddressOf();
	}

	void VertexShader::CreateShaderAndInputLayout(ID3D11Device* device, const std::string& path)
	{
		std::ifstream vsFile(path, std::ios::binary);
		std::vector<char> vsData = { std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>() };

		device->CreateVertexShader(vsData.data(), vsData.size(), nullptr, &_vertexShader);

// 		// Create the vertex input layout.
// 		D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
// 		{
// 			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
// 			{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
// 		};

		assert(_vertexDesc != nullptr);

		HR(device->CreateInputLayout(_vertexDesc, 2, vsData.data(), vsData.size(), &_inputLayout));
	}

	void VertexShader::CreateMatrixBuffer(ID3D11Device* device)
	{
		D3D11_BUFFER_DESC matrixBufferDesc;
		matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
		matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		matrixBufferDesc.MiscFlags = 0;
		matrixBufferDesc.StructureByteStride = 0;

		HR(device->CreateBuffer(&matrixBufferDesc, NULL, &_matrixBuffer));
	}

	void VertexShader::SetVertexDesc(D3D11_INPUT_ELEMENT_DESC* desc)
	{
		_vertexDesc = desc;
	}

}
