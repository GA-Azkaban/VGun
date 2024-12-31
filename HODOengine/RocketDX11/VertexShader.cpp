#include <sstream>
#include <fstream>
#include <vector>
#include <DirectXMath.h>

#include "VertexShader.h"
#include "RocketMacroDX11.h"

namespace RocketCore::Graphics
{
	VertexShader::VertexShader(ID3D11Device* device, ID3D11DeviceContext* context)
		:IShader(device, context), _shader(nullptr), _inputLayout(nullptr)
	{

	}

	VertexShader::~VertexShader()
	{
		CleanUp();
	}

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
			shaderBlob->GetBufferSize(), 0, &_shader);
		if (hr != S_OK)
		{
			return false;
		}

		if (_inputLayout != nullptr)
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
			shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), &_inputLayout));

		// All done, clean up
		refl->Release();
		return true;
	}

	// Sets the vertex shader, input layout and constant buffers
	// for future DirectX drawing
	void VertexShader::SetShaderAndConstantBuffers()
	{
		// Set tha shader and input layout
		deviceContext->IASetInputLayout(_inputLayout);
		deviceContext->VSSetShader(_shader, 0, 0);

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
		if (_shader)
		{
			_shader->Release();
			_shader = nullptr;
		}
		if (_inputLayout)
		{
			_inputLayout->Release();
			_inputLayout = nullptr;
		}
	}
}
