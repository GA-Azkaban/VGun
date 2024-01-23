#include "StaticMeshObject.h"
#include "MZDX11Renderer.h"
#include "MZCamera.h"
#include "Mesh.h"
#include "Material.h"
#include "RasterizerState.h"
#include "SamplerState.h"
#include "ResourceManager.h"

StaticMeshObject::StaticMeshObject()
	: m_material(nullptr), m_isActive(true),
	m_world{ XMMatrixIdentity() }
{
	m_material = new Material(ResourceManager::Instance.Get().GetVertexShader("VertexShader.cso"), ResourceManager::Instance.Get().GetPixelShader("PixelShader.cso"));
	m_material->SetSamplerState(SamplerState::Instance.Get().GetSamplerState());
}

StaticMeshObject::~StaticMeshObject()
{
	delete m_material;
}

void StaticMeshObject::Update(float deltaTime)
{
	
}

void StaticMeshObject::Render()
{
	if (!m_isActive)
		return;

	MZDX11Renderer::Instance().GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	MZDX11Renderer::Instance().GetDeviceContext()->RSSetState(RasterizerState::Instance.Get().GetSolidRS());
	
	XMMATRIX invWorld = XMMatrixTranspose(m_world);

	XMMATRIX view = MZCamera::GetMainCamera()->View();
	XMMATRIX proj = MZCamera::GetMainCamera()->Proj();
	XMMATRIX worldViewProj = m_world * view * proj;
	XMMATRIX invWVP = XMMatrixTranspose(worldViewProj);

	VertexShader* vertexShader = m_material->GetVertexShader();
	PixelShader* pixelShader = m_material->GetPixelShader();

	vertexShader->SetMatrix4x4("world", invWorld);
	vertexShader->SetMatrix4x4("worldViewProj", invWVP);

	vertexShader->CopyAllBufferData();
	vertexShader->SetShader();

	XMFLOAT3 cameraPos = MZCamera::GetMainCamera()->GetPosition();
	pixelShader->SetFloat3("cameraPosition", cameraPos);

	pixelShader->SetSamplerState("Sampler", m_material->GetSamplerState());
	pixelShader->SetShaderResourceView("Texture", m_material->GetTextureSRV());
	pixelShader->SetShaderResourceView("NormalMap", m_material->GetNormalMapSRV());

	pixelShader->CopyAllBufferData();
	pixelShader->SetShader();

	for (UINT i = 0; i < m_meshes.size(); ++i)
	{
		m_meshes[i]->BindBuffers();
		m_meshes[i]->Draw();
	}
}

void StaticMeshObject::SetMesh(const std::string& fileName)
{
	m_fileName = fileName;
	m_meshes = ResourceManager::Instance.Get().GetMeshes(fileName);
}

void StaticMeshObject::SetVertexShader(const std::string& fileName)
{
	VertexShader* vs = ResourceManager::Instance.Get().GetVertexShader(fileName);
	if(vs != nullptr)
		m_material->SetVertexShader(vs);
}

void StaticMeshObject::SetPixelShader(const std::string& fileName)
{
	PixelShader* ps = ResourceManager::Instance.Get().GetPixelShader(fileName);
	if(ps != nullptr)
		m_material->SetPixelShader(ps);
}

void StaticMeshObject::SetDiffuseTexture(const std::string& fileName)
{
	ID3D11ShaderResourceView* diffuseTex = ResourceManager::Instance.Get().GetTexture(fileName);
	m_material->SetTextureSRV(diffuseTex);
}

void StaticMeshObject::SetNormalTexture(const std::string& fileName)
{
	ID3D11ShaderResourceView* normalTex = ResourceManager::Instance.Get().GetTexture(fileName);
	m_material->SetNormalTexture(normalTex);
}

void StaticMeshObject::SetSamplerState(ID3D11SamplerState* sampler)
{
	m_material->SetSamplerState(sampler);
}

