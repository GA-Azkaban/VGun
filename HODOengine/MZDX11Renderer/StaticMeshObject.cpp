#include "StaticMeshObject.h"
#include "MZDX11Renderer.h"
#include "MZCamera.h"
#include "Mesh.h"
#include "Material.h"
#include "RasterizerState.h"
#include "SamplerState.h"
#include "ResourceManager.h"
#include "ShaderManager.h"

StaticMeshObject::StaticMeshObject()
	: m_material(nullptr), m_isActive(true),
	m_world{ XMMatrixIdentity() }
	//m_meshBox(), m_depth(1.0f), m_isPickingOn(true)
{
	m_material = new Material(ShaderManager::Instance.Get().GetVertexShader("VertexShader.cso"), ShaderManager::Instance.Get().GetPixelShader("PixelShader.cso"));
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

	// 이거는 바깥쪽에서 한다.
	//MZDX11Renderer::Instance().GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//MZDX11Renderer::Instance().GetDeviceContext()->RSSetState(RasterizerState::Instance.Get().GetSolidRS());
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

bool StaticMeshObject::Pick(float x, float y)
{
	//   if (!m_isActive)
	//       return false;

	   //if (!m_isPickingOn)
	   //	return false;
	//   // Ray definition in view space.
	//   XMVECTOR rayOrigin = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	//   XMVECTOR rayDir = XMVectorSet(x, y, 1.0f, 0.0f);

	//   // Tranform ray to local space of Mesh.
	//   XMMATRIX V = MZCamera::GetMainCamera()->View();
	//   XMVECTOR detView = XMMatrixDeterminant(V);
	//   XMMATRIX invView = XMMatrixInverse(&detView, V);

	//   //XMMATRIX W = XMLoadFloat4x4(&m_world);
	//   XMVECTOR detWorld = XMMatrixDeterminant(m_world);
	//   XMMATRIX invWorld = XMMatrixInverse(&detWorld, m_world);

	//   XMMATRIX toLocal = XMMatrixMultiply(invView, invWorld);

	//   rayOrigin = XMVector3TransformCoord(rayOrigin, toLocal);
	//   rayDir = XMVector3TransformNormal(rayDir, toLocal);

	//   // Make the ray direction unit length for the intersection tests.
	//   rayDir = XMVector3Normalize(rayDir);

	//   float tmin = 0.0f;
	//   if (XNA::IntersectRayAxisAlignedBox(rayOrigin, rayDir, &m_meshBox, &tmin))
	//   {
	//       m_depth = tmin;
	//       return true;
	//   }
	return false;
}

void StaticMeshObject::SetMesh(const std::string& fileName)
{
	m_fileName = fileName;
	m_meshes = ResourceManager::Instance.Get().GetMeshes(fileName);
}

void StaticMeshObject::SetVertexShader(const std::string& fileName)
{
	VertexShader* vs = ShaderManager::Instance.Get().GetVertexShader(fileName);
	if(vs != nullptr)
		m_material->SetVertexShader(vs);
}

void StaticMeshObject::SetPixelShader(const std::string& fileName)
{
	PixelShader* ps = ShaderManager::Instance.Get().GetPixelShader(fileName);
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

