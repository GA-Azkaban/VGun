#pragma once
#include <d3d11.h>

class IShader
{
public:
	IShader(ID3D11Device* device, ID3D11DeviceContext* context);
	virtual ~IShader();

	void LoadShaderFile(LPCWSTR shaderFile);

protected:
	virtual void CreateShader(ID3DBlob* shaderBlob) = 0;

protected:
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	ID3DBlob* shaderBlob;
};