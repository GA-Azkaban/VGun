#include <sstream>
#include <fstream>
#include <vector>
#include <DirectXMath.h>

#include "VertexShader.h"
#include "RocketMacroDX11.h"

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
		CreateSamplerState(device);

		CleanUp();
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

	void VertexShader::SetVertexType(VertexType type)
	{
		_vertexType = type;
	}

	VertexType VertexShader::GetVertexType() const
	{
		return _vertexType;
	}

	void VertexShader::CreateSamplerState(ID3D11Device* device)
	{
		// 텍스처 샘플러 상태 구조체를 생성 및 설정한다.
		D3D11_SAMPLER_DESC samplerDesc;
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 1;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		// 텍스처 샘플러 상태를 만듭니다.
		HR(device->CreateSamplerState(&samplerDesc, &_sampleState));
	}

	ID3D11SamplerState** VertexShader::GetAddressOfSampleState()
	{
		return &_sampleState;
	}



	/// 2024.01.15 김민정
	VertexShader::VertexShader(ID3D11Device* device, ID3D11DeviceContext* context)
		:IShader(device, context), shader(nullptr), inputLayout(nullptr)
	{

	}

	/*VertexShader::~VertexShader()
	{
		CleanUp();
	}*/

	void VertexShader::SetShaderResourceView(std::string name, ID3D11ShaderResourceView* srv)
	{
		const SRVInfo* srvInfo = GetShaderResourceViewInfo(name);
		if (srvInfo == nullptr)
			return;

		deviceContext->VSSetShaderResources(srvInfo->BindIndex, 1, &srv);
	}

	void VertexShader::SetSamplerState(std::string name, ID3D11SamplerState* samplerState)
	{
		const SamplerInfo* samplerInfo = GetSamplerInfo(name);
		if (samplerInfo == nullptr)
			return;

		deviceContext->VSSetSamplers(samplerInfo->BindIndex, 1, &samplerState);
	}

	// Creates the DirectX vertex shader
	// shaderBlob - The shader's compiled code
	bool VertexShader::CreateShader(ID3DBlob* shaderBlob)
	{
		CleanUp();

		// Create the shader from the blob
		HRESULT hr = device->CreateVertexShader(shaderBlob->GetBufferPointer(),
			shaderBlob->GetBufferSize(), 0, &shader);
		if (hr != S_OK)
		{
			return false;
		}

		if (inputLayout != nullptr)
		{
			return true;
		}

		// Vertex shader was created successfully, so we now use the
		// shader code to re-reflect and create an input layout that
		// matches what the vertex shader expects. Code adapted from:
		// https://takinginitiative.wordpress.com/2011/12/11/directx-1011-basic-shader-reflection-automatic-input-layout-creation/

		// Reflect shader info
		ID3D11ShaderReflection* refl;
		D3DReflect(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(),
			IID_ID3D11ShaderReflection, (void**)&refl);

		// Get shader info
		D3D11_SHADER_DESC shaderDesc;
		refl->GetDesc(&shaderDesc);

		// Read input layout description from shader info
		std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;
		for (unsigned int i = 0; i < shaderDesc.InputParameters; ++i)
		{
			D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
			refl->GetInputParameterDesc(i, &paramDesc);

			// Fill out input element desc
			D3D11_INPUT_ELEMENT_DESC elementDesc
			{
				.SemanticName = paramDesc.SemanticName,
				.SemanticIndex = paramDesc.SemanticIndex,
				.InputSlot = 0,
				.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT,
				.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA,
				.InstanceDataStepRate = 0
			};

			// Determine DXGI format
			if (paramDesc.Mask == 1)
			{
				if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
					elementDesc.Format = DXGI_FORMAT_R32_UINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
					elementDesc.Format = DXGI_FORMAT_R32_SINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
					elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
			}
			else if (paramDesc.Mask <= 3)
			{
				if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
					elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
					elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
					elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
			}
			else if (paramDesc.Mask <= 7)
			{
				if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
					elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
					elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
					elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
			}
			else if (paramDesc.Mask <= 15)
			{
				if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
					elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
					elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
					elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			}

			// Save element desc
			inputLayoutDesc.push_back(elementDesc);
		}

		// Try to create input layout
		HR(device->CreateInputLayout(&inputLayoutDesc[0], inputLayoutDesc.size(),
			shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), &inputLayout));

		// All done, clean up
		refl->Release();
		return true;
	}

	// Sets the vertex shader, input layout and constant buffers
	// for future DirectX drawing
	void VertexShader::SetShaderAndConstantBuffers()
	{
		// Set tha shader and input layout
		deviceContext->IASetInputLayout(inputLayout);
		deviceContext->VSSetShader(shader, 0, 0);

		// Set the constant buffers
		for (unsigned int i = 0; i < constantBufferCount; ++i)
		{
			deviceContext->VSSetConstantBuffers(constantBuffers[i].BindIndex, 1,
				&constantBuffers[i].ConstantBuffer);
		}
	}

	void VertexShader::CleanUp()
	{
		IShader::CleanUp();
		if (shader)
		{
			shader->Release();
			shader = nullptr;
		}
		if (inputLayout)
		{
			inputLayout->Release();
			inputLayout = nullptr;
		}
	}
}
