#include "HelperObject.h"
#include "ShaderManager.h"
#include "Material.h"
#include "Mesh.h"
#include "MZCamera.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ResourceManager.h"

HelperObject::HelperObject()
	: m_material(nullptr), m_isActive(true),
	m_world{ XMMatrixIdentity() }, m_position{ 0, 0, 0 }, m_rotation{ 0, 0, 0, 1 }, m_scale{ 1, 1, 1 }
{
	m_material = new Material(ShaderManager::Instance.Get().GetVertexShader("DebugVertexShader.cso"), ShaderManager::Instance.Get().GetPixelShader("DebugPixelShader.cso"));
}

HelperObject::~HelperObject()
{
	delete m_material;
}

void HelperObject::Update(float deltaTime)
{
	if (!m_isActive)
		return;
	XMMATRIX trans = XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
	XMMATRIX rotX = XMMatrixRotationX(m_rotation.x);
	XMMATRIX rotY = XMMatrixRotationY(m_rotation.y);
	XMMATRIX rotZ = XMMatrixRotationZ(m_rotation.z);
	XMMATRIX sc = XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);

	m_world = sc * rotZ * rotY * rotX * trans;
}

void HelperObject::Render()
{
#ifdef _DEBUG
	if (!m_isActive)
		return;

	//m_deviceContext->RSSetState(RasterizerState::Instance.Get().GetWireframeRS());
	//m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	XMMATRIX view = MZCamera::GetMainCamera()->View();
	XMMATRIX proj = MZCamera::GetMainCamera()->Proj();
	XMMATRIX worldViewProj = m_world * view * proj;
	XMMATRIX invwvp = XMMatrixTranspose(worldViewProj);

	VertexShader* vertexShader = m_material->GetVertexShader();
	PixelShader* pixelShader = m_material->GetPixelShader();

	vertexShader->SetMatrix4x4("worldViewProj", invwvp);

	vertexShader->CopyAllBufferData();
	vertexShader->SetShader();

	pixelShader->CopyAllBufferData();
	pixelShader->SetShader();

	for (UINT i = 0; i < m_meshes.size(); ++i)
	{
		m_meshes[i]->BindBuffers();
		m_meshes[i]->Draw();
	}
#endif
}

void HelperObject::SetWorldTM(const XMMATRIX& tm)
{
	m_world = tm;

	XMVECTOR trans;
	XMVECTOR rot;
	XMVECTOR sc;

	XMMatrixDecompose(&sc, &rot, &trans, m_world);

	XMStoreFloat3(&m_position, trans);
	XMStoreFloat4(&m_rotation, rot);
	XMStoreFloat3(&m_scale, sc);
}

void HelperObject::SetMesh(const std::string& meshName)
{
	m_meshes = ResourceManager::Instance.Get().GetLoadedMesh(meshName);
}
