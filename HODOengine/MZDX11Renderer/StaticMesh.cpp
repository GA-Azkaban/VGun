#include "StaticMesh.h"
#include "MZCamera.h"
#include "Mesh.h"
#include "Material.h"

StaticMesh::StaticMesh(::Mesh* mesh, Material* material)
    : m_meshName(), m_mesh(mesh), m_material(material), m_isActive(true), 
    m_world{ XMMatrixIdentity() }, m_position{ 0, 0, 0 }, m_rotation{ 0, 0, 0, 1 }, m_scale{ 1, 1, 1 },
    m_meshBox(), m_depth(1.0f), m_isPickingOn(true)
{

}

StaticMesh::~StaticMesh()
{

}

bool StaticMesh::Pick(float x, float y)
{
    if (!m_isActive)
        return false;

	if (!m_isPickingOn)
		return false;
    // Ray definition in view space.
    XMVECTOR rayOrigin = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
    XMVECTOR rayDir = XMVectorSet(x, y, 1.0f, 0.0f);

    // Tranform ray to local space of Mesh.
    XMMATRIX V = MZCamera::GetMainCamera()->View();
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

    float tmin = 0.0f;
    if (XNA::IntersectRayAxisAlignedBox(rayOrigin, rayDir, &m_meshBox, &tmin))
    {
        m_depth = tmin;
        return true;
    }
    return false;
}

void StaticMesh::Update(float deltaTime)
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

void StaticMesh::Render()
{
    if (!m_isActive)
        return;
    //// 입력 배치 객체 셋팅
    //m_d3dImmediateContext->IASetInputLayout(InputLayouts::StaticMeshGBufferInputLayout);

    //m_d3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    //// 렌더 스테이트
    //m_d3dImmediateContext->RSSetState(m_pRS.Get());


    ///// WVP TM등을 셋팅
    //// Set constants
    //XMMATRIX view = XMLoadFloat4x4(&m_view);
    //XMMATRIX proj = XMLoadFloat4x4(&m_proj);
    ////XMMATRIX world = XMLoadFloat4x4(&m_world);
    //XMMATRIX worldViewProj = m_world * view * proj;

    //// 월드의 역행렬
    //XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(m_world);

    //// Figure out which technique to use.
    //ID3DX11EffectTechnique* mTech = Effects::StaticMeshFX->GetGBufferTech;

    //// 테크닉은...
    //D3DX11_TECHNIQUE_DESC techDesc;
    //mTech->GetDesc(&techDesc);

    //UINT stride = sizeof(VertexStruct::NormalMap);
    //UINT offset = 0;
    //auto mzMeshData = ResourceManager::Instance.Get().GetMeshData((LPCSTR)meshID.c_str());


    //Effects::StaticMeshFX->SetWorld(m_world);
    //Effects::StaticMeshFX->SetWorldInvTranspose(worldInvTranspose);
    //Effects::StaticMeshFX->SetWorldViewProj(worldViewProj);
    //Effects::StaticMeshFX->SetTexTransform(XMLoadFloat4x4(&mTexTransform));
    ////Effects::StaticMeshFX->SetMaterial(m_material);
    //Effects::StaticMeshFX->SetDiffuseMap(mDiffuseMapSRV);
    //Effects::StaticMeshFX->SetNormalMap(mNormalMapSRV);

    //if (mzMeshData)
    //{
    //    for (auto each : mzMeshData->m_MeshList)
    //    {
    //        D3D11_BUFFER_DESC desc;
    //        auto& vbuffer = each->vertexBuffer_staticMesh;
    //        if (vbuffer.Get() == nullptr)
    //            continue;

    //        vbuffer->GetDesc(&desc);
    //        unsigned int stride = sizeof(VertexStruct::NormalMap);
    //        m_d3dImmediateContext->IASetVertexBuffers(0, 1, vbuffer.GetAddressOf(), &stride, &offset);
    //        mTech->GetPassByIndex(0)->Apply(0, m_d3dImmediateContext.Get());
    //        m_d3dImmediateContext->Draw(desc.ByteWidth / stride, 0);
    //    }
    //}
    //m_d3dImmediateContext->IASetVertexBuffers(0, 1, &m_VB, &stride, 0);
    //mTech->GetPassByIndex(0)->Apply(0, m_d3dImmediateContext.Get());
    //m_d3dImmediateContext->Draw(IndexCount, 0);
}
