#include "StaticMesh.h"
#include "MZCamera.h"
#include "DDSTextureLoader.h"
#include "MeshData.h"
#include "Effects.h"
#include "ResourceManager.h"
#include "MZUtility.h"
#include "DebugCube.h"


StaticMesh::StaticMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, ID3D11RasterizerState* pRS)
    : m_d3dDevice(pDevice), m_d3dImmediateContext(pDeviceContext), m_pRS(pRS),
    mDiffuseMapSRV(0), mNormalMapSRV(0), m_world{ XMMatrixIdentity() }
{
    XMStoreFloat4x4(&mTexTransform, XMMatrixIdentity());
    m_material.Ambient = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
    m_material.Diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
    m_material.Specular = XMFLOAT4(0.8f, 0.8f, 0.8f, 16.0f);
}

//StaticMesh::StaticMesh()
//	://IRenderableObject(),
//	m_VB(nullptr), m_IB(nullptr), //m_FX(nullptr), m_effectTech(nullptr), m_fxWorldViewProj(nullptr), m_inputLayout(nullptr),
//	m_world(), m_view(), m_proj()
//{
//
//}

StaticMesh::~StaticMesh()
{
    if (mDiffuseMapSRV)
        SafeRelease(mDiffuseMapSRV);
}


void StaticMesh::SetWorldTM(const XMMATRIX& tm)
{
    m_world = tm;
    if (m_debugCube != nullptr)
        m_debugCube->SetWorldTM(m_world);
}

void StaticMesh::SetActive(bool isActive)
{
    m_isActive = isActive;
    if (m_debugCube != nullptr)
        m_debugCube->SetActive(isActive);
}

// ASE 파서에서 버텍스 정보, 인덱스 정보를 얻어온다.
void StaticMesh::LoadStaticMesh(LPCSTR filename)		// 기하구조로부터 버텍스/인덱스버퍼를 만든다.
{
    if (strcmp(filename, meshID.c_str()) == 0)
        return;
    MZMeshData::MeshData* _pMeshData = ResourceManager::Instance.Get().LoadMeshData(filename);
    MZMeshData::Mesh* mesh = _pMeshData->GetMesh(0);
    meshID = filename;

    // Bounding box
    XMFLOAT3 vMinf3(+MathHelper::Infinity, +MathHelper::Infinity, +MathHelper::Infinity);
    XMFLOAT3 vMaxf3(-MathHelper::Infinity, -MathHelper::Infinity, -MathHelper::Infinity);

    XMVECTOR vMax = XMLoadFloat3(&vMaxf3);
    XMVECTOR vMin = XMLoadFloat3(&vMinf3);
    //XMVECTOR vSum{ 0.0f, 0.0f, 0.0f };
    //XMVECTOR vScalar{ 0.0f, 0.0f, 0.0f };

    for (UINT i = 0; i < mesh->m_meshvertex.size(); ++i)
    {
        XMVECTOR P = XMVECTOR(mesh->m_meshvertex[i]->m_pos);
        vMin = XMVectorMin(vMin, P);
        vMax = XMVectorMax(vMax, P);
        //vSum += P;
        //XMVECTOR Scalar = XMVECTOR{ abs(mesh->m_meshvertex[i]->m_pos.x), abs(mesh->m_meshvertex[i]->m_pos.y), abs(mesh->m_meshvertex[i]->m_pos.z) };
        //vScalar += Scalar;
    }

    //XMStoreFloat3(&m_meshBox.Center, 0.5f * (vMin + vMax));
    //XMStoreFloat3(&m_meshBox.Extents, 0.5f * (vMax - vMin));

    //XMVECTOR vAvg = vSum / mesh->m_meshvertex.size();
    //vScalar /= mesh->m_meshvertex.size();
    //m_meshBox.Center = XMFLOAT3(m_world.r[3].m128_f32[0],
    //	m_world.r[3].m128_f32[1] + 0.5f * (vMin.m128_f32[2] + vMax.m128_f32[2]),
    //	m_world.r[3].m128_f32[2]);
    //m_meshBox.Extents = XMFLOAT3(vScalar.m128_f32[0], vScalar.m128_f32[2], vScalar.m128_f32[1]);

    m_meshBox.Center = XMFLOAT3(0.5f * (vMin.m128_f32[0] + vMax.m128_f32[0]),
        0.5f * (vMin.m128_f32[2] + vMax.m128_f32[2]),
        0.5f * (vMin.m128_f32[1] + vMax.m128_f32[1]));

    m_meshBox.Extents = XMFLOAT3(0.5f * (vMax.m128_f32[0] - vMin.m128_f32[0]),
        0.5f * (vMax.m128_f32[2] - vMin.m128_f32[2]),
        0.5f * (vMax.m128_f32[1] - vMin.m128_f32[1]));

    m_debugCube = unique_ptr<DebugCube>(new DebugCube(m_d3dDevice.Get(), m_d3dImmediateContext.Get(), RasterizerState::WireframeRS.Get(), m_meshBox.Center, 2.0f * m_meshBox.Extents, XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)));
    m_debugCube->SetWorldTM(m_world);
    return;

    // 아래 코드는 사실상 레거시임.
    UINT vcount = 0;
    UINT faceCount = 0;
    UINT tcount = 0;

    faceCount = mesh->m_meshface.size();
    vcount = faceCount * 3;
    m_vertices.resize(vcount);

    for (int i = 0; i < faceCount; i++)
    {
        int vIndex0 = mesh->m_meshface[i]->m_vertexindex[0];
        int vIndex1 = mesh->m_meshface[i]->m_vertexindex[2];
        int vIndex2 = mesh->m_meshface[i]->m_vertexindex[1];
        int tIndex0 = mesh->m_meshface[i]->m_TFace[0];
        int tIndex1 = mesh->m_meshface[i]->m_TFace[2];
        int tIndex2 = mesh->m_meshface[i]->m_TFace[1];

        m_vertices[i * 3 + 0].Pos.x = mesh->m_meshvertex[vIndex0]->m_pos.x;
        m_vertices[i * 3 + 0].Pos.y = mesh->m_meshvertex[vIndex0]->m_pos.z;
        m_vertices[i * 3 + 0].Pos.z = mesh->m_meshvertex[vIndex0]->m_pos.y;

        m_vertices[i * 3 + 1].Pos.x = mesh->m_meshvertex[vIndex1]->m_pos.x;
        m_vertices[i * 3 + 1].Pos.y = mesh->m_meshvertex[vIndex1]->m_pos.z;
        m_vertices[i * 3 + 1].Pos.z = mesh->m_meshvertex[vIndex1]->m_pos.y;

        m_vertices[i * 3 + 2].Pos.x = mesh->m_meshvertex[vIndex2]->m_pos.x;
        m_vertices[i * 3 + 2].Pos.y = mesh->m_meshvertex[vIndex2]->m_pos.z;
        m_vertices[i * 3 + 2].Pos.z = mesh->m_meshvertex[vIndex2]->m_pos.y;

        for (int j = 0; j < 3; j++)
        {
            //int vIndex = meshData->m_meshface[i]->m_vertexindex[j];
            //int tIndex = meshData->m_meshface[i]->m_TFace[j];

            m_vertices[i * 3 + j].Normal.x = mesh->m_meshface[i]->m_normalvertex[j].x;
            m_vertices[i * 3 + j].Normal.y = mesh->m_meshface[i]->m_normalvertex[j].z;
            m_vertices[i * 3 + j].Normal.z = mesh->m_meshface[i]->m_normalvertex[j].y;

            /*m_vertices[i * 3 + j].Pos.x = meshData->m_meshvertex[vIndex]->m_pos.x;
            m_vertices[i * 3 + j].Pos.y = meshData->m_meshvertex[vIndex]->m_pos.z;
            m_vertices[i * 3 + j].Pos.z = meshData->m_meshvertex[vIndex]->m_pos.y;
            if (!meshData->m_mesh_tvertex.empty())
            {
                m_vertices[i * 3 + j].Tex.x = meshData->m_mesh_tvertex[tIndex]->m_u;
                m_vertices[i * 3 + j].Tex.y = meshData->m_mesh_tvertex[tIndex]->m_v;
            }*/

            // Bounding box
            XMVECTOR P = XMLoadFloat3(&m_vertices[i * 3 + j].Pos);

            XMVECTOR vMin = XMVectorMin(vMin, P);
            XMVECTOR vMax = XMVectorMax(vMax, P);

            // tangent 값 초기화			
            m_vertices[i * 3 + j].TangentU = Vector3(0.0f, 0.0f, 0.0f);

        }

        if (!mesh->m_mesh_tvertex.empty())
        {
            m_vertices[i * 3 + 0].Tex.x = mesh->m_mesh_tvertex[tIndex0]->m_u;
            m_vertices[i * 3 + 0].Tex.y = mesh->m_mesh_tvertex[tIndex0]->m_v;

            m_vertices[i * 3 + 1].Tex.x = mesh->m_mesh_tvertex[tIndex1]->m_u;
            m_vertices[i * 3 + 1].Tex.y = mesh->m_mesh_tvertex[tIndex1]->m_v;

            m_vertices[i * 3 + 2].Tex.x = mesh->m_mesh_tvertex[tIndex2]->m_u;
            m_vertices[i * 3 + 2].Tex.y = mesh->m_mesh_tvertex[tIndex2]->m_v;
        }
        /// CalculateTangents
        // 모든 삼각형에 대해 Tangent 계산하고, 삼각형의 3개의 버텍스에 Tangent 값을 누산함.
        const XMFLOAT3& p0 = m_vertices[i * 3 + 0].Pos;
        const XMFLOAT3& p1 = m_vertices[i * 3 + 1].Pos;
        const XMFLOAT3& p2 = m_vertices[i * 3 + 2].Pos;
        const XMFLOAT2& w0 = m_vertices[i * 3 + 0].Tex;
        const XMFLOAT2& w1 = m_vertices[i * 3 + 1].Tex;
        const XMFLOAT2& w2 = m_vertices[i * 3 + 2].Tex;

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
            m_vertices[i * 3 + j].TangentU.x += t.x;
            m_vertices[i * 3 + j].TangentU.y += t.y;
            m_vertices[i * 3 + j].TangentU.z += t.z;
        }
    }

    // 얻어진 tangent 벡터는 이 정점을 공유하고 있는 모든 삼각형들의 
    // tangent 벡터와 합산하여 계산된다.
    // 여러 개의 tangent 벡터를 합산하고 나면 T, B, N 벡터는 서로 수직이 아닐 수 있다.
    // 따라서 수직을 만들어 주는 과정을 추가해야 한다.
    // T, B, N 벡터를 모두 구하면 Normal을 탄젠트 공간에서 오브젝트 공간으로 변환가능하다.
    // B 벡터는 T, N 벡터의 외적으로 구할 수 있으므로 지금은 저장하지 않는다.
    for (int i = 0; i < vcount; ++i)
    {
        XMVECTOR n = XMLoadFloat3(&(m_vertices[i].Normal));
        XMVECTOR t = XMLoadFloat3(&(m_vertices[i].TangentU));
        Vector3 Tangent = XMVector3Normalize(t - XMVector3Dot(t, n) * n);
        m_vertices[i].TangentU = Tangent;
    }

    // Bouding box
    XMStoreFloat3(&m_meshBox.Center, 0.5f * (vMin + vMax));
    XMStoreFloat3(&m_meshBox.Extents, 0.5f * (vMax - vMin));


    tcount = mesh->m_meshface.size();
    IndexCount = 3 * tcount;
    m_indices.resize(IndexCount);
    for (UINT i = 0; i < tcount; ++i)
    {
        m_indices[i * 3 + 0] = i * 3 + 0;
        m_indices[i * 3 + 1] = i * 3 + 1;
        m_indices[i * 3 + 2] = i * 3 + 2;
    }

    D3D11_BUFFER_DESC vbd;
    vbd.Usage = D3D11_USAGE_IMMUTABLE;
    vbd.ByteWidth = sizeof(VertexStruct::NormalMap) * vcount;
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
    D3D11_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = &m_vertices[0];
    //HR(m_d3dDevice->CreateBuffer(&vbd, &vinitData, &m_VB));

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
    iinitData.pSysMem = &m_indices[0];
    //HR(m_d3dDevice->CreateBuffer(&ibd, &iinitData, &m_IB));

    //ReleaseASEParser(m_pASEParser);
}
void StaticMesh::LoadDiffuseTexture(const WCHAR* filename)
{
    ID3D11Resource* texResource = nullptr;
    HR(CreateDDSTextureFromFile(m_d3dDevice.Get(),
        filename, &texResource, &mDiffuseMapSRV));
    SafeRelease(texResource);	// view saves reference
}
void StaticMesh::LoadNormalTexture(const WCHAR* filename)
{
    ID3D11Resource* normalMapResource = nullptr;
    HR(CreateDDSTextureFromFile(m_d3dDevice.Get(),
        filename, &normalMapResource, &mNormalMapSRV));
    SafeRelease(normalMapResource);
}

bool StaticMesh::Pick(MZCamera* pCamera, float x, float y)
{
    if (!m_isActive)
        return false;

	if (!m_isPickingOn)
		return false;
    // Ray definition in view space.
    XMVECTOR rayOrigin = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
    XMVECTOR rayDir = XMVectorSet(x, y, 1.0f, 0.0f);

    // Tranform ray to local space of Mesh.
    XMMATRIX V = pCamera->View();
    XMVECTOR detView = XMMatrixDeterminant(V);
    XMMATRIX invView = XMMatrixInverse(&detView, V);

    //XMMATRIX W = XMLoadFloat4x4(&m_world);
    XMVECTOR detWorld = XMMatrixDeterminant(m_world);
    XMMATRIX invWorld = XMMatrixInverse(&detWorld, m_world);

    XMMATRIX toLocal = XMMatrixMultiply(invView, invWorld);

    rayOrigin = XMVector3TransformCoord(rayOrigin, toLocal);
    rayDir = XMVector3TransformNormal(rayDir, toLocal);

    // Make the ray direction unit length for the intersection tests.
    rayDir = XMVector3Normalize(rayDir);

    // If we hit the bounding box of the Mesh, then we might have picked a Mesh triangle,
    // so do the ray/triangle tests.
    //
    // If we did not hit the bounding box, then it is impossible that we hit 
    // the Mesh, so do not waste effort doing ray/triangle tests.

    float tmin = 0.0f;
    if (XNA::IntersectRayAxisAlignedBox(rayOrigin, rayDir, &m_meshBox, &tmin))
    {
        //// Find the nearest ray/triangle intersection.
        //tmin = MathHelper::Infinity;
        //for (UINT i = 0; i < m_indices.size() / 3; ++i)
        //{
        //	// Indices for this triangle.
        //	UINT i0 = m_indices[i * 3 + 0];
        //	UINT i1 = m_indices[i * 3 + 1];
        //	UINT i2 = m_indices[i * 3 + 2];

        //	// m_vertices for this triangle.
        //	XMVECTOR v0 = XMLoadFloat3(&m_vertices[i0].Pos);
        //	XMVECTOR v1 = XMLoadFloat3(&m_vertices[i1].Pos);
        //	XMVECTOR v2 = XMLoadFloat3(&m_vertices[i2].Pos);

        //	// We have to iterate over all the triangles in order to find the nearest intersection.
        //	float t = 0.0f;
        //	if (XNA::IntersectRayTriangle(rayOrigin, rayDir, v0, v1, v2, &t))
        //	{
        //		if (t < tmin)
        //		{
        //			// This is the new nearest picked triangle.
        //			tmin = t;
        //		}
        //	}
        //}
        m_depth = tmin;
        return true;
    }
    return false;
}

void StaticMesh::Update(MZCamera* pCamera, float deltaTime)
{
    if (!m_isActive)
        return;
    //XMMATRIX mWorldPos = scaleTM * rotateTM * translateTM;
    //XMStoreFloat4x4(&m_world, XMMatrixIdentity());
    //XMStoreFloat4x4(&m_world, mWorldPos);
    XMStoreFloat4x4(&m_view, pCamera->View());
    XMStoreFloat4x4(&m_proj, pCamera->Proj());

    //mEyePosW = XMFLOAT3(pCamera->GetPosition().x, pCamera->GetPosition().y, pCamera->GetPosition().z);
    if (m_debugCube != nullptr)
        m_debugCube->Update(pCamera, deltaTime);
}

void StaticMesh::RenderToTexture()
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
    auto mzMeshData = ResourceManager::Instance.Get().GetMeshData((LPCSTR)meshID.c_str());


    Effects::StaticMeshFX->SetWorld(m_world);
    Effects::StaticMeshFX->SetWorldInvTranspose(worldInvTranspose);
    Effects::StaticMeshFX->SetWorldViewProj(worldViewProj);
    Effects::StaticMeshFX->SetTexTransform(XMLoadFloat4x4(&mTexTransform));
    //Effects::StaticMeshFX->SetMaterial(m_material);
    Effects::StaticMeshFX->SetDiffuseMap(mDiffuseMapSRV);
    Effects::StaticMeshFX->SetNormalMap(mNormalMapSRV);

    if (mzMeshData)
    {
        for (auto each : mzMeshData->m_MeshList)
        {
            D3D11_BUFFER_DESC desc;
            auto& vbuffer = each->vertexBuffer_staticMesh;
            if (vbuffer.Get() == nullptr)
                continue;

            vbuffer->GetDesc(&desc);
            unsigned int stride = sizeof(VertexStruct::NormalMap);
            m_d3dImmediateContext->IASetVertexBuffers(0, 1, vbuffer.GetAddressOf(), &stride, &offset);
            mTech->GetPassByIndex(0)->Apply(0, m_d3dImmediateContext.Get());
            m_d3dImmediateContext->Draw(desc.ByteWidth / stride, 0);
        }
    }
    //m_d3dImmediateContext->IASetVertexBuffers(0, 1, &m_VB, &stride, 0);
    //mTech->GetPassByIndex(0)->Apply(0, m_d3dImmediateContext.Get());
    //m_d3dImmediateContext->Draw(IndexCount, 0);
    if (m_debugCube != nullptr)
        m_debugCube->RenderToTexture();
}
