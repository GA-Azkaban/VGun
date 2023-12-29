#include "Axis.h"
#include "MZCamera.h"
#include "Mesh.h"
#include "Material.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "RasterizerState.h"
#include "DeferredBuffers.h"
using namespace DirectX;

Axis::Axis(ID3D11DeviceContext* deviceContext, Mesh* mesh, Material* material)
	: m_deviceContext(deviceContext), m_mesh(mesh), m_material(material), m_isActive(true),
	m_world{ XMMatrixIdentity() }, m_position{ 0, 0, 0 }, m_rotation{ 0, 0, 0, 1 }, m_scale{ 1, 1, 1 }
{
	m_RS = RasterizerState::Instance.Get().GetWireframeRS();
	m_vertexShader = m_material->GetVertexShader();
	m_pixelShader = m_material->GetPixelShader();
}

Axis::~Axis()
{
	m_deviceContext.Reset();
	m_RS.Reset();

	delete m_vertexShader;
	delete m_pixelShader;
	delete m_mesh;
	delete m_material;
}

void Axis::Update(float deltaTime)
{
	if (!m_isActive)
		return;
	XMMATRIX trans = XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
	XMMATRIX rotX = XMMatrixRotationX(m_rotation.x);
	XMMATRIX rotY = XMMatrixRotationY(m_rotation.y);
	XMMATRIX rotZ = XMMatrixRotationZ(m_rotation.z);
	XMMATRIX sc = XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);

	XMMATRIX transformTM = sc * rotZ * rotY * rotX * trans;
	m_world = XMMatrixTranspose(transformTM);
}

void Axis::Render()
{
#ifdef _DEBUG
	if (!m_isActive)
		return;

	ID3D11Buffer* vb = m_mesh->GetVertexBuffer();
	ID3D11Buffer* ib = m_mesh->GetIndexBuffer();

	UINT stride = sizeof(VertexStruct::PosColor);
	UINT offset = 0;

	m_deviceContext->RSSetState(m_RS.Get());

	// 입력 배치 객체 셋팅
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	m_deviceContext->IASetVertexBuffers(0, 1, &vb, &stride, &offset);
	m_deviceContext->IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, 0);

	XMMATRIX view = MZCamera::GetMainCamera()->View();
	XMMATRIX proj = MZCamera::GetMainCamera()->Proj();
	XMMATRIX worldViewProj = m_world * view * proj;

	m_vertexShader->SetMatrix4x4("worldViewProj", worldViewProj);

	m_vertexShader->CopyAllBufferData();
	m_vertexShader->SetShader();

	m_pixelShader->CopyAllBufferData();
	m_pixelShader->SetShader();

	m_deviceContext->DrawIndexed(m_mesh->GetIndexCount(), 0, 0);
#endif
}

void Axis::SetWorldTM(const XMMATRIX& tm)
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
