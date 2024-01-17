#include "IShader.h"

namespace RocketCore::Graphics
{
	IShader::IShader(ID3D11Device* device, ID3D11DeviceContext* context)
		:device(device), deviceContext(context), shaderBlob(nullptr)
	{

	}

	IShader::~IShader()
	{
		
	}

	// Loads the specified shader and builds the variable table using shader reflection.
	// This must be a separate step from the constructor since we can't invoke derived
	// class overrides in the base class constructor.
	// shaderFile - A "wide string" specifying the compiled shader to load
	bool IShader::LoadShaderFile(LPCWSTR shaderFile)
	{
		// Load the shader to a blob and ensure it worked
		HRESULT hr = D3DReadFileToBlob(shaderFile, &shaderBlob);
		if (hr != S_OK)
		{
			return false;
		}

		// Create the shader - Calls an overloaded version of this abstract
		// method in the appropriate child class
		if (!CreateShader(shaderBlob))
		{
			return false;
		}

		// Set up shader reflection to get information about
		// this shader and its variables, buffers, etc.
		ID3D11ShaderReflection* shaderRefl;
		D3DReflect(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(),
			IID_ID3D11ShaderReflection, (void**)&shaderRefl);

		// Get the description of the shader
		D3D11_SHADER_DESC shaderDesc;
		shaderRefl->GetDesc(&shaderDesc);

		// Create resource arrays
		constantBufferCount = shaderDesc.ConstantBuffers;
		constantBuffers = new ConstantBufferInfo[constantBufferCount];

		// Handle bound resources (like shaders and samplers)
		unsigned int resourceCount = shaderDesc.BoundResources;
		for (unsigned int r = 0; r < resourceCount; r++)
		{
			// Get this resource's description
			D3D11_SHADER_INPUT_BIND_DESC resourceDesc;
			shaderRefl->GetResourceBindingDesc(r, &resourceDesc);

			// Check the type
			switch (resourceDesc.Type)
			{
				case D3D_SIT_TEXTURE:	// A texture resource
				{
					// Create the SRV wrapper
					SRVInfo* srv = new SRVInfo();
					srv->BindIndex = resourceDesc.BindPoint;	// Shader bind point
					srv->Index = shaderResourceViews.size();	// Raw index

					textureTable.insert(std::pair<std::string, SRVInfo*>(resourceDesc.Name, srv));
					shaderResourceViews.push_back(srv);
				}
				break;

				case D3D_SIT_SAMPLER:	// A sampler resource
				{
					// Create the sampler wrapper
					SamplerInfo* sampler = new SamplerInfo();
					sampler->BindIndex = resourceDesc.BindPoint;	// Shader bind point
					sampler->Index = samplerStates.size();			// Raw index

					samplerTable.insert(std::pair<std::string, SamplerInfo*>(resourceDesc.Name, sampler));
					samplerStates.push_back(sampler);
				}
				break;
			}
		}

		// Loop through all constant buffers
		for (unsigned int b = 0; b < constantBufferCount; ++b)
		{
			// Get this buffer
			ID3D11ShaderReflectionConstantBuffer* cb =
				shaderRefl->GetConstantBufferByIndex(b);

			// Get the description of this buffer
			D3D11_SHADER_BUFFER_DESC bufferDesc;
			cb->GetDesc(&bufferDesc);

			// Get the description of the resource binding,
			// so we know exactly how it's bound in the shader
			D3D11_SHADER_INPUT_BIND_DESC bindDesc;
			shaderRefl->GetResourceBindingDescByName(bufferDesc.Name, &bindDesc);

			// Set up the buffer and put its pointer in the table
			constantBuffers[b].BindIndex = bindDesc.BindPoint;
			constantBuffers[b].Name = bufferDesc.Name;
			constantBufferTable.insert(std::pair<std::string, ConstantBufferInfo*>(bufferDesc.Name, &constantBuffers[b]));

			// Create this constant buffer
			D3D11_BUFFER_DESC constantBuffDesc
			{
				.ByteWidth = bufferDesc.Size,
				.Usage = D3D11_USAGE_DEFAULT,
				.BindFlags = D3D11_BIND_CONSTANT_BUFFER,
				.CPUAccessFlags = 0,
				.MiscFlags = 0,
				.StructureByteStride = 0,
			};
			device->CreateBuffer(&constantBuffDesc, 0, &constantBuffers[b].ConstantBuffer);

			// Set up the data buffer for this constant buffer
			constantBuffers[b].Size = bufferDesc.Size;
			constantBuffers[b].LocalDataBuffer = new unsigned char[bufferDesc.Size];
			ZeroMemory(constantBuffers[b].LocalDataBuffer, bufferDesc.Size);

			// Loop through all variables in this buffer
			for (unsigned int v = 0; v < bufferDesc.Variables; ++v)
			{
				// Get this variable
				ID3D11ShaderReflectionVariable* var =
					cb->GetVariableByIndex(v);

				// Get the description of the variable and its type
				D3D11_SHADER_VARIABLE_DESC varDesc;
				var->GetDesc(&varDesc);

				// Create the variable struct
				ShaderVariableInfo varStruct;
				varStruct.ConstantBufferIndex = b;
				varStruct.ByteOffset = varDesc.StartOffset;
				varStruct.Size = varDesc.Size;

				// Get a string version
				std::string varName(varDesc.Name);

				// Add this variable to the table and the constant buffer
				variableTable.insert(std::pair<std::string, ShaderVariableInfo>(varName, varStruct));
				constantBuffers[b].Variables.push_back(varStruct);
			}
		}

		// All set
		shaderRefl->Release();
		return true;
	}

	// Sets the shader and associated constant buffers in DirectX
	void IShader::SetShader()
	{
		// Set the shader and any relevant constant buffers, which
		// is an overrided method in a subclass
		SetShaderAndConstantBuffers();
	}

	// Copies the relevant data to the all of this
	// shader's constant buffers. To just copy one
	// buffer, use CopyBufferData()
	void IShader::CopyAllBufferData()
	{
		for (unsigned int i = 0; i < constantBufferCount; ++i)
		{
			deviceContext->UpdateSubresource(
				constantBuffers[i].ConstantBuffer, 0, 0,
				constantBuffers[i].LocalDataBuffer, 0, 0);
		}
	}

	// Copies local data to the shader's specified constant buffer
	// index - The index of the buffer to copy.
	//			Useful for updating more frequently-changing
	//			variables without having to re-copy all buffers.
	// NOTE : The "index" of the buffer might NOT be the same
	//			as its register, especially if you have buffers
	//			bound to non-sequential registers!
	void IShader::CopyBufferData(unsigned int index)
	{
		// Validate the index
		if (index >= constantBufferCount)
			return;

		// Check for the buffer
		ConstantBufferInfo* cb = &constantBuffers[index];
		if (cb == nullptr)
			return;

		// Copy the data
		deviceContext->UpdateSubresource(
			cb->ConstantBuffer, 0, 0,
			cb->LocalDataBuffer, 0, 0);
	}

	void IShader::CopyBufferData(std::string bufferName)
	{
		// Check for the buffer
		ConstantBufferInfo* cb = GetConstantBufferInfo(bufferName);
		if (cb == nullptr)
			return;

		// Copy the data
		deviceContext->UpdateSubresource(
			cb->ConstantBuffer, 0, 0,
			cb->LocalDataBuffer, 0, 0);
	}

	void IShader::SetData(std::string name, const void* data, unsigned int size)
	{
		ShaderVariableInfo* var = GetVariableInfo(name, size);
		if (var == nullptr)
		{
			return;
		}

		memcpy(constantBuffers[var->ConstantBufferIndex].LocalDataBuffer + var->ByteOffset,
			data, size);
	}

	void IShader::SetInt(std::string name, int data)
	{
		SetData(name, (void*)(&data), sizeof(int));
	}

	void IShader::SetFloat(std::string name, float data)
	{
		SetData(name, (void*)(&data), sizeof(float));
	}

	void IShader::SetFloat2(std::string name, const float data[2])
	{
		SetData(name, (void*)data, sizeof(float) * 2);
	}

	void IShader::SetFloat2(std::string name, const DirectX::XMFLOAT2& data)
	{
		SetData(name, &data, sizeof(float) * 2);
	}

	void IShader::SetFloat3(std::string name, const float data[3])
	{
		SetData(name, (void*)data, sizeof(float) * 3);
	}

	void IShader::SetFloat3(std::string name, const DirectX::XMFLOAT3& data)
	{
		SetData(name, &data, sizeof(float) * 3);
	}

	void IShader::SetFloat4(std::string name, const float data[4])
	{
		SetData(name, (void*)data, sizeof(float) * 4);
	}

	void IShader::SetFloat4(std::string name, const DirectX::XMFLOAT4& data)
	{
		SetData(name, &data, sizeof(float) * 4);
	}

	void IShader::SetMatrix4x4(std::string name, const float data[16])
	{
		SetData(name, (void*)data, sizeof(float) * 16);
	}

	void IShader::SetMatrix4x4(std::string name, const DirectX::XMFLOAT4X4& data)
	{
		SetData(name, &data, sizeof(float) * 16);
	}

	void IShader::SetMatrix4x4(std::string name, const DirectX::XMMATRIX& data)
	{
		SetData(name, &data, sizeof(float) * 16);
	}

	void IShader::SetMatrix4x4Array(std::string name, const DirectX::XMMATRIX* data, UINT size)
	{
		SetData(name, data, sizeof(float) * 16 * size);
	}

	const SRVInfo* IShader::GetShaderResourceViewInfo(std::string name)
	{
		std::unordered_map<std::string, SRVInfo*>::iterator textureTableIter =
			textureTable.find(name);

		if (textureTableIter == textureTable.end())
		{
			return nullptr;
		}

		return textureTableIter->second;
	}

	const SRVInfo* IShader::GetShaderResourceViewInfo(unsigned int index)
	{
		if (index >= shaderResourceViews.size())
		{
			return nullptr;
		}
		return shaderResourceViews[index];
	}

	const SamplerInfo* IShader::GetSamplerInfo(std::string name)
	{
		std::unordered_map<std::string, SamplerInfo*>::iterator samplerTableIter =
			samplerTable.find(name);

		if (samplerTableIter == samplerTable.end())
		{
			return nullptr;
		}

		return samplerTableIter->second;
	}

	const SamplerInfo* IShader::GetSamplerInfo(unsigned int index)
	{
		if (index >= samplerStates.size())
		{
			return nullptr;
		}
		return samplerStates[index];
	}

	void IShader::CleanUp()
	{
		for (unsigned int i = 0; i < constantBufferCount; ++i)
		{
			constantBuffers[i].ConstantBuffer->Release();
			delete[] constantBuffers[i].LocalDataBuffer;
		}

		if (constantBuffers)
		{
			delete[] constantBuffers;
			constantBufferCount = 0;
		}

		for (unsigned int i = 0; i < shaderResourceViews.size(); ++i)
		{
			delete shaderResourceViews[i];
		}

		for (unsigned int i = 0; i < samplerStates.size(); ++i)
		{
			delete samplerStates[i];
		}

		constantBufferTable.clear();
		variableTable.clear();
		textureTable.clear();
		samplerTable.clear();
	}

	ShaderVariableInfo* IShader::GetVariableInfo(std::string name, int size)
	{
		std::unordered_map<std::string, ShaderVariableInfo>::iterator variableTableIter =
			variableTable.find(name);

		if (variableTableIter == variableTable.end())
		{
			return nullptr;
		}

		ShaderVariableInfo* varInfo = &(variableTableIter->second);

		if (size < 0 || varInfo->Size != size)
		{
			return nullptr;
		}

		return varInfo;
	}

	ConstantBufferInfo* IShader::GetConstantBufferInfo(std::string name)
	{
		std::unordered_map<std::string, ConstantBufferInfo*>::iterator cbTableIter =
			constantBufferTable.find(name);

		if (cbTableIter == constantBufferTable.end())
		{
			return nullptr;
		}

		return cbTableIter->second;
	}

}