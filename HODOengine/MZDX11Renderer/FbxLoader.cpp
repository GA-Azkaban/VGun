#include "FbxLoader.h"

FbxLoader* CreateFBXLoader()
{
	return new FbxLoader();
}

void ReleaseFBXLoader(FbxLoader* loader)
{
	delete loader;
	loader = nullptr;
}

FbxLoader::FbxLoader()
	: m_pManager(nullptr), m_pScene(nullptr)
{
	Initialize();
}

FbxLoader::~FbxLoader()
{
	Finalize();
}

void FbxLoader::Load(const char* fileName)
{
	FbxImporter* pImporter = FbxImporter::Create(m_pManager, "");

	pImporter->Initialize(fileName, -1, m_pManager->GetIOSettings());
	pImporter->Import(m_pScene);

	pImporter->Destroy();

	SetGeometryBuffers();
}

std::vector<MyVertex> FbxLoader::GetVertexList()
{
	return m_VertextList;
}

std::vector<unsigned int> FbxLoader::GetIndexList()
{
	return m_IndexList;
}

void FbxLoader::Initialize()
{
	// 매니저 만들기
	m_pManager = FbxManager::Create();

	// IOS 세팅하기
	FbxIOSettings* pIOsettings = FbxIOSettings::Create(m_pManager, IOSROOT);
	pIOsettings->SetBoolProp(IMP_FBX_MATERIAL, true);
	m_pManager->SetIOSettings(pIOsettings);

	m_pScene = FbxScene::Create(m_pManager, "");
}

void FbxLoader::SetGeometryBuffers()
{
	FbxNode* pFbxRootNode = m_pScene->GetRootNode();
	auto rootName = pFbxRootNode->GetName();
	if (pFbxRootNode)
	{
		// 모든 자식 메쉬들을 순회 (pMesh->GetPolygonCount()), 현재는 첫번째 자식 메쉬만 순회
		//unsigned int nodeCount = pFbxRootNode->GetChildCount();
		unsigned int nodeCount = 1;
		for (unsigned int i = 0; i < nodeCount; i++)
		{
			// 자식 메쉬를 가리킴
			FbxNode* pFbxChildNode = pFbxRootNode->GetChild(i);
			auto nodeName = pFbxChildNode->GetName();
			// ?
			FbxNodeAttribute::EType AttributeType = pFbxChildNode->GetNodeAttribute()->GetAttributeType();
			// pMesh = 가리키고있는 자식 메쉬
			FbxMesh* pMesh = (FbxMesh*)pFbxChildNode->GetNodeAttribute();

			// 자식 메쉬의 모든 Vertex들을 담고있는 포인터
			FbxVector4* pVertexList = pMesh->GetControlPoints();

			// 노말 정보를 담음
			FbxArray<FbxVector4> normals;
			pMesh->GetPolygonVertexNormals(normals);

			// UV 정보를 담음
			FbxStringList uvNames;
			pMesh->GetUVSetNames(uvNames);
			const char* uvSetName = uvNames.GetStringAt(0);
			FbxArray<FbxVector2> uv;
			pMesh->GetPolygonVertexUVs(uvSetName, uv);

			// pMesh->GetPolygonCount() -> 페이스 숫자만큼 순회
			int polygonCount = pMesh->GetPolygonCount();
			for (int j = 0; j < polygonCount; j++)
			{
				// pMesh->GetPolygonSize(j) -> j번 페이스가 이루어진 정점의 갯수 (삼각형 폴리곤이므로 3)
				int polygonTypeCount = pMesh->GetPolygonSize(j);
				for (int k = 0; k < polygonTypeCount; k++)
				{
					MyVertex vertex;
					m_VertextList.push_back(vertex);

					// pMesh->GetPolygonVertex(j, k) -> j번 페이스를 이루는 k번째 버텍스의 인덱스
					int nowVertexIndex = pMesh->GetPolygonVertex(j, k);

					// 각 페이스를 이루고 있는 버텍스에 대한 위치 정보 값을 내가 만든 버텍스 리스트 벡터에 담아줌
					m_VertextList[j * 3 + k].pos[0] = (float)pVertexList[nowVertexIndex].mData[0];
					m_VertextList[j * 3 + k].pos[1] = (float)pVertexList[nowVertexIndex].mData[1];
					m_VertextList[j * 3 + k].pos[2] = (float)pVertexList[nowVertexIndex].mData[2];
				
					m_VertextList[j * 3 + k].normal[0] = normals[nowVertexIndex][0];
					m_VertextList[j * 3 + k].normal[1] = normals[nowVertexIndex][1];
					m_VertextList[j * 3 + k].normal[2] = normals[nowVertexIndex][2];

					if (uv != nullptr)
					{
						m_VertextList[j * 3 + k].uv[0] = uv[j * 3 + k].mData[0];
						m_VertextList[j * 3 + k].uv[1] = uv[j * 3 + k].mData[1];
						isTexCoordExist = true;

					}
				}
			}

			// 자식 메쉬의 모든 index들을 담고있는 포인터
			//int* indexList = pMesh->GetPolygonVertices();
			for (int j = 0; j < pMesh->GetPolygonCount(); j++)
			{
				// 모든 인덱스 정보를 순서대로 내가 만든 인덱스 리스트 벡터에 넣어줌
				/*m_IndexList.push_back(indexList[3 * j]);
				m_IndexList.push_back(indexList[3 * j + 1]);
				m_IndexList.push_back(indexList[3 * j + 2]);*/
				m_IndexList.push_back(3 * j + 0);
				m_IndexList.push_back(3 * j + 1);
				m_IndexList.push_back(3 * j + 2);
			}
		}
	}
}

void FbxLoader::Finalize()
{
	std::vector<unsigned int> tempIntVec;
	m_IndexList.swap(tempIntVec);

	std::vector<MyVertex> tempVerTexVec;
	m_VertextList.swap(tempVerTexVec);

	m_pScene->Destroy();
	m_pManager->Destroy();
}


