#include "MeshData.h"
#include "DX11Define.h"
#include <d3d11.h>

MZMeshData::Vertex::Vertex()
    : m_pos{ 0, 0, 0 }, m_normal{ 0, 0, 0 }, u(0.0f), v(0.0f), m_indices(0),
    m_bone_blenging_weights{ 0.0f, }, m_bone_blending_indices{ 0, }
{

}

MZMeshData::Face::Face()
    : m_vertexindex{ 0, 0, 0 }, m_normal{ 0, 0, 0 }, m_TFace{ 0, 0, 0 },
    m_normalvertex{ {0, 0, 0}, {0, 0, 0}, {0, 0, 0} }
{

}

MZMeshData::TextureVertex::TextureVertex()
    : m_u(0.0f), m_v(0.0f), m_w(0.0f)
{

}


MZMeshData::Mesh::Mesh()
    : m_timevalue(0.0f), m_numbone(0), m_mesh_numvertex(0), m_mesh_numfaces(0),
    m_mesh_numtvertex(0), m_mesh_tvfaces(0), m_isparentexist(false),
    m_inherit_pos{ 0, 0, 0 }, m_inherit_rot{ 0, 0, 0 }, m_inherit_scl{ 0, 0, 0 },
    m_tm_row0{ 0, 0, 0 }, m_tm_row1{ 0, 0, 0 }, m_tm_row2{ 0, 0, 0 }, m_tm_row3{ 0, 0, 0 },
    m_tm_pos{ 0, 0, 0 }, m_tm_rotaxis{ 0, 0, 0 }, m_tm_rotangle(0.0f),
    m_tm_scale{ 0, 0, 0 }, m_tm_scaleaxis{ 0, 0, 0 }, m_tm_scaleaxisang(0.0f)
{

}
MZMeshData::Mesh::~Mesh()
{
    for (auto* each : m_meshvertex)
        delete each;

    for (auto* each : m_meshface)
        delete each;

    for (auto* each : m_mesh_tvertex)
        delete each;
}

MZMeshData::MeshData::~MeshData()
{
    for (UINT i = 0; i < m_MeshList.size(); ++i)
    {
        delete m_MeshList[i];
        m_MeshList[i] = nullptr;
    }
}

template<>
void MZMeshData::Mesh::SplitAndGenerateVertexBuffer<VertexStruct::NormalMap>()
{
    UINT vcount = 0;
    UINT faceCount = 0;
    UINT tcount = 0;

    faceCount = m_meshface.size();
	if (faceCount <= 0)
		return;
    vcount = faceCount * 3;
    std::vector<VertexStruct::NormalMap> vertices(vcount);

    for (int i = 0; i < faceCount; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            vertices[i * 3 + j].Normal.x = m_meshface[i]->m_normalvertex[j].x;
            vertices[i * 3 + j].Normal.y = m_meshface[i]->m_normalvertex[j].z;
            vertices[i * 3 + j].Normal.z = m_meshface[i]->m_normalvertex[j].y;
            vertices[i * 3 + j].TangentU = Vector3(0.0f, 0.0f, 0.0f);
        }
        int vIndex0 = m_meshface[i]->m_vertexindex[0];
        int vIndex1 = m_meshface[i]->m_vertexindex[2];
        int vIndex2 = m_meshface[i]->m_vertexindex[1];
        int tIndex0 = m_meshface[i]->m_TFace[0];
        int tIndex1 = m_meshface[i]->m_TFace[2];
        int tIndex2 = m_meshface[i]->m_TFace[1];

        vertices[i * 3 + 0].Pos.x = m_meshvertex[vIndex0]->m_pos.x;
        vertices[i * 3 + 0].Pos.y = m_meshvertex[vIndex0]->m_pos.z;
        vertices[i * 3 + 0].Pos.z = m_meshvertex[vIndex0]->m_pos.y;

        vertices[i * 3 + 1].Pos.x = m_meshvertex[vIndex1]->m_pos.x;
        vertices[i * 3 + 1].Pos.y = m_meshvertex[vIndex1]->m_pos.z;
        vertices[i * 3 + 1].Pos.z = m_meshvertex[vIndex1]->m_pos.y;

        vertices[i * 3 + 2].Pos.x = m_meshvertex[vIndex2]->m_pos.x;
        vertices[i * 3 + 2].Pos.y = m_meshvertex[vIndex2]->m_pos.z;
        vertices[i * 3 + 2].Pos.z = m_meshvertex[vIndex2]->m_pos.y;

        if (!m_mesh_tvertex.empty())
        {
            vertices[i * 3 + 0].Tex.x = m_mesh_tvertex[tIndex0]->m_u;
            vertices[i * 3 + 0].Tex.y = m_mesh_tvertex[tIndex0]->m_v;

            vertices[i * 3 + 1].Tex.x = m_mesh_tvertex[tIndex1]->m_u;
            vertices[i * 3 + 1].Tex.y = m_mesh_tvertex[tIndex1]->m_v;

            vertices[i * 3 + 2].Tex.x = m_mesh_tvertex[tIndex2]->m_u;
            vertices[i * 3 + 2].Tex.y = m_mesh_tvertex[tIndex2]->m_v;
        }
        // face
        else if (m_texture_sets.size() > 0)
        {
            // 지금은 쓰이는 모든 텍스처의 uv좌표가 다 같다고 생각한다.
            vertices[i * 3 + 0].Tex.x = m_meshface[i]->UVs[0][0].x;
            vertices[i * 3 + 0].Tex.y = m_meshface[i]->UVs[0][0].y;

            vertices[i * 3 + 1].Tex.x = m_meshface[i]->UVs[1][0].x;
            vertices[i * 3 + 1].Tex.y = m_meshface[i]->UVs[1][0].y;

            vertices[i * 3 + 2].Tex.x = m_meshface[i]->UVs[2][0].x;
            vertices[i * 3 + 2].Tex.y = m_meshface[i]->UVs[2][0].y;
        }
        // 모든 삼각형에 대해 Tangent 계산하고, 삼각형의 3개의 버텍스에 Tangent 값을 누산함.
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
    for (int i = 0; i < vcount; ++i)
    {
        XMVECTOR n = XMLoadFloat3(&(vertices[i].Normal));
        XMVECTOR t = XMLoadFloat3(&(vertices[i].TangentU));
        Vector3 Tangent = XMVector3Normalize(t - XMVector3Dot(t, n) * n);
        vertices[i].TangentU = Tangent;
    }

    D3D11_BUFFER_DESC vbd;
    vbd.Usage = D3D11_USAGE_IMMUTABLE;
    vbd.ByteWidth = sizeof(VertexStruct::NormalMap) * vcount;
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
    //vbd.StructureByteStride = sizeof(VertexStruct::NormalMap);
    D3D11_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = &vertices[0];
    HR(DeferredRenderer::Instance.Get().GetDevice()->CreateBuffer(&vbd, &vinitData, vertexBuffer_staticMesh.GetAddressOf()));
    vertexBuffer_staticMesh->GetDesc(&vbd);
}
template<>
void MZMeshData::Mesh::SplitAndGenerateVertexBuffer<VertexStruct::PosNormalTexTanSkinned>()
{
    //UINT tcount = 0;
    auto vertexCount = m_meshface.size() * 3;
    std::vector<VertexStruct::PosNormalTexTanSkinned> vertices(vertexCount);

    UINT faceCount = 0;
    faceCount = m_meshface.size();

    for (int i = 0; i < faceCount; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            auto posIndex = m_meshface[i]->m_vertexindex[j];
            //auto texIndex = m_meshface[i]->m_TFace[j];
            // apply inverse world transform right here!
            // especially, when it comes to normal, you must multiply inverse transpose world transform.
            XMVECTOR pos = m_meshvertex[posIndex]->m_pos;
            pos.m128_f32[3] = 1;
            XMVECTOR normal = m_meshface[i]->m_normalvertex[j];
            auto& vertex = vertices[i * 3 + j];

            XMStoreFloat3(&vertex.Pos, pos);
            XMStoreFloat3(&vertex.Normal, normal);

            const int& weightIndex0 = m_meshvertex[posIndex]->m_bone_blending_indices[0];
            const int& weightIndex1 = m_meshvertex[posIndex]->m_bone_blending_indices[1];
            const int& weightIndex2 = m_meshvertex[posIndex]->m_bone_blending_indices[2];
            const int& weightIndex3 = m_meshvertex[posIndex]->m_bone_blending_indices[3];

            const float& weight0 = m_meshvertex[posIndex]->m_bone_blenging_weights[0];
            const float& weight1 = m_meshvertex[posIndex]->m_bone_blenging_weights[1];
            const float& weight2 = m_meshvertex[posIndex]->m_bone_blenging_weights[2];
            const float& weight3 = m_meshvertex[posIndex]->m_bone_blenging_weights[3];

            vertex.BoneIndices0 = weightIndex0;
            vertex.BoneIndices1 = weightIndex1;
            vertex.BoneIndices2 = weightIndex2;
            vertex.BoneIndices3 = weightIndex3;

            vertex.Weights0 = weight0;
            vertex.Weights1 = weight1;
            vertex.Weights2 = weight2;

            // vertex blendWeight가 0인 경우, 이는 스키닝 메시에 포함된 디버그 메시다.
            constexpr bool showDebugMesh = false;
            if (vertex.Weights0 == 0.0f)
            {
                if (showDebugMesh)
                {
                    //vertex.Weights0 = 1.0f;
                    //vertex.BoneIndices0 = nodeIndex;
                }
                else
                {
                    vertex.Pos = Vector3(0, 0, 0);
                }
            }

            //모든 맵들(normal, diffuse, regardless..)에 대해 같은 uv 좌표를 가지는 것으로 가정한다.
            vertex.Tex.x = 1 - m_meshface[i]->UVs[j][0].x;
            vertex.Tex.y = 1 - m_meshface[i]->UVs[j][0].y;
        }
    }


    D3D11_BUFFER_DESC vbd;
    vbd.Usage = D3D11_USAGE_IMMUTABLE;
    vbd.ByteWidth = sizeof(VertexStruct::PosNormalTexTanSkinned) * vertexCount;
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
    D3D11_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = &vertices[0];
    HR(DeferredRenderer::Instance.Get().GetDevice()->CreateBuffer(&vbd, &vinitData, vertexBuffer_skinnedMesh.GetAddressOf()));
}
