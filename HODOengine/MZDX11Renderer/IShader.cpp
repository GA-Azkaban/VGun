#include "IShader.h"

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
			ShaderVariable varStruct;
			varStruct.ConstantBufferIndex = b;
			varStruct.ByteOffset = varDesc.StartOffset;
			varStruct.Size = varDesc.Size;

			// Get a string version
			std::string varName(varDesc.Name);
			
			// Add this variable to the table and the constant buffer
			variableTable.insert(std::pair<std::string, ShaderVariable>(varName, varStruct));
			constantBuffers[b].Variables.push_back(varStruct);
		}
	}

	// All set
	shaderRefl->Release();
	return true;
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
