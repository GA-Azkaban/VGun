#include "ASEParser.h"
#include <map>
#include <regex>
#include <algorithm>

ASEParser::ASEParser()
{
    m_materialcount = 0;
    m_parsingmode = eNone;
}

ASEParser::~ASEParser()
{
    delete m_lexer;
}

bool ASEParser::Init()
{
    m_lexer = new ASE::ASELexer;

    return TRUE;
}
//---------------------------------------------------------------------------------------------------
// 로딩한다.
// 이것이 끝났다면 정해진 데이터형에 모든 데이터를 읽어서 들어가 있어야 한다.
//
// Parsing에 대해:
// 항상 느끼는 것이지만 parsing이라는 것은 데이터가 일정 규격에 맞게 들어가 있다는 것을 전제로 한다.
// 하지만, 파일 내부에 순서가 바뀌어 들어가 있는것이 가능하다던지 하는 규칙이 생기면
// 검색하는 루틴이 복잡해지기 마련. 일단은 순서대로 일정 규약으로 들어가 있다는것을 가정하자.
// -> 그래서, 재귀 호출을 하는 하나의 큰 함수로 해결봤다.
// -> depth를 기준으로 오동작에 대한 안전 코드를 넣어야겠다
//---------------------------------------------------------------------------------------------------
bool ASEParser::Load(LPSTR p_File)
{
    ClearLoadedData();
    /// 0) 파일을 로드한다.
    if (!m_lexer->Open(p_File))
    {
        TRACE("파일을 여는 중에 문제가 발생했습니다!");
        return FALSE;
    }

    /// 1) 다롱이아부지김교수의 파워풀한 재귀함수 한개로 처리 끝!
    Parsing_DivergeRecursiveALL(0, {});

    return TRUE;
}

/// 1차 변환

// CScenedata 복사
bool ASEParser::TranslateToD3DFormat_scene(Scenedata* pscene)
{
    // 값 복사
    *pscene = this->m_scenedata;
    return TRUE;
}

/// <summary>
/// 메시의 최적화를 해 준다.
/// 노말값, 텍스쳐 좌표에 따라 버텍스를 늘리고, 중첩되는것은 제거하고..
/// </summary>
bool ASEParser::Convert_Optimize(Mesh* pMesh)
{
    // 2021.04.12
    // 구현 제거 (예시용)

    return FALSE;
}

bool ASEParser::ConvertAll(Mesh* pMesh)
{
    // 버텍스들은 일단 모두 복사
    for (unsigned int i = 0; i < pMesh->m_meshvertex.size(); i++)
    {
        Vertex* _pVertex = new Vertex;
        _pVertex->m_pos = pMesh->m_meshvertex[i]->m_pos;

        pMesh->m_opt_vertex.push_back(_pVertex);
    }

    // 나머지는 face를 기준으로 한 인덱스로 찾아야 함
    unsigned int _faceCount = pMesh->m_meshface.size();
    TRACE("FaceCount : %d", _faceCount);

    for (unsigned int i = 0; i < pMesh->m_meshface.size(); i++)
    {
        TRACE("FaceNum : %d / %d\n", i, _faceCount);

        for (int j = 0; j < 3; j++)
        {
            Vertex* _nowVertex = pMesh->m_opt_vertex[pMesh->m_meshface[i]->m_vertexindex[j]];

            // face기준으로 버텍스의 노말을 넣어준다.
            // (예시용)
            _nowVertex->m_normal.x = 0;
            _nowVertex->m_normal.y = 0;
            _nowVertex->m_normal.z = 0;
        }
    }

    // 인덱스는 그냥 복사
    pMesh->m_opt_index = new IndexList[pMesh->m_meshface.size()];

    for (unsigned int i = 0; i < pMesh->m_meshface.size(); i++)
    {
        for (int j = 0; j < 3; j++)
        {
            pMesh->m_opt_index[i].index[j] = pMesh->m_meshface[i]->m_vertexindex[j];
        }
    }

    return FALSE;
}

ASE::Mesh* ASEParser::GetMesh(int index)
{
    return m_MeshList[index].get();
}
vector<ASE::Mesh*> ASEParser::GetMeshes()
{
    vector<ASE::Mesh*> rawPtrs;
    std::transform(m_MeshList.begin(), m_MeshList.end(), back_inserter(rawPtrs),
        [](const std::unique_ptr<ASE::Mesh>& ptr) {return ptr.get(); });
    return rawPtrs;
}

//----------------------------------------------------------------
// 재귀 호출됨을 전제로 하는 분기 함수이다.
//
// 일단 읽고, 토큰에 따라 동작한다.
//
// 재귀를 들어가는 조건은 { 을 만날 경우이다.
// 리턴하는 조건은 '}'를 만나거나 TOKEND_END를 만났을 때이다.
//
// 더불어, 재귀를 들어가기 전 토큰을 읽어, 지금 이 함수가 어떤 모드인지를 결정한다.
//
//----------------------------------------------------------------
void ASEParser::Parsing_DivergeRecursiveALL(int depth, vector<LONG> parents)
{
    //----------------------------------------------------------------------
    // 지역변수들 선언
    // 재귀 돌리다가 일정 이상 루프를 도는 경우(오류)를 체크하기 위한 변수
    int i = 0;
    // 리스트에 데이터를 넣어야 할 때 쓰이는 리스트 카운터.
    int listcount = 0;		// 매우 C스러운데 다른 해결책이 없을까?
    // 이것은 현 토큰이 어떤 종류인가를 판별해준다.
    // 이것을 멤버 변수로 하고 재귀를 돌리는것은 위험하다? -> 잘못된 재귀방식이다?
    LONG nowtoken;
    LONG lastToken;
    //----------------------------------------------------------------------

    Matrix TM1, TM2;
    Quaternion tempQT;
    Quaternion prevQT;
    Quaternion resultQT;
    static int currentVertexIndex;
    static int currentBoneIndex;
    static int currentMaterialIndex;

    /// 이것을 하면 한 개의 토큰을 읽고, 그 종류와 내용을 알 수 있다.
    while (nowtoken = m_lexer->GetToken(m_TokenString), nowtoken != TOKEND_BLOCK_END)
    {
        // 일단 한 개의 토큰을 읽고, 그것이 괄호 닫기가 아니라면.
        // 넘어온 토큰에 따라 처리해준다.

        static int iv = 0;

        switch (nowtoken)
        {
        case TOKEND_BLOCK_START:
        {
            vector<LONG> nextParents = parents;
            nextParents.push_back(lastToken);
            Parsing_DivergeRecursiveALL(depth++, nextParents);
            break;
        }
        case TOKENR_HELPER_CLASS:
            break;

            //--------------------
            // 3DSMAX_ASCIIEXPORT
            //--------------------

        case TOKENR_3DSMAX_ASCIIEXPORT:
            m_data_asciiexport = Parsing_NumberLong();
            break;

            //--------------------
            // COMMENT
            //--------------------

        case TOKENR_COMMENT:
            Parsing_String();	// 그냥 m_TokenString에 읽어버리는 역할 뿐.
            //AfxMessageBox( m_TokenString, NULL, NULL);		/// 임시로 코멘트를 출력해본다
            break;

            //--------------------
            // SCENE
            //--------------------

        case TOKENR_SCENE:
            //
            break;
        case TOKENR_MESH_NUMTVERTEX:
        {
            int TVSize = Parsing_NumberInt();
            m_OneMesh->m_mesh_tvertex.resize(TVSize);
        }
        break;
        case TOKENR_MESH_MAPPINGCHANNEL:
        {
            Parsing_NumberInt();
        }
        break;
        case TOKENR_MESH_TVERT:
        {
            if (parents.size() > 2 && *(parents.end() - 2) == TOKENR_MESH_MAPPINGCHANNEL)
                break;

            int TVertexIndex = Parsing_NumberInt();
            m_OneMesh->m_mesh_tvertex[TVertexIndex] = new COneTVertex();
            m_OneMesh->m_mesh_tvertex[TVertexIndex]->m_u = Parsing_NumberFloat();
            m_OneMesh->m_mesh_tvertex[TVertexIndex]->m_v = Parsing_NumberFloat();
            m_OneMesh->m_mesh_tvertex[TVertexIndex]->m_w = Parsing_NumberFloat();
        }
        break;
        case TOKENR_MESH_TFACE:
        {
            int TVertexIndex = Parsing_NumberInt();
            if (m_OneMesh->m_meshface[TVertexIndex] == nullptr)
                m_OneMesh->m_meshface[TVertexIndex] = new ASE::Face;
            m_OneMesh->m_meshface[TVertexIndex]->m_TFace[0] = Parsing_NumberInt();
            m_OneMesh->m_meshface[TVertexIndex]->m_TFace[1] = Parsing_NumberInt();
            m_OneMesh->m_meshface[TVertexIndex]->m_TFace[2] = Parsing_NumberInt();
        }
        break;
        case TOKENR_SCENE_FILENAME:
            m_scenedata.m_filename = Parsing_String();		// 일관성 있는 함수의 사용을 위해 String과 Int도 만들어줬다.
            break;
        case TOKENR_SCENE_FIRSTFRAME:
            m_scenedata.m_firstframe = Parsing_NumberLong();
            break;
        case TOKENR_SCENE_LASTFRAME:
            m_scenedata.m_lastframe = Parsing_NumberLong();
            break;
        case TOKENR_SCENE_FRAMESPEED:
            m_scenedata.m_framespeed = Parsing_NumberLong();
            break;
        case TOKENR_SCENE_TICKSPERFRAME:
            m_scenedata.m_ticksperframe = Parsing_NumberLong();
            break;
        case TOKENR_SCENE_MESHFRAMESTEP:
            m_scenedata.m_meshframestep = Parsing_NumberLong();
            break;
        case TOKENR_SCENE_KEYFRAMESTEP:
            m_scenedata.m_keyframestep = Parsing_NumberLong();
            break;
        case TOKENR_SCENE_BACKGROUND_STATIC:
            m_scenedata.m_scene_background_static.x = Parsing_NumberFloat();
            m_scenedata.m_scene_background_static.y = Parsing_NumberFloat();
            m_scenedata.m_scene_background_static.z = Parsing_NumberFloat();
            break;
        case TOKENR_SCENE_AMBIENT_STATIC:
            m_scenedata.m_scene_ambient_static.x = Parsing_NumberFloat();
            m_scenedata.m_scene_ambient_static.y = Parsing_NumberFloat();
            m_scenedata.m_scene_ambient_static.z = Parsing_NumberFloat();
            break;

        case TOKENR_SCENE_ENVMAP:
        {
            // 그냥 안의 내용을 읽어버린다 }가 나올때까지
            while (nowtoken = m_lexer->GetToken(m_TokenString), nowtoken != TOKEND_BLOCK_END) {
            }
        }
        break;

        //--------------------
        // MATERIAL_LIST
        //--------------------
        case TOKENR_MATERIAL_LIST:
        {
            break;
        }
        case TOKENR_MATERIAL_COUNT:
        {
            int count = Parsing_NumberInt();
            m_list_materialdata.clear();
            m_list_materialdata.resize(count);
            for (int i = 0; i < count; i++)
                m_list_materialdata[i] = make_unique<ASEMaterial>();

            break;
        }
        case TOKENR_MATERIAL:
        {
            currentMaterialIndex = Parsing_NumberInt();
            break;
        }
        case TOKENR_MATERIAL_AMBIENT:
        {
            m_list_materialdata[currentMaterialIndex]->m_material_ambient = Parsing_NumberVector3();
            break;
        }
        case TOKENR_MATERIAL_DIFFUSE:
        {
            m_list_materialdata[currentMaterialIndex]->m_material_diffuse = Parsing_NumberVector3();
            break;
        }
        case TOKENR_MATERIAL_SPECULAR:
        {
            m_list_materialdata[currentMaterialIndex]->m_material_specular = Parsing_NumberVector3();
            break;
        }
        case TOKENR_MAP_DIFFUSE:
        {
            m_list_materialdata[currentMaterialIndex]->m_map_diffuse = make_unique<MaterialMap>();
            break;
        }
        case TOKENR_BITMAP:
        {
            string* m_bitmap = nullptr;
            switch (*parents.rbegin())
            {
            case TOKENR_MAP_DIFFUSE:
                m_bitmap = &m_list_materialdata[currentMaterialIndex]->m_map_diffuse->m_bitmap;
                break;
            }
            assert(m_bitmap != nullptr, "parsing error : bitmap type is not specified");
            *m_bitmap = Parsing_String();
            regex pattern("(\\w*\.dds)");
            smatch matches;
            if (regex_search(*m_bitmap, matches, pattern))
                *m_bitmap = matches[0];
            break;
        }


        //--------------------
        // GEOMOBJECT
        //--------------------

        case TOKENR_GROUP:
            //	한 개의 그룹 시작. 이 다음에 이름이 스트링으로 나오기는 하는데.
            break;

        case TOKENR_HELPEROBJECT:
            // 일단 생성하고
            // 오브젝트의 타입 정해줌. 이것에 따라 서로 다른 파싱 모드 발동.
            break;

        case TOKENR_GEOMOBJECT:
            /// 이 토큰을 만났다는건 새로운 메시가 생겼다는 것이다. 지역 변수로 mesh를 하나 선언, 그 포인터를 리스트에 넣고, m_onemesh에 그 포인터를 복사, 그대로 쓰면 될까?
            break;

        case TOKENR_NODE_NAME:
        {
            string nodename = Parsing_String();
            if (*(parents.end() - 1) == TOKENR_GEOMOBJECT ||
                *(parents.end() - 1) == TOKENR_HELPEROBJECT)
            {
                if (boneIdxByNodeName.find(nodename) != boneIdxByNodeName.end())
                    nodeIdxByBoneIdx[boneIdxByNodeName[nodename]] = m_MeshList.size();
                Create_onemesh_to_list();
                m_OneMesh->m_nodename = nodename;
            }
            break;
        }

        case TOKENR_NODE_PARENT:
            m_OneMesh->m_nodeparent = Parsing_String();
            // 현 노드의 부모 노드의 정보.
            // 일단 입력을 하고, 나중에 정리하자.
            break;

            /// NODE_TM

        case TOKENR_NODE_TM:
            //m_parsingmode	=	eGeomobject;
            // (NODE_TM으로 진입 후 NODE_NAME이 한번 더 나온다.)
            // (Animation과도 구별을 해야 하기 때문에 이렇게 모드를 적어준다)

            /// 게다가,
            // 카메라는 NodeTM이 두번 나온다. 두번째라면 넣지 않는다.
            // 아예 이 재귀에서 리턴을 시키고 싶지만. 중간에 읽는것을 무시해야 하기 때문에...
            //if (m_onemesh->m_camera_isloadTarget) {
            //	'}'이 나올때까지 나오는건 무조건 무시! 뭐 이런거 안되나..
            // 재귀구조의 문제점이다....

            break;

        case TOKENR_INHERIT_POS:
            m_OneMesh->m_inherit_pos = Parsing_NumberVector3();
            break;
        case TOKENR_INHERIT_ROT:
            m_OneMesh->m_inherit_rot = Parsing_NumberVector3();
            break;
        case TOKENR_INHERIT_SCL:
            m_OneMesh->m_inherit_scl = Parsing_NumberVector3();
            break;
        case TOKENR_TM_ROW0:
            m_OneMesh->m_tm_row0 = Parsing_NumberVector3();
            break;
        case TOKENR_TM_ROW1:
            m_OneMesh->m_tm_row1 = Parsing_NumberVector3();
            break;
        case TOKENR_TM_ROW2:
            m_OneMesh->m_tm_row2 = Parsing_NumberVector3();
            break;
        case TOKENR_TM_ROW3:
            m_OneMesh->m_tm_row3 = Parsing_NumberVector3();
            break;
        case TOKENR_TM_POS:
            m_OneMesh->m_tm_pos = Parsing_NumberVector3();
            break;
        case TOKENR_TM_ROTAXIS:
            m_OneMesh->m_tm_rotaxis = Parsing_NumberVector3();
            break;
        case TOKENR_TM_ROTANGLE:
            m_OneMesh->m_tm_rotangle = Parsing_NumberFloat();
            break;
        case TOKENR_TM_SCALE:
            m_OneMesh->m_tm_scale = Parsing_NumberVector3();
            break;
        case TOKENR_TM_SCALEAXIS:
            m_OneMesh->m_tm_scaleaxis = Parsing_NumberVector3();
            break;
        case TOKENR_TM_SCALEAXISANG:
            // 현재 카메라 상태였다면 이미 노드를 읽은 것으로 표시해준다.
            break;


            /// MESH

        case TOKENR_MESH:
            break;
        case TOKENR_TIMEVALUE:
            break;
        case TOKENR_MESH_NUMBONE:
            // 이게 있다면 이것은 Skinned Mesh라고 단정을 짓는다.
            // 내용 입력
            break;
        case TOKENR_MESH_NUMSKINWEIGHT:
            break;
        case TOKENR_MESH_NUMVERTEX:
            m_OneMesh->m_mesh_numvertex = Parsing_NumberInt();
            m_OneMesh->m_meshvertex.resize(m_OneMesh->m_mesh_numvertex);
            break;
        case TOKENR_MESH_NUMFACES:
            m_OneMesh->m_mesh_numfaces = Parsing_NumberInt();
            m_OneMesh->m_meshface.resize(m_OneMesh->m_mesh_numfaces);
            //m_OneMesh->m_mesh_facenormal = new Vector3[m_OneMesh->m_mesh_numfaces];
            m_OneMesh->m_opt_index = new IndexList[m_OneMesh->m_mesh_numfaces];
            break;

            /// MESH_VERTEX_LIST

        case TOKENR_MESH_VERTEX_LIST:
            //
            // 버텍스의 값들을 집어넣어야 하는데
            // 이미 벡터로 선언이 돼 있으므로 그냥 넣으면 된다.
            break;
        case TOKENR_MESH_VERTEX:
        {
            // 데이터 입력
            auto vertex = new Vertex();
            int index = Parsing_NumberInt();
            vertex->m_pos.x = Parsing_NumberFloat();
            vertex->m_pos.y = Parsing_NumberFloat();
            vertex->m_pos.z = Parsing_NumberFloat();
            m_OneMesh->m_meshvertex[index] = vertex;
        }
        break;

        /// Bone

        case TOKENR_SKIN_INITTM:
            break;
        case TOKENR_BONE_LIST:
            break;
        case TOKENR_BONE:
        {
            currentBoneIndex = Parsing_NumberInt();
            /// 모드 체인지 해 주고, Bone을 소유하고 있다는 것은 이것은 스키닝 오브젝트라는 것이다.
            // 본 하나를 만들어서 임시 포인터 보관, 벡터에 넣고
            // Bone의 넘버를 읽어 주자
        }
        break;
        //이 다음에 본의 이름을 넣어야 한다. 하지만 {를 한 개 더 열었으므로 임시 포인터 변수로서 보관해야겠지.
        case TOKENR_BONE_NAME:
        {
            boneIdxByNodeName[Parsing_String()] = currentBoneIndex;
        }
        case TOKENR_BONE_PROPERTY:
            // 이 다음 ABSOLUTE가 나오기는 하는데, 쓸 일이 없다.
            break;
            // 다음에는 TM_ROW0~3이 나오는데 역시 무시됨..

        case TOKENR_MESH_WVERTEXS:
            break;

        case TOKENR_MESH_WEIGHT:
        {
            // 버텍스 하나의 정보를 만들어서 리스트에 넣음
            currentVertexIndex = Parsing_NumberInt();
        }
        break;
        case TOKENR_BONE_BLENGING_WEIGHT:
        {
            int& blendIndex = m_OneMesh->m_meshvertex[currentVertexIndex]->m_blend_number;
            m_OneMesh->m_meshvertex[currentVertexIndex]->m_blend_indices[blendIndex] = Parsing_NumberInt();
            m_OneMesh->m_meshvertex[currentVertexIndex]->m_blend_weights[blendIndex] = Parsing_NumberFloat();
            blendIndex++;
        }
        break;


        /// MESH_FACE_LIST
        case TOKENR_MESH_FACE_LIST:
            //
            break;
        case TOKENR_MESH_FACE:
        {
            // Face의 번호인데...

            // A:를 읽고
            // B:
            // C:
            int index = Parsing_NumberInt();
            if (m_OneMesh->m_meshface[index] == nullptr)
                m_OneMesh->m_meshface[index] = new Face();
            Parsing_String(); // A:
            m_OneMesh->m_meshface[index]->m_vertexindex[0] = Parsing_NumberInt();
            Parsing_String(); // B:
            m_OneMesh->m_meshface[index]->m_vertexindex[1] = Parsing_NumberInt();
            Parsing_String(); // C:
            m_OneMesh->m_meshface[index]->m_vertexindex[2] = Parsing_NumberInt();

            m_OneMesh->m_opt_index[index].index[0] = m_OneMesh->m_meshface[index]->m_vertexindex[0];
            m_OneMesh->m_opt_index[index].index[1] = m_OneMesh->m_meshface[index]->m_vertexindex[1];
            m_OneMesh->m_opt_index[index].index[2] = m_OneMesh->m_meshface[index]->m_vertexindex[2];

            /// (뒤에 정보가 더 있지만 default에 의해 스킵될 것이다.)
            /// ......

            // 벡터에 넣어준다.
        }
        break;
        case TOKENR_MESH_FACENORMAL:
        {
            int index = Parsing_NumberInt();
            m_OneMesh->m_meshface[index]->m_normal = Parsing_NumberVector3();

            Parsing_String();
            Parsing_NumberInt();

            m_OneMesh->m_meshface[index]->m_normalvertex[0].x = Parsing_NumberFloat();
            m_OneMesh->m_meshface[index]->m_normalvertex[0].y = Parsing_NumberFloat();
            m_OneMesh->m_meshface[index]->m_normalvertex[0].z = Parsing_NumberFloat();

            Parsing_String();
            Parsing_NumberInt();

            m_OneMesh->m_meshface[index]->m_normalvertex[1].x = Parsing_NumberFloat();
            m_OneMesh->m_meshface[index]->m_normalvertex[1].y = Parsing_NumberFloat();
            m_OneMesh->m_meshface[index]->m_normalvertex[1].z = Parsing_NumberFloat();

            Parsing_String();
            Parsing_NumberInt();

            m_OneMesh->m_meshface[index]->m_normalvertex[2].x = Parsing_NumberFloat();
            m_OneMesh->m_meshface[index]->m_normalvertex[2].y = Parsing_NumberFloat();
            m_OneMesh->m_meshface[index]->m_normalvertex[2].z = Parsing_NumberFloat();
        }
        break;
        // vertex normal vectors are parsed above in case TOKENR_MESH_FACENORMAL:
        case TOKENR_MESH_VERTEXNORMAL:
        {
        }
        break;
        case TOKENR_MESH_TVERTLIST:
            //
            break;
        case TOKENR_MESH_NUMTVFACES:
            break;

        case TOKENR_TM_ANIMATION:
            Create_animationdata_to_list();
            m_animation->m_nodename = m_OneMesh->m_nodename;
            break;
        case TOKENR_CONTROL_POS_SAMPLE:
        {
            m_animation->m_position.push_back({ Parsing_NumberInt(),Parsing_NumberVector3() });
        }
        break;
        case TOKENR_CONTROL_ROT_TRACK:
        {
            m_lastQuaternionAcc = XMQuaternionIdentity();
        }
        break;
        case TOKENR_CONTROL_ROT_SAMPLE:
        {
            CAnimation_rot rot;
            rot.m_time = Parsing_NumberInt();
            rot.m_rot = Parsing_NumberVector3();
            rot.m_angle = Parsing_NumberFloat();
            //auto temp = rot.m_rot.z;
            //rot.m_rot.z = rot.m_rot.y;
            //rot.m_rot.y = temp;
            rot.m_angle *= -1;
            //rot.m_rotQT_accumulation = XMQuaternionMultiply(m_lastQuaternionAcc, Quaternion(XMQuaternionRotationAxis(rot.m_rot, rot.m_angle)));
            //rot.m_rotQT_accumulation = XMQuaternionMultiply(Quaternion(XMQuaternionRotationAxis(rot.m_rot, rot.m_angle)), m_lastQuaternionAcc);
            rot.m_rotQT_accumulation = XMQuaternionMultiply(m_lastQuaternionAcc, Quaternion(XMQuaternionRotationAxis(rot.m_rot, rot.m_angle)));
            //if (rot.m_time == 0)
            //{
                //rot.m_rotQT_accumulation = XMQuaternionMultiply(m_lastQuaternionAcc, Quaternion(XMQuaternionRotationAxis(rot.m_rot, rot.m_angle)));
            //}

            m_lastQuaternionAcc = rot.m_rotQT_accumulation;


            // come back for this for accumulation
            m_animation->m_rotation.push_back(rot);
        }
        break;

        case TOKEND_END:
            // 아마도 이건 파일의 끝이 나타났을때인것 같은데. while을 탈출해야 하는데?

            //AfxMessageBox("파일의 끝을 본 것 같습니다!", MB_OK, NULL);
            TRACE("TRACE: 파싱중: 파일의 끝을 봤습니다!\n");
            return;

            break;

            /// 위의 아무것도 해당하지 않을때
        default:
            // 아무것도 하지 않는다.
            break;

        }	// switch()


        lastToken = nowtoken;
        ///-----------------------------------------------
        /// 안전 코드.
        i++;
        if (i > 1000000)
        {
            // 루프를 1000000번이상이나 돌 이유가 없다. (데이터가 100000개가 아닌이상)
            // 만약 1000000이상 돌았다면 확실히 뭔가 문제가 있는 것이므로
            TRACE("루프를 백만번 돌았습니다!");
            return;
        }
        /// 안전 코드.
        ///-----------------------------------------------

    }		// while()

    // 여기까지 왔다면 while()을 벗어났다는 것이고 그 말은
    // 괄호를 닫았다는 것이므로
    // 리턴하자 (재귀함수)

    return;
}



///----------------------------------------------------------------------
/// parsing을 위한 단위별 함수들
///----------------------------------------------------------------------

// long을 읽어서 리턴해준다.
int ASEParser::Parsing_NumberLong()
{
    LONG			token;
    LONG			tempNumber;

    token = m_lexer->GetToken(m_TokenString);	//ASSERT(token == TOKEND_NUMBER);
    tempNumber = strtoul(m_TokenString, NULL, 10);

    return			tempNumber;
}

// float
float ASEParser::Parsing_NumberFloat()
{
    LONG			token;
    float			tempNumber;

    token = m_lexer->GetToken(m_TokenString);	//ASSERT(token == TOKEND_NUMBER);
    tempNumber = (float)atof(m_TokenString);

    return			tempNumber;
}

// String
LPSTR ASEParser::Parsing_String()
{
    /// ※m_TokenString ( char[255] ) 이기 때문에 CString에 넣으면 에러 날거라 생각했는데, 생각보다 CString은 잘 만들어진 것 같다. 알아서 받아들이는데?
    m_lexer->GetToken(m_TokenString);

    return m_TokenString;
}


// int
int ASEParser::Parsing_NumberInt() {

    LONG			token;
    int				tempNumber;

    token = m_lexer->GetToken(m_TokenString);	//ASSERT(token == TOKEND_NUMBER);
    tempNumber = (int)atoi(m_TokenString);

    return			tempNumber;
}


// 3개의 Float를 벡터 하나로
Vector3 ASEParser::Parsing_NumberVector3()
{
    LONG				token;
    Vector3			tempVector3;

    token = m_lexer->GetToken(m_TokenString);
    tempVector3.x = (float)atof(m_TokenString);
    token = m_lexer->GetToken(m_TokenString);
    tempVector3.y = (float)atof(m_TokenString);
    token = m_lexer->GetToken(m_TokenString);
    tempVector3.z = (float)atof(m_TokenString);

    return			tempVector3;		// 스태틱 변수의 레퍼런스보다는 값 전달을 하자.
}

///--------------------------------------------------
/// 내부에서 뭔가를 생성, 리스트에 넣는다
///--------------------------------------------------
// 메시를 하나 동적으로 생성하고, 그 포인터를 리스트에 넣는다.
void ASEParser::Create_onemesh_to_list()
{
    m_OneMesh = new Mesh();
    m_OneMesh->m_scenedata = m_scenedata;		// 클래스간 값 복사
    m_MeshList.push_back(unique_ptr<Mesh>(m_OneMesh));
}

// 메트리얼 하나를 동적으로 생성하고, 그 포인터를 리스트에 넣는다.
void ASEParser::Create_materialdata_to_list()
{
    ASEMaterial* temp = new ASEMaterial;
    m_materialdata = temp;
    m_list_materialdata.push_back(unique_ptr<ASEMaterial>(m_materialdata));
}

// 애니메이션데이터 하나를 동적으로 생성하고, 그 포인터를 리스트에 넣는다.
void ASEParser::Create_animationdata_to_list()
{
    m_animation = make_shared<Animation>();
    m_list_animation.push_back(m_animation);
}

// 정점 하나를..
void ASEParser::Create_onevertex_to_list()
{
    Vertex* temp = new Vertex;
    m_OneMesh->m_meshvertex.push_back(temp);
}
void ASEParser::ClearLoadedData()
{
    m_materialdata = nullptr;
    m_list_materialdata.clear();
    m_OneMesh = nullptr;
    m_MeshList.clear();
    nodeIdxByBoneIdx.clear();

    delete m_nowshapeline;
    m_nowshapeline = nullptr;

    delete m_nowshapevertex;
    m_nowshapevertex = nullptr;

    m_animation = nullptr;
    boneIdxByNodeName.clear();
    m_list_animation.clear();
}
