#include "GeometryShader.h"

namespace RocketCore::Graphics
{

	GeometryShader::GeometryShader(ID3D11Device* device, ID3D11DeviceContext* context, bool useStreamOut /* = false */, bool allowStreamOutRasterization /* = false */)
		: IShader(device, context), _shader(nullptr), _useStreamOut(useStreamOut), _allowStreamOutRasterization(allowStreamOutRasterization)
	{

	}

	GeometryShader::~GeometryShader()
	{
		CleanUp();
	}

	ID3D11GeometryShader* GeometryShader::GetGeometryShader()
	{
		return _shader;
	}

	void GeometryShader::SetShaderResourceView(std::string name, ID3D11ShaderResourceView* srv)
	{
		// Look for the variable and verify
		const SRVInfo* srvInfo = GetShaderResourceViewInfo(name);
		if (srvInfo == nullptr)
			return;

		// Set the shader resource view
		deviceContext->GSSetShaderResources(srvInfo->BindIndex, 1, &srv);
	}

	void GeometryShader::SetSamplerState(std::string name, ID3D11SamplerState* samplerState)
	{
		// Look for the variable and verify
		const SamplerInfo* sampInfo = GetSamplerInfo(name);
		if (sampInfo == nullptr)
			return;

		// Set the shader resource view
		deviceContext->GSSetSamplers(sampInfo->BindIndex, 1, &samplerState);
	}

	bool GeometryShader::CreateCompatibleStreamOutBuffer(ID3D11Buffer** buffer, int vertexCount)
	{
		// Was stream output actually used?
		if (!_useStreamOut || _streamOutVertexSize == 0)
			return false;

		// Set up the buffer description
		D3D11_BUFFER_DESC desc;
		desc.BindFlags = D3D11_BIND_STREAM_OUTPUT | D3D11_BIND_VERTEX_BUFFER;
		desc.ByteWidth = _streamOutVertexSize * vertexCount;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;

		// Attempt to create the buffer and return the result
		HRESULT result = device->CreateBuffer(&desc, 0, buffer);
		return (result == S_OK);
	}

	void GeometryShader::UnbindStreamOutStage(ID3D11DeviceContext* deviceContext)
	{
		unsigned int offset = 0;
		ID3D11Buffer* unset[1] = { 0 };
		deviceContext->SOSetTargets(1, unset, &offset);
	}

	bool GeometryShader::CreateShader(ID3DBlob* shaderBlob)
	{
		CleanUp();

		if (_useStreamOut)
		{
			return CreateShaderWithStreamOut(shaderBlob);
		}

		HRESULT result = device->CreateGeometryShader(shaderBlob->GetBufferPointer(),
			shaderBlob->GetBufferSize(), 0, &_shader);

		return result == S_OK;
	}

	void GeometryShader::SetShaderAndConstantBuffers()
	{
		deviceContext->GSSetShader(_shader, 0, 0);

		for (unsigned int i = 0; i < constantBufferCount; ++i)
		{
			deviceContext->GSSetConstantBuffers(constantBuffers[i].BindIndex, 1,
				&constantBuffers[i].ConstantBuffer);
		}
	}

	void GeometryShader::CleanUp()
	{
		IShader::CleanUp();
		if (_shader)
		{
			_shader->Release();
			_shader = nullptr;
		}
	}

	// --------------------------------------------------------
	// Creates the DirectX Geometry shader and sets it up for
	// stream output, if possible.
	//
	// shaderBlob - The shader's compiled code
	//
	// Returns true if shader is created correctly, false otherwise
	// --------------------------------------------------------
	bool GeometryShader::CreateShaderWithStreamOut(ID3DBlob* shaderBlob)
	{
		CleanUp();

		// Reflect shader info
		ID3D11ShaderReflection* refl;
		D3DReflect(
			shaderBlob->GetBufferPointer(),
			shaderBlob->GetBufferSize(),
			IID_ID3D11ShaderReflection,
			(void**)&refl);

		// Get shader info
		D3D11_SHADER_DESC shaderDesc;
		refl->GetDesc(&shaderDesc);

		// Set up the output signature
		_streamOutVertexSize = 0;
		std::vector<D3D11_SO_DECLARATION_ENTRY> soDecl;
		for (unsigned int i = 0; i < shaderDesc.OutputParameters; i++)
		{
			// Get the info about this entry
			D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
			refl->GetOutputParameterDesc(i, &paramDesc);

			// Create the SO Declaration
			D3D11_SO_DECLARATION_ENTRY entry;
			entry.SemanticIndex = paramDesc.SemanticIndex;
			entry.SemanticName = paramDesc.SemanticName;
			entry.Stream = paramDesc.Stream;
			entry.StartComponent = 0; // Assume starting at 0
			entry.OutputSlot = 0; // Assume the first output slot

			// Check the mask to determine how many components are used
			entry.ComponentCount = CalcComponentCount(paramDesc.Mask);

			// Increment the size
			_streamOutVertexSize += entry.ComponentCount * sizeof(float);

			// Add to the declaration
			soDecl.push_back(entry);
		}

		// Rasterization allowed?
		unsigned int rast = _allowStreamOutRasterization ? 0 : D3D11_SO_NO_RASTERIZED_STREAM;

		// Create the shader
		HRESULT result = device->CreateGeometryShaderWithStreamOutput(
			shaderBlob->GetBufferPointer(), // Shader blob pointer
			shaderBlob->GetBufferSize(),    // Shader blob size
			&soDecl[0],                     // Stream out declaration
			soDecl.size(),                  // Number of declaration entries
			NULL,                           // Buffer strides (not used - assume tightly packed?)
			0,                              // No buffer strides
			rast,                           // Index of the stream to rasterize (if any)
			NULL,                           // Not using class linkage
			&_shader);

		return (result == S_OK);
	}

	unsigned int GeometryShader::CalcComponentCount(unsigned int mask)
	{
		unsigned int result = 0;
		result += (unsigned int)((mask & 1) == 1);
		result += (unsigned int)((mask & 2) == 2);
		result += (unsigned int)((mask & 4) == 4);
		result += (unsigned int)((mask & 8) == 8);
		return result;
	}

}
