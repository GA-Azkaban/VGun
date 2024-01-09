#include "ShaderManager.h"
#include "VertexShader.h"
#include "PixelShader.h"

LazyObjects<ShaderManager> ShaderManager::Instance;

ShaderManager::ShaderManager()
	: vertexShader(0), pixelShader(0), 
	debugVertexShader(0), debugPixelShader(0),
	dirVertexShader(0), dirPixelShader(0)
{

}

ShaderManager::~ShaderManager()
{

}

void ShaderManager::LoadShaders(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	vertexShader = new VertexShader(device, deviceContext);
	if (vertexShader->LoadShaderFile(L"../Shaders/VertexShader.cso"))
		m_loadedVertexShaders.insert(std::make_pair("VertexShader.cso", vertexShader));

	pixelShader = new PixelShader(device, deviceContext);
	if (pixelShader->LoadShaderFile(L"../Shaders/PixelShader.cso"))
		m_loadedPixelShaders.insert(std::make_pair("PixelShader.cso", pixelShader));

	debugVertexShader = new VertexShader(device, deviceContext);
	if (debugVertexShader->LoadShaderFile(L"../Shaders/DebugVertexShader.cso"))
		m_loadedVertexShaders.insert(std::make_pair("DebugVertexShader.cso", debugVertexShader));

	debugPixelShader = new PixelShader(device, deviceContext);
	if (debugPixelShader->LoadShaderFile(L"../Shaders/DebugPixelShader.cso"))
		m_loadedPixelShaders.insert(std::make_pair("DebugPixelShader.cso", debugPixelShader));

	dirVertexShader = new VertexShader(device, deviceContext);
	if (dirVertexShader->LoadShaderFile(L"../Shaders/DirVertexShader.cso"))
		m_loadedVertexShaders.insert(std::make_pair("DirVertexShader.cso", dirVertexShader));

	dirPixelShader = new PixelShader(device, deviceContext);
	if (dirPixelShader->LoadShaderFile(L"../Shaders/DirPixelShader.cso"))
		m_loadedPixelShaders.insert(std::make_pair("DirPixelShader.cso", dirPixelShader));
}

VertexShader* ShaderManager::GetVertexShader(const std::string& fileName)
{
	if (m_loadedVertexShaders.find(fileName) != m_loadedVertexShaders.end())
		return m_loadedVertexShaders[fileName];

	return nullptr;
}

PixelShader* ShaderManager::GetPixelShader(const std::string& fileName)
{
	if (m_loadedPixelShaders.find(fileName) != m_loadedPixelShaders.end())
		return m_loadedPixelShaders[fileName];

	return nullptr;
}
