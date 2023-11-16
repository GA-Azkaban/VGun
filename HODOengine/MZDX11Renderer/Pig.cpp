#include "Pig.h"
#include "MZCamera.h"
#include "DDSTextureLoader.h"
#include "MeshData.h"
#include "Effects.h"
#include "Node.h"
#include "ResourceManager.h"


Pig::Pig(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, ID3D11RasterizerState* pRS)
	: m_d3dDevice(pDevice), m_d3dImmediateContext(pDeviceContext), m_pRS(pRS),
	m_angle(0.0f), m_timeElapsed(7000.0f),
	m_diffuseMapSRV(0), m_normalMapSRV(0)
{
	ID3D11Resource* texResource = nullptr;
	HR(CreateDDSTextureFromFile(m_d3dDevice.Get(),
		L"Textures/WoodCrate01.dds", &texResource, &m_diffuseMapSRV));
	SafeRelease(texResource);

	BuildGeometryBuffers();

	// LocalTM
	for (auto& node : nodeList)
	{
		node->InitLocalTM();
		node->InitQuaternion();
		node->InitTransformMatrix(m_translateTM, m_rotateTM, m_scaleTM);
	}

	SetTranslateMatrix(0.0f, 0.0f, 0.0f);
	SetRotateMatrix(0.0f);
	SetScaleMatrix(1.0f);
}

Pig::~Pig()
{
	SafeRelease(m_diffuseMapSRV);
	SafeRelease(m_normalMapSRV);

	//SafeRelease(m_IB);
	//SafeRelease(m_VB);
}

void Pig::SetTranslateMatrix(float offsetX, float offsetY, float offsetZ)
{
	m_translateTM.r[0] = { 1.0f, 0.0f, 0.0f, 0.0f };
	m_translateTM.r[1] = { 0.0f, 1.0f, 0.0f, 0.0f };
	m_translateTM.r[2] = { 0.0f, 0.0f, 1.0f, 0.0f };
	m_translateTM.r[3] = { offsetX, offsetY, offsetZ, 1.0f };
}


void Pig::SetRotateMatrix(float radian)
{
	m_rotateTM.r[0] = { cosf(radian), 0.0f, sinf(radian), 0.0f };
	m_rotateTM.r[1] = { 0.0f, 1.0f, 0.0f, 0.0f };
	m_rotateTM.r[2] = { -sinf(radian), 0.0f, cosf(radian), 0.0f };
	m_rotateTM.r[3] = { 0.0f, 0.0f, 0.0f, 1.0f };
}

void Pig::SetScaleMatrix(float scale)
{
	m_scaleTM.r[0] = { scale, 0.0f, 0.0f, 0.0f };
	m_scaleTM.r[1] = { 0.0f, scale, 0.0f, 0.0f };
	m_scaleTM.r[2] = { 0.0f, 0.0f, scale, 0.0f };
	m_scaleTM.r[3] = { 0.0f, 0.0f, 0.0f, 1.0f };
}

// ASE 파서에서 버텍스 정보, 인덱스 정보를 얻어온다.
void Pig::BuildGeometryBuffers()
{
	/*m_pASEParser = CreateASEParser();
	m_pASEParser->Init();
	m_pASEParser->Load((LPSTR)"ASEFile/babypig_walk_6x.ASE");*/
	//m_pASEParser->Load((LPSTR)"ASEFile/03IK-Joe.ASE");
	MZMeshData::MeshData* _pMeshData = ResourceManager::Instance.Get().LoadMeshData((LPSTR)"ASEFile/babypig_walk_6x.ASE");

	int index = _pMeshData->m_MeshList.size();
	for (int i = 0; i < index; ++i)
	{
		Node* node = new Node(m_d3dDevice.Get(), m_d3dImmediateContext.Get(), m_pRS.Get());
		node->SetDiffuseMapSRV(m_diffuseMapSRV);
		node->SetNormalMapSRV(m_normalMapSRV);

		/// 이것을 써서 ASE 파일 로드
		MZMeshData::Mesh* mesh = _pMeshData->GetMesh(i);
		node->LoadGeometry(mesh);
		nodeList.push_back(node);
	}
	// 부모 노드 저장
	for (int i = 0; i < nodeList.size(); ++i)
	{
		if (nodeList[i]->m_isParentExist == false)
		{
			continue;
		}
		for (int j = 0; j < nodeList.size(); ++j)
		{
			if (nodeList[i]->m_parentName == nodeList[j]->m_nodeName)
			{
				nodeList[i]->m_parent = nodeList[j];
				break;
			}
		}
	}
	// bone 저장
	for (std::map<string, int>::iterator it = _pMeshData->m_boneList.begin(); it != _pMeshData->m_boneList.end(); it++)
	{
		for (int i = 0; i < nodeList.size(); ++i)
		{
			if (nodeList[i]->m_nodeName.compare(it->first) == 0)
			{
				boneList.emplace(it->second, nodeList[i]);
				break;
			}
		}
	}
	boneCount = boneList.size();
	boneTransforms.resize(boneCount);

	// 반복재생을 위한 프레임 저장
	int _posAnimStartFrame = 0;
	int _rotAnimStartFrame = 0;
	for (UINT i = 1; i < nodeList.size(); ++i)
	{
		if (nodeList[i]->IsHaveAnimation())
		{
			_posAnimStartFrame = nodeList[i]->GetPosAnimStartFrame();
			_rotAnimStartFrame = nodeList[i]->GetRotAnimStartFrame();
			break;
		}
	}
	if (_posAnimStartFrame < _rotAnimStartFrame)
		m_startFrame = _posAnimStartFrame;
	else
		m_startFrame = _rotAnimStartFrame;

	int _posAnimEndFrame = 0;
	int _rotAnimEndFrame = 0;
	for (UINT i = nodeList.size() - 1; i >= 1; --i)
	{
		if (nodeList[i]->IsHaveAnimation())
		{
			_posAnimEndFrame = nodeList[i]->GetPosAnimEndFrame();
			_rotAnimEndFrame = nodeList[i]->GetRotAnimEndFrame();
			break;
		}
	}
	if (_posAnimEndFrame > _rotAnimEndFrame)
		m_endFrame = _posAnimEndFrame;
	else
		m_endFrame = _rotAnimEndFrame;

	// 애니메이션 바로 시작
	m_timeElapsed = m_startFrame;
}

void Pig::Update(MZCamera* pCamera, float deltaTime)
{
	for (auto& node : nodeList)
	{
		node->Update(pCamera, m_timeElapsed);
	}
	//nodeList[0]->Update(pCamera, m_timeElapsed);
	UpdateBoneTransformsTM();

	m_timeElapsed += deltaTime * 1000;
	if (m_timeElapsed > m_endFrame)
		m_timeElapsed = m_startFrame - 1; // 애니메이션 연속 재생
}

void Pig::RenderToTexture()
{
	if (boneCount > 0)
		Effects::SkinnedMeshFX->SetBoneTransforms(&boneTransforms[0], boneCount);

	//for (auto& node : nodeList)
	//{
	//	node->Render();
	//}
	nodeList[0]->Render();
}

void Pig::UpdateBoneTransformsTM()
{
	for (int i = 0; i < boneList.size(); ++i)
	{
		XMMATRIX _boneWorldTM = boneList[i]->GetRealWorldTM();
		XMMATRIX _boneInitLocalTM = boneList[i]->GetInitLocalTM();

		XMFLOAT4X4 _skinWorld = nodeList[0]->GetWorldTM();
		XMMATRIX _skinWorldTM = XMLoadFloat4x4(&_skinWorld);
		XMMATRIX _skinWorldTMInv = XMMatrixInverse(nullptr, _skinWorldTM);

		XMMATRIX _boneOffsetTM = _boneInitLocalTM * _skinWorldTMInv;
		XMMATRIX _boneOffsetTMInv = XMMatrixInverse(nullptr, _boneOffsetTM);

		XMMATRIX _finalTM = _boneOffsetTMInv * _boneWorldTM;

		XMStoreFloat4x4(&boneTransforms[i], _finalTM);
	}
}

