#include "PixelShader.h"
#include "ShaderStruct.h"

PixelShader::PixelShader(ID3D11Device* device, ID3D11DeviceContext* context)
	: IShader(device, context), shader(nullptr)
{

}

PixelShader::~PixelShader()
{
	CleanUp();
}

// Sets a shader resource view in the pixel shader stage
// name - The name of the texture resource in the shader
// srv - The shader resource view of the texture in GPU memory
void PixelShader::SetShaderResourceView(std::string name, ID3D11ShaderResourceView* srv)
{
	const SRVInfo* srvInfo = GetShaderResourceViewInfo(name);
	if (srvInfo == nullptr)
		return;

	// Set the shader resource view
	deviceContext->PSSetShaderResources(srvInfo->BindIndex, 1, &srv);
}

// Sets a sampler state in the pixel shader stage
// name - The name of the sampler state in the shader
// srv - The sampler state in GPU memory
void PixelShader::SetSamplerState(std::string name, ID3D11SamplerState* samplerState)
{
	const SamplerInfo* samplerInfo = GetSamplerInfo(name);
	if (samplerInfo == nullptr)
		return;

	// Set the sampler state
	deviceContext->PSSetSamplers(samplerInfo->BindIndex, 1, &samplerState);
}

// Creates the DirectX pixel shader
// shaderBlob - The shader's compiled code
bool PixelShader::CreateShader(ID3DBlob* shaderBlob)
{
	CleanUp();

	// Create the shader from the blob
	HRESULT result = device->CreatePixelShader(shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(), 0, &shader);

	return result == S_OK;
}

// Sets the pixel shader and constant buffers for future DirectX drawing
void PixelShader::SetShaderAndConstantBuffers()
{
	// Set the shader
	deviceContext->PSSetShader(shader, 0, 0);

	// Set the constant buffers
	for (unsigned int i = 0; i < constantBufferCount; ++i)
	{
		deviceContext->PSSetConstantBuffers(constantBuffers[i].BindIndex, 1,
			&constantBuffers[i].ConstantBuffer);
	}
}

void PixelShader::CleanUp()
{
	IShader::CleanUp();
	if (shader)
	{
		shader->Release();
		shader = nullptr;
	}
}
