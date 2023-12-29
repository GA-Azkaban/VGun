#pragma once
#include <d3d11.h>
#include <string>
#include <vector>

// Used by shaders to store information about
// specific variables in constant buffers
struct ShaderVariableInfo
{
	unsigned int ByteOffset;
	unsigned int Size;
	unsigned int ConstantBufferIndex;
};

// Contains information about a specific
// constant buffer in a sader, as well as
// the local data buffer for it
struct ConstantBufferInfo
{
	std::string Name;
	unsigned int Size;
	unsigned int BindIndex;
	ID3D11Buffer* ConstantBuffer;
	unsigned char* LocalDataBuffer;
	std::vector<ShaderVariableInfo> Variables;
};

// Contains info about a single SRV in a shader
struct SRVInfo
{
	unsigned int Index;		// The raw index of the SRV
	unsigned int BindIndex;	// The register of the SRV
};

// Contains info about a single Sampler in a shader
struct SamplerInfo
{
	unsigned int Index;		// The raw index of the Sampler
	unsigned int BindIndex;	// The register of the Sampler
};