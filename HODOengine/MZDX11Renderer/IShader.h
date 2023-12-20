#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <cassert>
#include <vector>
#include <unordered_map>
#include "ShaderStruct.h"

/// <summary>
/// Base abstract class for simplifying shader handling
/// </summary>

class IShader
{
public:
	IShader(ID3D11Device* device, ID3D11DeviceContext* context);
	virtual ~IShader();

	bool LoadShaderFile(LPCWSTR shaderFile);

	virtual void SetShaderResourceView(std::string name, ID3D11ShaderResourceView* srv) = 0;
	virtual void SetSamplerState(std::string name, ID3D11SamplerState* samplerState) = 0;

	const SRVInfo* GetShaderResourceViewInfo(std::string name);
	const SRVInfo* GetShaderResourceViewInfo(unsigned int index);
	unsigned int GetShaderResourceViewCount() { return textureTable.size(); }

	const SamplerInfo* GetSamplerInfo(std::string name);
	const SamplerInfo* GetSamplerInfo(unsigned int index);
	unsigned int GetSamplerCount() { return samplerTable.size(); };

protected:
	virtual bool CreateShader(ID3DBlob* shaderBlob) = 0;
	virtual void SetShaderAndConstantBuffers() = 0;

	virtual void CleanUp();

protected:
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	ID3DBlob* shaderBlob;

	unsigned int constantBufferCount;

	ConstantBufferInfo* constantBuffers;
	std::vector<SRVInfo*> shaderResourceViews;
	std::vector<SamplerInfo*> samplerStates;
	std::unordered_map<std::string, ConstantBufferInfo*> constantBufferTable;
	std::unordered_map<std::string, ShaderVariable> variableTable;
	std::unordered_map<std::string, SRVInfo*> textureTable;
	std::unordered_map<std::string, SamplerInfo*> samplerTable;
};