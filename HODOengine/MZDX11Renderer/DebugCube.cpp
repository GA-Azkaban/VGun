#include "DebugCube.h"
#include "MZCamera.h"
#include "Vertex.h"
#include "Effects.h"


bool DebugCube::s_IsActive = true;

DebugCube::DebugCube(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, ID3D11RasterizerState* pRS, XMFLOAT3 position, XMFLOAT3 size, XMFLOAT4 color)
	: m_d3dDevice(pDevice), m_d3dImmediateContext(pDeviceContext), m_pRenderstate(pRS),
	m_VB(nullptr), m_IB(nullptr),
	m_world(XMMatrixIdentity()), m_view(), m_proj(),
	m_position(position), m_size(size), m_color(color)
{
	BuildGeometryBuffers();
	m_isActive = s_IsActive;
}

DebugCube::~DebugCube()
{
	
}

void DebugCube::SetColor(XMFLOAT4 color)
{
	m_color = color;
}

void DebugCube::Update(MZCamera* pCamera, float deltaTime)
{
	if (!m_isActive)
		return;
	/*XMMATRIX worldTM = XMMatrixIdentity();
	XMStoreFloat4x4(&m_world, worldTM);*/
	//XMStoreFloat4x4(&m_world, XMMatrixIdentity());
	//XMStoreFloat4x4(&m_view, pCamera->View());
	//XMStoreFloat4x4(&m_proj, pCamera->Proj());
	DirectX::XMMATRIX view = pCamera->View();
	DirectX::XMMATRIX proj = pCamera->Proj();
	Update(view, proj);
}

void DebugCube::Update(const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& proj)
{
	if (!m_isActive) 
		return;
	XMStoreFloat4x4(&m_view, view);
	XMStoreFloat4x4(&m_proj, proj);
}

void DebugCube::RenderToTexture()
{
#ifdef _DEBUG
	if (!m_isActive) 
		return;
	// 입력 배치 객체 셋팅
	m_d3dImmediateContext->IASetInputLayout(InputLayouts::PosColor);
	m_d3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 렌더 스테이트
	m_d3dImmediateContext->RSSetState(m_pRenderstate.Get());

	// 버텍스버퍼와 인덱스버퍼 셋팅
	UINT stride = sizeof(VertexStruct::PosColor);
	UINT offset = 0;
	m_d3dImmediateContext->IASetVertexBuffers(0, 1, m_VB.GetAddressOf(), &stride, &offset);
	m_d3dImmediateContext->IASetIndexBuffer(m_IB.Get(), DXGI_FORMAT_R32_UINT, 0);

	/// WVP TM등을 셋팅
	// Set constants
	XMMATRIX view = XMLoadFloat4x4(&m_view);
	XMMATRIX proj = XMLoadFloat4x4(&m_proj);
	//XMMATRIX world = XMLoadFloat4x4(&m_world);
	XMMATRIX worldViewProj = m_world * view * proj;

	ID3DX11EffectTechnique* mTech = Effects::DebugObjectFX->ColorTech;

	// 테크닉은...
	D3DX11_TECHNIQUE_DESC techDesc;
	mTech->GetDesc(&techDesc);

	Effects::DebugObjectFX->SetWorld(m_world);
	Effects::DebugObjectFX->SetWorldViewProj(worldViewProj);
	Effects::DebugObjectFX->SetColor(m_color);

	// 렌더패스는...
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		mTech->GetPassByIndex(p)->Apply(0, m_d3dImmediateContext.Get());
		m_d3dImmediateContext->DrawIndexed(36, 0, 0);
	}
#endif
}

void DebugCube::BuildGeometryBuffers()
{
	/// 버텍스 버퍼를 생성한다.

	VertexStruct::PosColor vertices[] =
	{
		{ XMFLOAT3((-m_size.x / 2.0f), (-m_size.y / 2.0f), (-m_size.z / 2.0f)), m_color},
		{ XMFLOAT3((-m_size.x / 2.0f), (+m_size.y / 2.0f), (-m_size.z / 2.0f)), m_color},
		{ XMFLOAT3((+m_size.x / 2.0f), (+m_size.y / 2.0f), (-m_size.z / 2.0f)), m_color},
		{ XMFLOAT3((+m_size.x / 2.0f), (-m_size.y / 2.0f), (-m_size.z / 2.0f)), m_color},
		{ XMFLOAT3((-m_size.x / 2.0f), (-m_size.y / 2.0f), (+m_size.z / 2.0f)), m_color},
		{ XMFLOAT3((-m_size.x / 2.0f), (+m_size.y / 2.0f), (+m_size.z / 2.0f)), m_color},
		{ XMFLOAT3((+m_size.x / 2.0f), (+m_size.y / 2.0f), (+m_size.z / 2.0f)), m_color},
		{ XMFLOAT3((+m_size.x / 2.0f), (-m_size.y / 2.0f), (+m_size.z / 2.0f)), m_color},
	};
	
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(VertexStruct::PosColor) * 8;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = vertices;
	HR(m_d3dDevice->CreateBuffer(&vbd, &vinitData, m_VB.GetAddressOf()));


	/// 인덱스 버퍼를 생성한다.

	UINT indices[] =
	{
		// front face
		0, 1, 2,
		0, 2, 3,

		// back face
		4, 6, 5,
		4, 7, 6,

		// left face
		4, 5, 1,
		4, 1, 0,

		// right face
		3, 2, 6,
		3, 6, 7,

		// top face
		1, 5, 6,
		1, 6, 2,

		// bottom face
		4, 0, 3,
		4, 3, 7
	};

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * 36;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indices;
	HR(m_d3dDevice->CreateBuffer(&ibd, &iinitData, m_IB.GetAddressOf()));

}

