#include "Skybox.h"
#include "MZDX11Renderer.h"
#include "MZCamera.h"
#include "ResourceManager.h"
#include "RasterizerState.h"
#include "SamplerState.h"
#include "Mesh.h"
#include "Material.h"

std::vector<Skybox*> Skybox::_skyboxObjects;

Skybox::Skybox()
	: _material(nullptr), _isActive(true)
{
	_skyboxObjects.push_back(this);
	_meshes = ResourceManager::Instance.Get().GetMeshes("skybox");
	_material = new Material(ResourceManager::Instance.Get().GetVertexShader("SkyboxVertexShader.cso"), ResourceManager::Instance.Get().GetPixelShader("SkyboxPixelShader.cso"));
}

Skybox::~Skybox()
{
	delete _material;
}

void Skybox::Update(float deltaTime)
{

}

void Skybox::Render()
{
	if (!_isActive)
		return;

	MZDX11Renderer::Instance().GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	MZDX11Renderer::Instance().GetDeviceContext()->RSSetState(RasterizerState::Instance.Get().GetSolidRS());
	
	XMFLOAT3 cameraPos = MZCamera::GetMainCamera()->GetPosition();
	XMMATRIX cameraTranslate = XMMatrixTranslation(cameraPos.x, cameraPos.y, cameraPos.z);

	XMMATRIX view = MZCamera::GetMainCamera()->View();
	XMMATRIX proj = MZCamera::GetMainCamera()->Proj();
	XMMATRIX worldViewProj = cameraTranslate * view * proj;
	XMMATRIX invWVP = XMMatrixTranspose(worldViewProj);

	VertexShader* vertexShader = _material->GetVertexShader();
	PixelShader* pixelShader = _material->GetPixelShader();

	vertexShader->SetMatrix4x4("worldViewProj", invWVP);

	vertexShader->CopyAllBufferData();
	vertexShader->SetShader();

	pixelShader->SetShaderResourceView("Texture", _material->GetTextureSRV());

	pixelShader->CopyAllBufferData();
	pixelShader->SetShader();

	for (UINT i = 0; i < _meshes.size(); ++i)
	{
		_meshes[i]->BindBuffers();
		_meshes[i]->Draw();
	}
}

void Skybox::SetMesh(const std::string& meshName)
{
	_meshes = ResourceManager::Instance.Get().GetMeshes(meshName);
}

//void Sky::SetVertexShader(const std::string& fileName)
//{
//	VertexShader* vs = ResourceManager::Instance.Get().GetVertexShader(fileName);
//	m_material->SetVertexShader(vs);
//}
//
//void Sky::SetPixelShader(const std::string& fileName)
//{
//	PixelShader* ps = ResourceManager::Instance.Get().GetPixelShader(fileName);
//	m_material->SetPixelShader(ps);
//}

void Skybox::SetCubeMapTexture(const std::string& fileName)
{
	ID3D11ShaderResourceView* diffuseTex = ResourceManager::Instance.Get().GetTexture(fileName);
	_material->SetTextureSRV(diffuseTex);
}
