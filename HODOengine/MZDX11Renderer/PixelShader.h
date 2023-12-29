#pragma once
#include "IShader.h"

/// <summary>
/// Derived class for PIXEL shaders
/// </summary>
class PixelShader : public IShader
{
public:
	PixelShader(ID3D11Device* device, ID3D11DeviceContext* context);
	~PixelShader();
	ID3D11PixelShader* GetPixelShader() { return shader; }

	virtual void SetShaderResourceView(std::string name, ID3D11ShaderResourceView* srv) override;
	virtual void SetSamplerState(std::string name, ID3D11SamplerState* samplerState) override;

protected:
	virtual bool CreateShader(ID3DBlob* shaderBlob) override;
	virtual void SetShaderAndConstantBuffers() override;
	virtual void CleanUp() override;

protected:
	ID3D11PixelShader* shader;
};

