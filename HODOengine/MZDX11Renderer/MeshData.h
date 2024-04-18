#pragma once
#include <vector>
#include <map>
#include <string>
#include <SimpleMath.h>
#include <d3d11.h>
#include <wrl.h>
#include <array>
#include "DX11Define.h"
#include "Vertex.h"
#include "DeferredRenderer.h"
#include "Skeleton.h"

using namespace std;
using namespace DirectX::SimpleMath;

/// ResourceManager에 Mesh Data들을 저장하기 위한 구조체들.
/// 데이터들에 여기저기서 접근해야 되기 때문에 
/// 기본적으로 public으로 설정되어 있는 구조체로 만들었다.
/// 2023.07.27 MJKIM

namespace MZMeshData
{
    struct Vertex
    {
        Vertex();

        // 좌표상의 위치값
        Vector3 m_pos;

        // 노말값
        Vector3 m_normal;

        // 텍스쳐 좌표
        float u, v;

        /// Skinned Mesh 에서 추가
        UINT m_skinnedBoneCount{ 0 };
        std::array<float, 4> m_bone_blenging_weights;
        std::array<UINT, 4> m_bone_blending_indices;

        // 인덱스
        int m_indices;

    };

    struct Face
    {
        Face();

        // 이 Face를 이루는 버텍스의 인덱스
        int				m_vertexindex[3];
        // Face의 Normal값
        Vector3			m_normal;
        // Vertex의 Normal값
        Vector3			m_normalvertex[3];

        // Texture Coordinate
        int				m_TFace[3];

        // UV 좌표, diffuse map에 대한 좌표일수도, normal map에 대한 좌표일수도 있음.
        // n번째 인덱스에 해당하는 버텍스의 m번째 텍스처에 대응되는 UV 좌표는 UVs[n][m]이다.
        vector<Vector2> UVs[3];

    };

    struct TextureVertex
    {
        TextureVertex();

        float m_u, m_v, m_w;
    };

    struct Mesh
    {
        Mesh();
        ~Mesh();
        template<typename VertexStructType>
        void SplitAndGenerateVertexBuffer();
        Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer_staticMesh{nullptr};
        Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer_staticMesh{nullptr};
        Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer_skinnedMesh{nullptr};
        Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer_skinnedMesh{nullptr};

        ///----------------------------------
        /// *MESH
        ///----------------------------------
        float m_timevalue;			// 여러 곳에서 여러가지로 쓰인다. 여튼 시간값.
        int	m_numbone;			// 본의 갯수. 이것이 있다면 스키닝 오브젝트로 간주한다.
        int	m_mesh_numvertex;		// (버텍스의 좌표데이터)
        int	m_mesh_numfaces;		// (버텍스의 인덱스)

        /// *MESH_VERTEX_LIST
        vector<Vertex*>	m_meshvertex;	// 버텍스
        /// *MESH_FACE_LIST
        vector<Face*> m_meshface;		// 인덱스

        // 텍스쳐용 데이터들
        /// *MESH_NUMTVERTEX
        vector<TextureVertex*> m_mesh_tvertex;		// 텍스쳐용 버텍스
        int	m_mesh_numtvertex;		// 텍스쳐용 버텍스 갯수
        int	m_mesh_tvfaces;			// 텍스쳐용 페이스 갯수
        vector<std::string> m_texture_sets; // 텍스처의 종류에 대한 식별자들 ex) normal, diffuse...

        ///----------------------------------
        /// *NODE_NAME
        ///---------------------------------
        std::string	m_nodename;
        std::string	m_nodeparent;
        bool m_isparentexist;

        ///----------------------------------
        /// *NODE_TM (Transform Matrix)
        ///----------------------------------
        Vector3	m_inherit_pos;
        Vector3	m_inherit_rot;
        Vector3	m_inherit_scl;
        Vector3	m_tm_row0;
        Vector3	m_tm_row1;
        Vector3	m_tm_row2;
        Vector3	m_tm_row3;
        Vector3	m_tm_pos;
        Vector3	m_tm_rotaxis;
        float m_tm_rotangle;
        Vector3	m_tm_scale;
        Vector3	m_tm_scaleaxis;
        float m_tm_scaleaxisang;

        ///----------------------------------
        /// Animation
        ///----------------------------------
        vector<pair<int, Vector3>> pos_sample;
        vector<pair<int, Vector4>> rot_sample;
    };

    struct MeshData
    {
        MeshData() {};
        ~MeshData();

        vector<Mesh*> m_MeshList;
        std::map<string, int> m_boneList;
        unique_ptr<Skeleton> skeletonData;

        Mesh* GetMesh(UINT index)
        {
            return m_MeshList[index];
        }
    };
}

//namespace MZFBXMeshData
//{
//	struct Vertex
//	{
//		//Vertex();
//
//		Vector3 pos;
//		Vector3 normal;
//		Vector2 uvCoord;
//	};
//	
//	struct Mesh
//	{
//		std::vector<Vertex> m_vertexList;
//		std::vector<UINT> m_indicesList;
//	};
//
//	// Mesh가 여러 개 있을 수 있는데, 그 Mesh들을 벡터로 저장하고 있다.
//	struct MeshData
//	{
//		MeshData() {};
//		~MeshData();	
//
//		vector<Mesh*> m_MeshList;
//
//		Mesh* GetMesh(UINT index)
//		{
//			return m_MeshList[index];
//		}
//	};
//}

