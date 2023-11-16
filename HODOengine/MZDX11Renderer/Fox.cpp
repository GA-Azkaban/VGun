#include "Fox.h"
#include "MZCamera.h"
#include "DDSTextureLoader.h"
#include "MeshData.h"
#include "Effects.h"
#include "ResourceManager.h"
#include "FbxLoader.h"
#include "Vertex.h"


Fox::Fox(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, ID3D11RasterizerState* pRS)
	: m_d3dDevice(pDevice), m_d3dImmediateContext(pDeviceContext), m_pRS(pRS),
	m_VB(nullptr), m_IB(nullptr), mDiffuseMapSRV(0), mNormalMapSRV(0)
{
	XMStoreFloat4x4(&mTexTransform, XMMatrixIdentity());
	SetTranslateMatrix(0.0f, 0.0f, 0.0f);
	SetRotateMatrix(0.0f);
	SetScaleMatrix(1.0f);

	m_material.Ambient = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	m_material.Diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	m_material.Specular = XMFLOAT4(0.8f, 0.8f, 0.8f, 16.0f);

	ID3D11Resource* texResource = nullptr;
	HR(CreateDDSTextureFromFile(m_d3dDevice.Get(),
		//L"Textures/000000002405.dds", &texResource, &mDiffuseMapSRV));
		L"Textures/fox_reverse.dds", &texResource, &mDiffuseMapSRV));
	SafeRelease(texResource);	// view saves reference

	/*ID3D11Resource* normalMapResource = nullptr;
	HR(CreateDDSTextureFromFile(m_d3dDevice.Get(),
		L"Textures/000000002406_b.dds", &normalMapResource, &mNormalMapSRV));
	SafeRelease(normalMapResource);*/

	BuildGeometryBuffers();
}

Fox::~Fox()
{
	SafeRelease(mDiffuseMapSRV);
	SafeRelease(m_IB);
	SafeRelease(m_VB);
}


void Fox::SetTranslateMatrix(float offsetX, float offsetY, float offsetZ)
{
	translateTM.r[0] = { 1.0f, 0.0f, 0.0f, 0.0f };
	translateTM.r[1] = { 0.0f, 1.0f, 0.0f, 0.0f };
	translateTM.r[2] = { 0.0f, 0.0f, 1.0f, 0.0f };
	translateTM.r[3] = { offsetX, offsetY, offsetZ, 1.0f };
}


void Fox::SetRotateMatrix(float radian)
{
	rotateTM.r[0] = { cosf(radian), 0.0f, sinf(radian), 0.0f };
	rotateTM.r[1] = { 0.0f, 1.0f, 0.0f, 0.0f };
	rotateTM.r[2] = { -sinf(radian), 0.0f, cosf(radian), 0.0f };
	rotateTM.r[3] = { 0.0f, 0.0f, 0.0f, 1.0f };
}

void Fox::SetScaleMatrix(float scale)
{
	scaleTM.r[0] = { scale, 0.0f, 0.0f, 0.0f };
	scaleTM.r[1] = { 0.0f, scale, 0.0f, 0.0f };
	scaleTM.r[2] = { 0.0f, 0.0f, scale, 0.0f };
	scaleTM.r[3] = { 0.0f, 0.0f, 0.0f, 1.0f };
}

// ASE 파서에서 버텍스 정보, 인덱스 정보를 얻어온다.
void Fox::BuildGeometryBuffers()
{
	MZMeshData::MeshData* _pMeshData = ResourceManager::Instance.Get().LoadMeshData((LPSTR)"FBXFile/fox.fbx");

	UINT vcount = 0;
	std::vector<VertexStruct::NormalMap> vertices;
	std::vector<UINT> indices;

	//for (UINT meshIndex = 0; meshIndex < _pMeshData->m_MeshList.size(); ++meshIndex)
	for (UINT meshIndex = 0; meshIndex < 1; ++meshIndex)
	{
		MZMeshData::Mesh* _pMesh = _pMeshData->GetMesh(meshIndex);
		vcount = _pMesh->m_meshvertex.size();
		vertices.resize(vcount);

		for (UINT i = 0; i < vcount; ++i)
		{
			vertices[i].Normal.x = _pMesh->m_meshvertex[i]->m_normal.x;
			vertices[i].Normal.y = _pMesh->m_meshvertex[i]->m_normal.y;
			vertices[i].Normal.z = _pMesh->m_meshvertex[i]->m_normal.z;

			vertices[i].Pos.x = _pMesh->m_meshvertex[i]->m_pos.x;
			vertices[i].Pos.y = _pMesh->m_meshvertex[i]->m_pos.y;
			vertices[i].Pos.z = _pMesh->m_meshvertex[i]->m_pos.z;

			vertices[i].Tex.x  = _pMesh->m_meshvertex[i]->u;
			vertices[i].Tex.y  = _pMesh->m_meshvertex[i]->v;
		}

		/// CalculateTangents
		UINT faceCount = vcount / 3;
		for (UINT i = 0; i < faceCount; ++i)
		{
			const XMFLOAT3& p0 = vertices[i * 3 + 0].Pos;
			const XMFLOAT3& p1 = vertices[i * 3 + 1].Pos;
			const XMFLOAT3& p2 = vertices[i * 3 + 2].Pos;
			const XMFLOAT2& w0 = vertices[i * 3 + 0].Tex;
			const XMFLOAT2& w1 = vertices[i * 3 + 1].Tex;
			const XMFLOAT2& w2 = vertices[i * 3 + 2].Tex;

			Vector3 de1 = p1 - p0;
			Vector3 de2 = p2 - p0;
			float du1 = w1.x - w0.x;
			float du2 = w2.x - w0.x;
			float dv1 = w1.y - w0.y;
			float dv2 = w2.y - w0.y;

			float r = 1.0f / (du1 * dv2 - du2 * dv1);
			Vector3 t = (de1 * dv2 - de2 * dv1) * r;

			for (int j = 0; j < 3; j++)
			{
				vertices[i * 3 + j].TangentU.x += t.x;
				vertices[i * 3 + j].TangentU.y += t.y;
				vertices[i * 3 + j].TangentU.z += t.z;
			}
		}

		/*for (int i = 0; i < vcount; ++i)
		{
			XMVECTOR n = XMLoadFloat3(&(vertices[i].Normal));
			XMVECTOR t = XMLoadFloat3(&(vertices[i].TangentU));
			Vector3 Tangent = XMVector3Normalize(t - XMVector3Dot(t, n) * n);
			vertices[i].TangentU = Tangent;
		}*/

		//IndexCount = _pMesh->m_indicesList.size();
		IndexCount = vcount;
		indices.resize(IndexCount);

		for (UINT i = 0; i < IndexCount; ++i)
		{
			indices[i] = i;
		}
	}

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(VertexStruct::NormalMap) * vcount;
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

void Fox::Update(MZCamera* pCamera, float deltaTime)
{
	m_world = scaleTM * rotateTM * translateTM;
	//XMStoreFloat4x4(&m_world, XMMatrixIdentity());
	XMStoreFloat4x4(&m_view, pCamera->View());
	XMStoreFloat4x4(&m_proj, pCamera->Proj());

	//mEyePosW = XMFLOAT3(pCamera->GetPosition().x, pCamera->GetPosition().y, pCamera->GetPosition().z);
}

void Fox::RenderToTexture()
{
	// 입력 배치 객체 셋팅
	m_d3dImmediateContext->IASetInputLayout(InputLayouts::StaticMeshGBufferInputLayout);

	m_d3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 렌더 스테이트
	m_d3dImmediateContext->RSSetState(m_pRS.Get());

	/// WVP TM등을 셋팅
	// Set constants
	XMMATRIX view = XMLoadFloat4x4(&m_view);
	XMMATRIX proj = XMLoadFloat4x4(&m_proj);
	//XMMATRIX world = XMLoadFloat4x4(&m_world);
	XMMATRIX worldViewProj = m_world * view * proj;

	// 월드의 역행렬
	XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(m_world);

	// Figure out which technique to use.
	ID3DX11EffectTechnique* mTech = Effects::StaticMeshFX->GetGBufferTech;

	// 테크닉은...
	D3DX11_TECHNIQUE_DESC techDesc;
	mTech->GetDesc(&techDesc);

	UINT stride = sizeof(VertexStruct::NormalMap);
	UINT offset = 0;
	m_d3dImmediateContext->IASetVertexBuffers(0, 1, &m_VB, &stride, &offset);
	m_d3dImmediateContext->IASetIndexBuffer(m_IB, DXGI_FORMAT_R32_UINT, 0);

	Effects::StaticMeshFX->SetWorld(m_world);
	Effects::StaticMeshFX->SetWorldInvTranspose(worldInvTranspose);
	Effects::StaticMeshFX->SetWorldViewProj(worldViewProj);
	Effects::StaticMeshFX->SetTexTransform(XMLoadFloat4x4(&mTexTransform));
	//Effects::StaticMeshFX->SetMaterial(m_material);
	Effects::StaticMeshFX->SetDiffuseMap(mDiffuseMapSRV);
	//Effects::StaticMeshFX->SetNormalMap(mNormalMapSRV);

	mTech->GetPassByIndex(0)->Apply(0, m_d3dImmediateContext.Get());
	m_d3dImmediateContext->DrawIndexed(IndexCount, 0, 0);

}


