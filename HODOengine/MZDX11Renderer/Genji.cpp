#include "Genji.h"
#include "MZCamera.h"
#include "DDSTextureLoader.h"
#include "MeshData.h"
#include "Effects.h"
#include "ResourceManager.h"


Genji::Genji(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, ID3D11RasterizerState* pRS)
	: m_d3dDevice(pDevice), m_d3dImmediateContext(pDeviceContext), m_pRS(pRS),
	m_VB(nullptr), m_IB(nullptr), mDiffuseMapSRV(0), mNormalMapSRV(0)
{
	m_material.Ambient = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	m_material.Diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	m_material.Specular = XMFLOAT4(0.8f, 0.8f, 0.8f, 16.0f);

	ID3D11Resource* texResource = nullptr;
	HR(CreateDDSTextureFromFile(m_d3dDevice.Get(),
		//L"Textures/000000002405.dds", &texResource, &mDiffuseMapSRV));
		L"Textures/000000002405_reverse.dds", &texResource, &mDiffuseMapSRV));
	SafeRelease(texResource);	// view saves reference

	ID3D11Resource* normalMapResource = nullptr;
	HR(CreateDDSTextureFromFile(m_d3dDevice.Get(),
		L"Textures/000000002406_b_reverse.dds", &normalMapResource, &mNormalMapSRV));
	SafeRelease(normalMapResource);

	BuildGeometryBuffers();

	XMStoreFloat4x4(&mTexTransform, XMMatrixIdentity());
	SetTranslateMatrix(0.0f, 0.0f, 0.0f);
	SetRotateMatrix(0.0f);
	SetScaleMatrix(1.0f);
}

//Genji::Genji()
//	://IRenderableObject(),
//	m_VB(nullptr), m_IB(nullptr), //m_FX(nullptr), m_effectTech(nullptr), m_fxWorldViewProj(nullptr), m_inputLayout(nullptr),
//	m_world(), m_view(), m_proj()
//{
//
//}

Genji::~Genji()
{
	SafeRelease(mDiffuseMapSRV);
	SafeRelease(m_IB);
	SafeRelease(m_VB);
}


void Genji::SetTranslateMatrix(float offsetX, float offsetY, float offsetZ)
{
	translateTM.r[0] = { 1.0f, 0.0f, 0.0f, 0.0f };
	translateTM.r[1] = { 0.0f, 1.0f, 0.0f, 0.0f };
	translateTM.r[2] = { 0.0f, 0.0f, 1.0f, 0.0f };
	translateTM.r[3] = { offsetX, offsetY, offsetZ, 1.0f };
}


void Genji::SetRotateMatrix(float radian)
{
	rotateTM.r[0] = { cosf(radian), 0.0f, sinf(radian), 0.0f };
	rotateTM.r[1] = { 0.0f, 1.0f, 0.0f, 0.0f };
	rotateTM.r[2] = { -sinf(radian), 0.0f, cosf(radian), 0.0f };
	rotateTM.r[3] = { 0.0f, 0.0f, 0.0f, 1.0f };
}

void Genji::SetScaleMatrix(float scale)
{
	scaleTM.r[0] = { scale, 0.0f, 0.0f, 0.0f };
	scaleTM.r[1] = { 0.0f, scale, 0.0f, 0.0f };
	scaleTM.r[2] = { 0.0f, 0.0f, scale, 0.0f };
	scaleTM.r[3] = { 0.0f, 0.0f, 0.0f, 1.0f };
}

// ASE 파서에서 버텍스 정보, 인덱스 정보를 얻어온다.
void Genji::BuildGeometryBuffers()
{
	MZMeshData::MeshData* _pMeshData = ResourceManager::Instance.Get().LoadMeshData((LPSTR)"ASEFile/genji_blender.ASE");
	MZMeshData::Mesh* mesh = _pMeshData->GetMesh(0);

	UINT vcount = 0;
	UINT faceCount = 0;
	UINT tcount = 0;

	faceCount = mesh->m_meshface.size();
	vcount = faceCount * 3;
	std::vector<VertexStruct::NormalMap> vertices(vcount);

	for (int i = 0; i < faceCount; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			//int vIndex = meshData->m_meshface[i]->m_vertexindex[j];
			//int tIndex = meshData->m_meshface[i]->m_TFace[j];

			vertices[i * 3 + j].Normal.x = mesh->m_meshface[i]->m_normalvertex[j].x;
			vertices[i * 3 + j].Normal.y = mesh->m_meshface[i]->m_normalvertex[j].z;
			vertices[i * 3 + j].Normal.z = mesh->m_meshface[i]->m_normalvertex[j].y;

			/*vertices[i * 3 + j].Pos.x = meshData->m_meshvertex[vIndex]->m_pos.x;
			vertices[i * 3 + j].Pos.y = meshData->m_meshvertex[vIndex]->m_pos.z;
			vertices[i * 3 + j].Pos.z = meshData->m_meshvertex[vIndex]->m_pos.y;
			if (!meshData->m_mesh_tvertex.empty())
			{
				vertices[i * 3 + j].Tex.x = meshData->m_mesh_tvertex[tIndex]->m_u;
				vertices[i * 3 + j].Tex.y = meshData->m_mesh_tvertex[tIndex]->m_v;
			}*/

			// tangent 값 초기화			
			vertices[i * 3 + j].TangentU = Vector3(0.0f, 0.0f, 0.0f);

		}
		int vIndex0 = mesh->m_meshface[i]->m_vertexindex[0];
		int vIndex1 = mesh->m_meshface[i]->m_vertexindex[2];
		int vIndex2 = mesh->m_meshface[i]->m_vertexindex[1];
		int tIndex0 = mesh->m_meshface[i]->m_TFace[0];
		int tIndex1 = mesh->m_meshface[i]->m_TFace[2];
		int tIndex2 = mesh->m_meshface[i]->m_TFace[1];

		vertices[i * 3 + 0].Pos.x = mesh->m_meshvertex[vIndex0]->m_pos.x;
		vertices[i * 3 + 0].Pos.y = mesh->m_meshvertex[vIndex0]->m_pos.z;
		vertices[i * 3 + 0].Pos.z = mesh->m_meshvertex[vIndex0]->m_pos.y;

		vertices[i * 3 + 1].Pos.x = mesh->m_meshvertex[vIndex1]->m_pos.x;
		vertices[i * 3 + 1].Pos.y = mesh->m_meshvertex[vIndex1]->m_pos.z;
		vertices[i * 3 + 1].Pos.z = mesh->m_meshvertex[vIndex1]->m_pos.y;

		vertices[i * 3 + 2].Pos.x = mesh->m_meshvertex[vIndex2]->m_pos.x;
		vertices[i * 3 + 2].Pos.y = mesh->m_meshvertex[vIndex2]->m_pos.z;
		vertices[i * 3 + 2].Pos.z = mesh->m_meshvertex[vIndex2]->m_pos.y;

		if (!mesh->m_mesh_tvertex.empty())
		{
			vertices[i * 3 + 0].Tex.x = mesh->m_mesh_tvertex[tIndex0]->m_u;
			vertices[i * 3 + 0].Tex.y = mesh->m_mesh_tvertex[tIndex0]->m_v;

			vertices[i * 3 + 1].Tex.x = mesh->m_mesh_tvertex[tIndex1]->m_u;
			vertices[i * 3 + 1].Tex.y = mesh->m_mesh_tvertex[tIndex1]->m_v;

			vertices[i * 3 + 2].Tex.x = mesh->m_mesh_tvertex[tIndex2]->m_u;
			vertices[i * 3 + 2].Tex.y = mesh->m_mesh_tvertex[tIndex2]->m_v;
		}
		/// CalculateTangents
		// 모든 삼각형에 대해 Tangent 계산하고, 삼각형의 3개의 버텍스에 Tangent 값을 누산함.
		/*int vi0 = meshData->m_meshface[i]->m_vertexindex[0];
		int vi1 = meshData->m_meshface[i]->m_vertexindex[1];
		int vi2 = meshData->m_meshface[i]->m_vertexindex[2];
		int ti0 = meshData->m_meshface[i]->m_TFace[0];
		int ti1 = meshData->m_meshface[i]->m_TFace[1];
		int ti2 = meshData->m_meshface[i]->m_TFace[2];
		const XMFLOAT3& p0 = meshData->m_meshvertex[vi0]->m_pos;
		const XMFLOAT3& p1 = meshData->m_meshvertex[vi1]->m_pos;
		const XMFLOAT3& p2 = meshData->m_meshvertex[vi2]->m_pos;
		const XMFLOAT2& w0 = XMFLOAT2(meshData->m_mesh_tvertex[ti0]->m_u, meshData->m_mesh_tvertex[ti0]->m_v);
		const XMFLOAT2& w1 = XMFLOAT2(meshData->m_mesh_tvertex[ti1]->m_u, meshData->m_mesh_tvertex[ti1]->m_v);
		const XMFLOAT2& w2 = XMFLOAT2(meshData->m_mesh_tvertex[ti2]->m_u, meshData->m_mesh_tvertex[ti2]->m_v);*/
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

	// 얻어진 tangent 벡터는 이 정점을 공유하고 있는 모든 삼각형들의 
	// tangent 벡터와 합산하여 계산된다.
	// 여러 개의 tangent 벡터를 합산하고 나면 T, B, N 벡터는 서로 수직이 아닐 수 있다.
	// 따라서 수직을 만들어 주는 과정을 추가해야 한다.
	// T, B, N 벡터를 모두 구하면 Normal을 탄젠트 공간에서 오브젝트 공간으로 변환가능하다.
	// B 벡터는 T, N 벡터의 외적으로 구할 수 있으므로 지금은 저장하지 않는다.
	for(int i = 0; i < vcount; ++i)
	{
		XMVECTOR n = XMLoadFloat3(&(vertices[i].Normal));
		XMVECTOR t = XMLoadFloat3(&(vertices[i].TangentU));
		Vector3 Tangent = XMVector3Normalize(t - XMVector3Dot(t, n) * n);
		vertices[i].TangentU = Tangent;
	}


	tcount = mesh->m_mesh_numfaces;
	IndexCount = 3 * tcount;
	std::vector<UINT> indices(IndexCount);
	for (UINT i = 0; i < tcount; ++i)
	{
		indices[i * 3 + 0] = i * 3 + 0;
		indices[i * 3 + 1] = i * 3 + 1;
		indices[i * 3 + 2] = i * 3 + 2;
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

	//
	// Pack the indices of all the meshes into one index buffer.
	//

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * IndexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];
	HR(m_d3dDevice->CreateBuffer(&ibd, &iinitData, &m_IB));

	//ReleaseASEParser(m_pASEParser);
}

void Genji::Update(MZCamera* pCamera, float deltaTime)
{
	if (!m_isActive)
		return;
	m_world = scaleTM * rotateTM * translateTM;
	//XMStoreFloat4x4(&m_world, XMMatrixIdentity());
	//XMStoreFloat4x4(&m_world, mWorldPos);
	XMStoreFloat4x4(&m_view, pCamera->View());
	XMStoreFloat4x4(&m_proj, pCamera->Proj());

	//mEyePosW = XMFLOAT3(pCamera->GetPosition().x, pCamera->GetPosition().y, pCamera->GetPosition().z);
}

void Genji::RenderToTexture()
{
	if (!m_isActive)
		return;
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
	Effects::StaticMeshFX->SetNormalMap(mNormalMapSRV);

	mTech->GetPassByIndex(0)->Apply(0, m_d3dImmediateContext.Get());
	m_d3dImmediateContext->DrawIndexed(IndexCount, 0, 0);

}
