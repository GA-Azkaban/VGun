#pragma once
#include "DX11Define.h"
#include "LazyObjects.h"

class VertexShader;
class PixelShader;

class ShaderManager
{
public:
	static LazyObjects<ShaderManager> Instance;
	friend LazyObjects<ShaderManager>;
	friend class DeferredRenderer;

	~ShaderManager();

	void LoadShaders(ID3D11Device* device, ID3D11DeviceContext* deviceContext);

private:
	ShaderManager();

	VertexShader* vertexShader;
	PixelShader* pixelShader;
	VertexShader* debugVertexShader;
	PixelShader* debugPixelShader;
	/*VertexShader* deferredVertexShader;
	PixelShader* deferredPixelShader;
	VertexShader* backBufferVertexShader;
	PixelShader* backBufferPixelShader;
	VertexShader* lightVertexShader;
	PixelShader* lightPixelShader; */
	VertexShader* dirVertexShader;
	PixelShader* dirPixelShader;
};

