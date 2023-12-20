#pragma once
#include "IShader.h"

/// <summary>
/// Derived class for VERTEX shaders
/// </summary>

class VertexShader : public IShader
{
public:
	VertexShader(ID3D11Device* device, ID3D11DeviceContext* context);
	virtual ~VertexShader();
	ID3D11VertexShader* GetVertexShader() { return shader; }
	ID3D11InputLayout* GetInputLayout() { return inputLayout; }
	virtual void SetShaderResourceView(std::string name, ID3D11ShaderResourceView* srv) override;
	virtual void SetSamplerState(std::string name, ID3D11SamplerState* samplerState) override;

protected:
	virtual bool CreateShader(ID3DBlob* shaderBlob) override;
	virtual void SetShaderAndConstantBuffers() override;
	virtual void CleanUp() override;

private:
	ID3D11VertexShader* shader;
	ID3D11InputLayout* inputLayout;
};


