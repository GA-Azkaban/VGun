#include "LightObject.h"
#include "Mesh.h"
#include "Material.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "RasterizerState.h"
#include "MZCamera.h"
#include "DeferredBuffers.h"
using namespace DirectX;

LightObject::LightObject(ID3D11DeviceContext* deviceContext, Mesh* mesh, Material* material, DirectX::XMFLOAT3 lightColor)
	: m_deviceContext(deviceContext), m_mesh(mesh), m_material(material), m_lightColor(lightColor), m_isActive(true),
	m_world{ XMMatrixIdentity() }, m_position{ 0, 0, 0 }, m_rotation{ 0, 0, 0, 1 }, m_scale{ 1, 1, 1 }
{
	m_RS = RasterizerState::Instance.Get().GetSolidRS();
	m_vertexShader = m_material->GetVertexShader();
	m_pixelShader = m_material->GetPixelShader();
}

LightObject::~LightObject()
{
	m_deviceContext.Reset();
	m_RS.Reset();

	delete m_vertexShader;
	delete m_pixelShader;
	delete m_mesh;
	delete m_material;
}

void LightObject::Update(float deltaTime)
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

void LightObject::Render()
{
	if (!m_isActive)
		return;

	m_deviceContext->RSSetState(m_RS.Get());
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	XMMATRIX view = MZCamera::GetMainCamera()->View();
	XMMATRIX proj = MZCamera::GetMainCamera()->Proj();
	XMMATRIX worldViewProj = m_world * view * proj;

	m_vertexShader->SetMatrix4x4("worldViewProj", worldViewProj);

	m_vertexShader->CopyAllBufferData();
	m_vertexShader->SetShader();

	//m_pixelShader->SetShaderResourceView("NormalMap", DeferredBuffers::Instance.Get().GetShaderResourceView(0));
	//m_pixelShader->SetShaderResourceView("Texture", DeferredBuffers::Instance.Get().GetShaderResourceView(1));
	//m_pixelShader->SetShaderResourceView("PositionTexture", DeferredBuffers::Instance.Get().GetShaderResourceView(2));

	m_pixelShader->SetFloat3("cameraPosition", MZCamera::GetMainCamera()->GetPosition());

	m_pixelShader->SetFloat3("lightColor", m_lightColor);
	m_pixelShader->SetFloat3("lightPosition", m_position);

	m_pixelShader->CopyAllBufferData();
	m_pixelShader->SetShader();

	m_mesh->BindBuffers();

	m_mesh->Draw();
}

void LightObject::SetWorldTM(const DirectX::XMMATRIX& tm)
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

