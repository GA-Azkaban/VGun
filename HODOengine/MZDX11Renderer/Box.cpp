#include "Box.h"
#include "MZCamera.h"
#include "DDSTextureLoader.h"
#include "MeshData.h"
#include "Effects.h"
#include "ResourceManager.h"
#include "FbxLoader.h"


Box::Box(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, ID3D11RasterizerState* pRS)
	: m_d3dDevice(pDevice), m_d3dImmediateContext(pDeviceContext), m_pRS(pRS),
	m_VB(nullptr), m_IB(nullptr)
{
	
}

Box::~Box()
{
	SafeRelease(m_IB);
	SafeRelease(m_VB);
}

void Box::Initialize()
{
	BuildGeometryBuffers();

	m_material.Ambient = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	m_material.Diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	m_material.Specular = XMFLOAT4(0.8f, 0.8f, 0.8f, 16.0f);

	m_dirLights[0].Ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	m_dirLights[0].Diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	m_dirLights[0].Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	m_dirLights[0].Direction = XMFLOAT3(0.57735f, -0.57735f, 0.57735f);
}

// ASE 파서에서 버텍스 정보, 인덱스 정보를 얻어온다.
void Box::BuildGeometryBuffers()
{
	FbxLoader* _pFBXLoader = new FbxLoader();
	_pFBXLoader->Load("FBXFile/box.fbx");
	//_pFBXLoader->Load("FBXFile/fox.FBX");

	UINT vcount = _pFBXLoader->GetVertexList().size();
	std::vector<VertexStruct::PosNormal> vertices(vcount);

	for (UINT i = 0; i < vcount; ++i)
	{
		vertices[i].Normal.x = _pFBXLoader->GetVertexList()[i].normal[0];
		vertices[i].Normal.y = _pFBXLoader->GetVertexList()[i].normal[1];
		vertices[i].Normal.z = _pFBXLoader->GetVertexList()[i].normal[2];

		vertices[i].Pos.x = _pFBXLoader->GetVertexList()[i].pos[0];
		vertices[i].Pos.y = _pFBXLoader->GetVertexList()[i].pos[1];
		vertices[i].Pos.z = _pFBXLoader->GetVertexList()[i].pos[2];
	}

	IndexCount = _pFBXLoader->GetIndexList().size();

	std::vector<UINT> indices(IndexCount);
	for (UINT i = 0; i < IndexCount; ++i)
	{
		indices[i] = _pFBXLoader->GetIndexList()[i];
	}
	
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(VertexStruct::PosNormal) * vcount;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices[0];
	HR(m_d3dDevice->CreateBuffer(&vbd, &vinitData, &m_VB));
	
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * IndexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];
	HR(m_d3dDevice->CreateBuffer(&ibd, &iinitData, &m_IB));
}

void Box::Update(MZCamera* pCamera)
{
	XMStoreFloat4x4(&m_world, XMMatrixIdentity());
	XMStoreFloat4x4(&m_view, pCamera->View());
	XMStoreFloat4x4(&m_proj, pCamera->Proj());

	mEyePosW = XMFLOAT3(pCamera->GetPosition().x, pCamera->GetPosition().y, pCamera->GetPosition().z);
}

void Box::Render()
{
	// 입력 배치 객체 셋팅
	m_d3dImmediateContext->IASetInputLayout(InputLayouts::PosNormal);

	m_d3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 렌더 스테이트
	m_d3dImmediateContext->RSSetState(m_pRS.Get());

	/// WVP TM등을 셋팅
	// Set constants
	XMMATRIX view = XMLoadFloat4x4(&m_view);
	XMMATRIX proj = XMLoadFloat4x4(&m_proj);
	XMMATRIX world = XMLoadFloat4x4(&m_world);
	XMMATRIX worldViewProj = world * view * proj;

	// 월드의 역행렬
	XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(world);

	// Figure out which technique to use.
	ID3DX11EffectTechnique* mTech = Effects::BasicFX->Light1Tech;

	// 테크닉은...
	D3DX11_TECHNIQUE_DESC techDesc;
	mTech->GetDesc(&techDesc);

	UINT stride = sizeof(VertexStruct::PosNormal);
	UINT offset = 0;
	m_d3dImmediateContext->IASetVertexBuffers(0, 1, &m_VB, &stride, &offset);
	m_d3dImmediateContext->IASetIndexBuffer(m_IB, DXGI_FORMAT_R32_UINT, 0);

	Effects::BasicFX->SetWorld(world);
	Effects::BasicFX->SetWorldInvTranspose(worldInvTranspose);
	Effects::BasicFX->SetWorldViewProj(worldViewProj);
	Effects::BasicFX->SetEyePosW(mEyePosW);
	Effects::BasicFX->SetMaterial(m_material);
	Effects::BasicFX->SetDirLights(m_dirLights);

	mTech->GetPassByIndex(0)->Apply(0, m_d3dImmediateContext.Get());
	m_d3dImmediateContext->DrawIndexed(IndexCount, 0, 0);
}

