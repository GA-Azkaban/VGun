#pragma once
#include "DX11Define.h"
#include "LazyObjects.h"
#include <unordered_map>
#include <string>

class VertexShader;
class PixelShader;

class ShaderManager
{
public:
	static LazyObjects<ShaderManager> Instance;
	friend LazyObjects<ShaderManager>;
	//friend class DeferredRenderer;

	~ShaderManager();

	void LoadShaders(ID3D11Device* device, ID3D11DeviceContext* deviceContext);

	VertexShader* GetVertexShader(const std::string& fileName);
	PixelShader* GetPixelShader(const std::string& fileName);

private:
	ShaderManager();

	std::unordered_map<std::string, VertexShader*> m_loadedVertexShaders;
	std::unordered_map<std::string, PixelShader*> m_loadedPixelShaders;

public:
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

