#include <unordered_map>
#include <stack>
#include <codecvt>
#include "SkinnedMesh.h"
#include "MeshData.h"
#include "Effects.h"
#include "Node.h"
#include "ResourceManager.h"
#include "MZCamera.h"
#include "DDSTextureLoader.h"
#include "MathHelper.h"
#include "Vertex.h"

#include "DDSTextureLoader.h"
#include "DeferredBuffers.h"


SkinnedMesh::SkinnedMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, ID3D11RasterizerState* pRS)
    : md3dDevice(pDevice), md3dDeviceContext(pDeviceContext), m_pRenderstate(pRS),
    mVB(nullptr), mIB(nullptr),
    mDiffuseMapSRV(0), mEyePosW(0.0f, 0.0f, 0.0f)
{
    XMStoreFloat4x4(&mTexTransform, XMMatrixIdentity());

    mSpotLights[0].Ambient = XMFLOAT4(0.1f, 0.1f, 0.1f, 0.1f);
    mSpotLights[0].Diffuse = XMFLOAT4(100.0f, 100.0f, 100.f, 1.0f);
    mSpotLights[0].Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
    mSpotLights[0].Direction = XMFLOAT3(0.0f, -0.707f, -0.707f);
    mSpotLights[0].Att = XMFLOAT3(1, 0, 1);
    mSpotLights[0].Position = XMFLOAT3(0, 10, 10);
    mSpotLights[0].Range = 30;
    mSpotLights[0].Spot = 1;

    m_Material.Ambient = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
    m_Material.Diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
    m_Material.Specular = XMFLOAT4(0.8f, 0.8f, 0.8f, 16.0f);

    XMMATRIX skullScale = XMMatrixScaling(0.5f, 0.5f, 0.5f);
    XMMATRIX skullOffset = XMMatrixTranslation(0.0f, 1.0f, 0.0f);
    mWorld = XMMatrixMultiply(skullScale, skullOffset);

    mTech = Effects::YunuSkinnedMeshFX->defaultTech.Get();
}

SkinnedMesh::~SkinnedMesh()
{
    SafeRelease(mDiffuseMapSRV);

    //SafeRelease(mIB);
    //SafeRelease(mVB);
}

void SkinnedMesh::SetActive(bool isActive)
{
    m_isActive = isActive;
    animInstance.SetActive(isActive);
}

void SkinnedMesh::LoadMesh(LPCSTR fileName)
{
    meshID = fileName;
    if (auto skeleton = ResourceManager::Instance.Get().GetSkeletonData(fileName); skeleton != nullptr)
        animInstance.SetSkeleton(skeleton);
}

// ASE 파서에서 버텍스 정보, 인덱스 정보를 얻어온다.
void SkinnedMesh::LoadGeometry(ASE::Mesh* meshData, const XMMATRIX& basisTransform)
{
    LoadGeometry({ meshData }, basisTransform);
}
void SkinnedMesh::LoadGeometry(vector<ASE::Mesh*> meshDatas, const map<int, int>& nodeIdxByBoneIdx)
{
    //UINT tcount = 0;
    m_nodeIndexByName.clear();
    vertexCount = 0;
    int nodeIndex = 0;
    for (auto* meshData : meshDatas)
    {
        //mvcount += meshData->m_meshvertex.size();
        vertexCount += meshData->m_mesh_numfaces * 3;
        m_nodeIndexByName[meshData->m_nodename] = nodeIndex;
        nodeIndex++;
    }
    m_nodeSize = nodeIndex;
    m_nodeParents.resize(m_nodeSize);
    m_nodeLocalTM.resize(m_nodeSize);
    m_nodeLocalInitialPos.resize(m_nodeSize);
    m_nodeLocalInitialRot.resize(m_nodeSize);
    m_nodeLocalInitialScl.resize(m_nodeSize);
    m_nodeWorldTM.resize(m_nodeSize);
    m_nodeWorldTMCached.resize(m_nodeSize);
    m_nodeInitialWorldTM.resize(m_nodeSize);
    m_nodeInitialWorldTM_Inverse.resize(m_nodeSize);
    m_nodeVertexSize.resize(m_nodeSize);
    m_nodeVertexOffset.resize(m_nodeSize);
    std::vector<VertexStruct::PosNormalTexTanSkinned> vertices(vertexCount);

    nodeIndex = 0;
    for (auto* meshData : meshDatas)
    {
        if (m_nodeIndexByName.find(meshData->m_nodeparent) != m_nodeIndexByName.end())
            m_nodeParents[nodeIndex] = m_nodeIndexByName[meshData->m_nodeparent];
        else
            m_nodeParents[nodeIndex] = -1;
        m_nodeInitialWorldTM[nodeIndex] =
        {
            meshData->m_tm_row0.x,meshData->m_tm_row0.y,meshData->m_tm_row0.z, 0,
            meshData->m_tm_row1.x,meshData->m_tm_row1.y,meshData->m_tm_row1.z, 0,
            meshData->m_tm_row2.x,meshData->m_tm_row2.y,meshData->m_tm_row2.z, 0,
            meshData->m_tm_row3.x,meshData->m_tm_row3.y,meshData->m_tm_row3.z, 1
        };
        m_nodeInitialWorldTM_Inverse[nodeIndex] = XMMatrixInverse(nullptr, m_nodeInitialWorldTM[nodeIndex]);
        m_nodeWorldTM[nodeIndex] = m_nodeInitialWorldTM[nodeIndex];

        m_nodeVertexSize[nodeIndex] = meshData->m_mesh_numfaces * 3;
        m_nodeVertexOffset[nodeIndex] = 0;
        if (nodeIndex > 0)
        {
            m_nodeVertexOffset[nodeIndex] += m_nodeVertexOffset[nodeIndex - 1];
            m_nodeVertexOffset[nodeIndex] += m_nodeVertexSize[nodeIndex - 1];
        }
        nodeIndex++;
    }
    nodeIndex = 0;
    for (auto* meshData : meshDatas)
    {
        XMMATRIX WTMInvTrs = MathHelper::InverseTranspose(m_nodeWorldTM[nodeIndex]);
        XMMATRIX WTMInv = XMMatrixTranspose(WTMInvTrs);
        XMMATRIX parentWTMInv = XMMatrixIdentity();
        if (m_nodeParents[nodeIndex] >= 0)
            parentWTMInv = XMMatrixTranspose(MathHelper::InverseTranspose(m_nodeWorldTM[m_nodeParents[nodeIndex]]));
        m_nodeLocalTM[nodeIndex] = m_nodeWorldTM[nodeIndex] * parentWTMInv;
        XMMatrixDecompose(
            &m_nodeLocalInitialScl[nodeIndex],
            &m_nodeLocalInitialRot[nodeIndex],
            &m_nodeLocalInitialPos[nodeIndex],
            m_nodeLocalTM[nodeIndex]
        );

        //m_nodeWorldTM[nodeIndex] *= basisTransform;
        m_nodeWorldTMCached[nodeIndex] = true;
        UINT faceCount = 0;
        faceCount = meshData->m_meshface.size();

        for (int i = 0; i < faceCount; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                auto posIndex = meshData->m_meshface[i]->m_vertexindex[j];
                auto texIndex = meshData->m_meshface[i]->m_TFace[j];
                // apply inverse world transform right here!
                // especially, when it comes to normal, you must multiply inverse transpose world transform.
                XMVECTOR pos = meshData->m_meshvertex[posIndex]->m_pos;
                pos.m128_f32[3] = 1;
                XMVECTOR normal = meshData->m_meshface[i]->m_normalvertex[j];
                auto& vertex = vertices[m_nodeVertexOffset[nodeIndex] + i * 3 + j];

                XMStoreFloat3(&vertex.Pos, pos);
                XMStoreFloat3(&vertex.Normal, normal);

                const int& weightIndex0 = meshData->m_meshvertex[posIndex]->m_blend_indices[0];
                const int& weightIndex1 = meshData->m_meshvertex[posIndex]->m_blend_indices[1];
                const int& weightIndex2 = meshData->m_meshvertex[posIndex]->m_blend_indices[2];
                const int& weightIndex3 = meshData->m_meshvertex[posIndex]->m_blend_indices[3];

                const float& weight0 = meshData->m_meshvertex[posIndex]->m_blend_weights[0];
                const float& weight1 = meshData->m_meshvertex[posIndex]->m_blend_weights[1];
                const float& weight2 = meshData->m_meshvertex[posIndex]->m_blend_weights[2];
                const float& weight3 = meshData->m_meshvertex[posIndex]->m_blend_weights[3];

                vertex.BoneIndices0 = nodeIdxByBoneIdx.find(weightIndex0)->second;
                vertex.BoneIndices1 = nodeIdxByBoneIdx.find(weightIndex1)->second;
                vertex.BoneIndices2 = nodeIdxByBoneIdx.find(weightIndex2)->second;
                vertex.BoneIndices3 = nodeIdxByBoneIdx.find(weightIndex3)->second;

                vertex.Weights0 = weight0;
                vertex.Weights1 = weight1;
                vertex.Weights2 = weight2;

                // vertex blendWeight가 0인 경우, 이는 스키닝 메시에 포함된 디버그 메시다.
                constexpr bool showDebugMesh = false;
                if (vertex.Weights0 == 0.0f)
                {
                    if (showDebugMesh)
                    {
                        vertex.Weights0 = 1.0f;
                        vertex.BoneIndices0 = nodeIndex;
                    }
                    else
                    {
                        vertex.Pos = Vector3(0, 0, 0);
                    }
                }

                if (!meshData->m_mesh_tvertex.empty())
                {
                    vertex.Tex.x = meshData->m_mesh_tvertex[texIndex]->m_u;
                    vertex.Tex.y = 1 - meshData->m_mesh_tvertex[texIndex]->m_v;
                }
            }
        }

        nodeIndex++;
    }

    D3D11_BUFFER_DESC vbd;
    vbd.Usage = D3D11_USAGE_IMMUTABLE;
    vbd.ByteWidth = sizeof(VertexStruct::PosNormalTexTanSkinned) * vertexCount;
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
    D3D11_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = &vertices[0];
    HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mVB));
}

// 애니메이션 데이터를 로드한다.
void SkinnedMesh::LoadAnimation(const list<shared_ptr<Animation>>& nodeAnimList, const ASE::Scenedata& sceneData)
{
    m_Anim.assign(nodeAnimList.begin(), nodeAnimList.end());
    for (auto& eachNodeAnim : m_Anim)
    {
        auto nodeIndex = m_nodeIndexByName[eachNodeAnim->m_nodename];
        if (!eachNodeAnim->m_rotation.empty())
        {
            eachNodeAnim->m_rotation[0].m_rotQT_accumulation = m_nodeLocalInitialRot[nodeIndex];
            for (auto i = 1; i < eachNodeAnim->m_rotation.size(); i++)
                eachNodeAnim->m_rotation[i].m_rotQT_accumulation =
                eachNodeAnim->m_rotation[i - 1].m_rotQT_accumulation *
                Quaternion(XMQuaternionRotationAxis(eachNodeAnim->m_rotation[i].m_rot, eachNodeAnim->m_rotation[i].m_angle));
        }
    }

    m_animPosIndices = vector<int>(m_Anim.size(), -1);
    m_animRotIndices = vector<int>(m_Anim.size(), -1);
    m_animScaleIndices = vector<int>(m_Anim.size(), -1);
    m_animTickLength = 1 / float(sceneData.m_framespeed * sceneData.m_ticksperframe);
    m_animLength = (sceneData.m_lastframe - sceneData.m_firstframe) * (float)sceneData.m_ticksperframe * m_animTickLength;
    m_animStartOffset = sceneData.m_firstframe * (float)sceneData.m_ticksperframe * m_animTickLength;
    m_animElapsed = m_animStartOffset;
}
void SkinnedMesh::SetDiffuseMap(LPCSTR resourceID)
{
    diffuseMapID = resourceID;
    mDiffuseMapSRV = ResourceManager::Instance.Get().GetTextureSRV(resourceID);
}

void SkinnedMesh::SetNormalMap(LPCSTR resourceID)
{
	normalMapID = resourceID;
	mNormalMapSRV = ResourceManager::Instance.Get().GetTextureSRV(resourceID);
}

void SkinnedMesh::SetRootTM(const XMMATRIX& tm)
{
    m_rootTM = tm;
}
XMMATRIX SkinnedMesh::GetRootTM()
{
    return m_rootTM;
}
void SkinnedMesh::SetBoneLocalTM(const string& boneName, const XMMATRIX& tm)
{
    if (m_nodeIndexByName.find(boneName) == m_nodeIndexByName.end())
        return;

    auto index = m_nodeIndexByName[boneName];
    m_nodeLocalTM[index] = tm;
}

XMMATRIX SkinnedMesh::GetBoneWorldTM(const string& boneName)const
{
    return GetBoneWorldTM(m_nodeIndexByName.at(boneName));
}
XMMATRIX SkinnedMesh::GetBoneLocalTM(const string& boneName)const
{
    if (m_nodeIndexByName.find(boneName) == m_nodeIndexByName.end())
        return XMMatrixIdentity();

    auto index = m_nodeIndexByName.at(boneName);
    return m_nodeLocalTM[index];
}
void SkinnedMesh::Update(MZCamera* pCamera, float deltaTime)
{
	if (!m_isActive)
		return;
    // XMStoreFloat4x4(&mWorld, XMMatrixIdentity());
    mWorld = XMMatrixIdentity();
    XMStoreFloat4x4(&mView, pCamera->View());
    XMStoreFloat4x4(&mProj, pCamera->Proj());
    for (auto each : m_nodeWorldTMCached)
        each = false;

    mEyePosW = XMFLOAT3(pCamera->GetPosition().x, pCamera->GetPosition().y, pCamera->GetPosition().z);
    animInstance.Update(deltaTime);

    UpdateAnimation(deltaTime);
}
void SkinnedMesh::UpdateAnimation(float deltaTime)
{
	if (!m_isActive)
		return;
    m_animElapsed += deltaTime;

    // erase it after validating the first frame
    //m_animTickLength = 100000;
    if (m_animElapsed - m_animStartOffset > m_animLength)
    {
        m_animElapsed -= m_animLength;

        for (int animIndex = 0; animIndex < m_Anim.size(); animIndex++)
        {
            if (m_animPosIndices[animIndex] >= 0)
                m_animPosIndices[animIndex] = 0;
            if (m_animRotIndices[animIndex] >= 0)
                m_animRotIndices[animIndex] = 0;
            if (m_animScaleIndices[animIndex] >= 0)
                m_animScaleIndices[animIndex] = 0;
        }
    }

    // index를 업데이트하는 템플릿 람다 함수
    // CAnimation_pos와 rot, scl을 같은 인터페이스로 다룰 수 없어 만듬.
    auto updateAnimIndex = [&]<typename JsonStructType>(const vector<JsonStructType>&keys, int& index)
    {
        if (index == keys.size() - 1)
            return;

        while (keys[index + 1].m_time * m_animTickLength < m_animElapsed)
        {
            index++;
            if (index == keys.size() - 1)
                break;
        }
    };
    auto getLerpTime = [this]<typename JsonStructType>(const vector<JsonStructType>&keys, int& index)
    {
        float ret =
            float(m_animElapsed / m_animTickLength - keys[index].m_time) / float(keys[index + 1].m_time - keys[index].m_time);
        return -2 * ret * ret * ret + 3 * ret * ret;
    };

    for (int animIndex = 0; animIndex < m_Anim.size(); animIndex++)
    {
        float tPos = 0;
        float tRot = 0;
        float tScl = 0;
        int& posIndex = m_animPosIndices[animIndex];
        int& rotIndex = m_animRotIndices[animIndex];
        int& sclIndex = m_animScaleIndices[animIndex];
        Animation& anim = *m_Anim[animIndex].get();
        updateAnimIndex(anim.m_position, posIndex);
        updateAnimIndex(anim.m_rotation, rotIndex);
        updateAnimIndex(anim.m_scale, sclIndex);

        int nodeIndex = m_nodeIndexByName[anim.m_nodename];

        XMVECTOR transLerped = m_nodeLocalInitialPos[nodeIndex];
        XMVECTOR rotLerped = m_nodeLocalInitialRot[nodeIndex];
        XMVECTOR sclLerped = m_nodeLocalInitialScl[nodeIndex];

        // 애니메이션 데이터가 있을시, index는 0 이상이다. 그 경우 애니메이션 데이터를 이용한다.
        if (posIndex >= 0)
            transLerped = anim.m_position[posIndex].m_pos;
        if (rotIndex >= 0)
            rotLerped = anim.m_rotation[rotIndex].m_rotQT_accumulation;
        if (sclIndex >= 0)
            sclLerped = anim.m_scale[sclIndex].m_scale;

        // 인덱스가 마지막이 아닐 경우, 보간을 해줘야 한다.
        if (posIndex < anim.m_position.size() - 1)
            transLerped = XMVectorLerp(anim.m_position[posIndex].m_pos,
                anim.m_position[posIndex + 1].m_pos, getLerpTime(anim.m_position, posIndex));
        if (rotIndex < anim.m_rotation.size() - 1)
            rotLerped = XMQuaternionSlerp(anim.m_rotation[rotIndex].m_rotQT_accumulation,
                anim.m_rotation[rotIndex + 1].m_rotQT_accumulation, getLerpTime(anim.m_rotation, rotIndex));
        if (sclIndex < anim.m_scale.size() - 1)
            sclLerped = XMVectorLerp(anim.m_scale[sclIndex].m_scale,
                anim.m_scale[sclIndex + 1].m_scale, getLerpTime(anim.m_scale, sclIndex));

        XMMATRIX transTM = XMMatrixTranslationFromVector(transLerped);
        XMMATRIX rotTM = XMMatrixRotationQuaternion(rotLerped);
        XMMATRIX sclTM = XMMatrixScalingFromVector(sclLerped);

        m_nodeLocalTM[nodeIndex] = sclTM * rotTM * transTM;
        //m_nodeLocalTM[nodeIndex] = transTM * rotTM * sclTM;
    }

}

void SkinnedMesh::RenderToTexture()
{
	if (!m_isActive)
		return;
    // 입력 배치 객체 셋팅
    md3dDeviceContext->IASetInputLayout(InputLayouts::YunuSkinnedMeshGBufferInputLayout);
    md3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // 렌더 스테이트
    md3dDeviceContext->RSSetState(m_pRenderstate);

    // 버텍스버퍼와 인덱스버퍼 셋팅
    UINT stride = sizeof(VertexStruct::PosNormalTexTanSkinned);
    UINT offset = 0;

    /// WVP TM등을 셋팅
    // Set constants
    XMMATRIX view = XMLoadFloat4x4(&mView);
    XMMATRIX proj = XMLoadFloat4x4(&mProj);
    //XMMATRIX world = XMLoadFloat4x4(&mWorld);
    XMMATRIX worldViewProj = mWorld * view * proj;

    // 월드의 역전치행렬
    XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(mWorld);

    Effects::YunuSkinnedMeshFX->SetEyePosW(mEyePosW);

    // Figure out which technique to use.
    D3DX11_TECHNIQUE_DESC techDesc;
    mTech->GetDesc(&techDesc);
    Effects::YunuSkinnedMeshFX->SetWorld(mWorld);
    Effects::YunuSkinnedMeshFX->SetWorldInvTranspose(worldInvTranspose);
    Effects::YunuSkinnedMeshFX->SetWorldViewProj(worldViewProj);
    Effects::YunuSkinnedMeshFX->SetTexTransform(XMLoadFloat4x4(&mTexTransform));
    Effects::YunuSkinnedMeshFX->SetMaterial(m_Material);


    Effects::YunuSkinnedMeshFX->SetDiffuseMap(mDiffuseMapSRV);

    auto meshData{ ResourceManager::Instance.Get().GetMeshData(meshID.c_str()) };
    if (meshData == nullptr)
        return;

    auto bones{ meshData->skeletonData->GetBones() };
    auto nodeSize = bones.size();
    vector<XMMATRIX> nodeFinalMatrices(nodeSize);

    // 이 부분을 리소스 매니저에서 받아오는 걸로 수정해야 한다.
    for (UINT i = 0; i < nodeSize; i++)
    {
        auto quat= XMQuaternionIdentity();
        // 본 스페이스의 버텍스 위치를 구한 다음, 월드 본 스페이스의 위치로 다시 버텍스를 날린다.
        //nodeFinalMatrices[i] = bones[i].initialTM_inverse * bones[i].initialTM;
        nodeFinalMatrices[i] = animInstance.targetSkeleton->GetBones()[i].initialTM_inverse * animInstance.boneTMs[i];
        nodeFinalMatrices[i] = nodeFinalMatrices[i] * m_rootTM;
        //nodeFinalMatrices[i] = m_nodeInitialWorldTM_Inverse[i] * GetBoneWorldTM(i);
    }
    Effects::YunuSkinnedMeshFX->SetBoneFinalMatrix2(nodeFinalMatrices);

    for (UINT p = 0; p < techDesc.Passes; ++p)
    {
        if (meshData == nullptr)
            continue;

        for (auto eachMesh : ResourceManager::Instance.Get().GetMeshData(meshID.c_str())->m_MeshList)
        {
            if (eachMesh->vertexBuffer_skinnedMesh == nullptr)
                continue;

            D3D11_BUFFER_DESC desc;
            eachMesh->vertexBuffer_skinnedMesh->GetDesc(&desc);

            md3dDeviceContext->IASetVertexBuffers(0, 1,
                eachMesh->vertexBuffer_skinnedMesh.GetAddressOf(), &stride, &offset);
            Effects::YunuSkinnedMeshFX->deferredGeometryTech->GetPassByIndex(0)->Apply(0, md3dDeviceContext);
            md3dDeviceContext->Draw(desc.ByteWidth / stride, 0);
        }
    }
}

XMMATRIX SkinnedMesh::GetBoneWorldTM(int boneIndex)const
{
    if (m_nodeWorldTMCached[boneIndex])
        return m_nodeWorldTM[boneIndex] * m_rootTM;

    stack<int> hierarchy;
    hierarchy.push(boneIndex);
    XMMATRIX ret = XMMatrixIdentity();
    while (!m_nodeWorldTMCached[hierarchy.top()])
    {
        if (m_nodeParents[hierarchy.top()] < 0)
        {
            m_nodeWorldTM[hierarchy.top()] = m_nodeLocalTM[hierarchy.top()];
            m_nodeWorldTMCached[hierarchy.top()] = true;
            break;
        }
        hierarchy.push(m_nodeParents[hierarchy.top()]);
    }
    ret = m_nodeWorldTM[hierarchy.top()];
    hierarchy.pop();

    while (!hierarchy.empty())
    {
        ret = m_nodeLocalTM[hierarchy.top()] * ret;
        m_nodeWorldTM[hierarchy.top()] = ret;
        m_nodeWorldTMCached[hierarchy.top()] = true;
        hierarchy.pop();
    }
    return ret * m_rootTM;
}