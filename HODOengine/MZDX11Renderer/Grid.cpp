#include "Grid.h"
#include "MZCamera.h"
#include "Mesh.h"
#include "Material.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "RasterizerState.h"
#include "DeferredBuffers.h"
using namespace DirectX;

Grid::Grid(ID3D11DeviceContext* deviceContext, Mesh* mesh, Material* material)
	: m_deviceContext(deviceContext), m_mesh(mesh), m_material(material), m_isActive(true),
	m_world{ XMMatrixIdentity() }, m_position{ 0, 0, 0 }, m_rotation{ 0, 0, 0, 1 }, m_scale{ 1, 1, 1 }
{
	
}

Grid::~Grid()
{
	m_deviceContext.Reset();

	delete m_mesh;
	delete m_material;
}

void Grid::Update(float deltaTime)
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

void Grid::Render()
{
#ifdef _DEBUG
	if (!m_isActive)
		return;

	m_deviceContext->RSSetState(RasterizerState::Instance.Get().GetWireframeRS());
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

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

	m_mesh->BindBuffers();

	m_mesh->Draw();
#endif
}

void Grid::SetWorldTM(const XMMATRIX& tm)
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

