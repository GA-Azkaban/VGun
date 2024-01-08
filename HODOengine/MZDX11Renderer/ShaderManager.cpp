#include "ShaderManager.h"
#include "VertexShader.h"
#include "PixelShader.h"

LazyObjects<ShaderManager> ShaderManager::Instance;

ShaderManager::ShaderManager()
	: vertexShader(0), pixelShader(0)
{

}

ShaderManager::~ShaderManager()
{

}

void ShaderManager::LoadShaders(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	vertexShader = new VertexShader(device, deviceContext);
	if (!vertexShader->LoadShaderFile(L"../x64/Debug/Shaders/VertexShader.cso"))
		vertexShader->LoadShaderFile(L"VertexShader.cso");

	pixelShader = new PixelShader(device, deviceContext);
	if (!pixelShader->LoadShaderFile(L"../x64/Debug/Shaders/PixelShader.cso"))
		pixelShader->LoadShaderFile(L"PixelShader.cso");

	debugVertexShader = new VertexShader(device, deviceContext);
	if (!debugVertexShader->LoadShaderFile(L"../x64/Debug/Shaders/DebugVertexShader.cso"))
		debugVertexShader->LoadShaderFile(L"DebugVertexShader.cso");

	debugPixelShader = new PixelShader(device, deviceContext);
	if (!debugPixelShader->LoadShaderFile(L"../x64/Debug/Shaders/DebugPixelShader.cso"))
		debugPixelShader->LoadShaderFile(L"DebugPixelShader.cso");

	dirVertexShader = new VertexShader(device, deviceContext);
	if (!dirVertexShader->LoadShaderFile(L"../x64/Debug/Shaders/DirVertexShader.cso"))
		dirVertexShader->LoadShaderFile(L"DirVertexShader.cso");

	dirPixelShader = new PixelShader(device, deviceContext);
	if (!dirPixelShader->LoadShaderFile(L"../x64/Debug/Shaders/DirPixelShader.cso"))
		dirPixelShader->LoadShaderFile(L"DirPixelShader.cso");
}
