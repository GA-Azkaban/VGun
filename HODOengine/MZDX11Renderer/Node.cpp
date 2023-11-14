#include "Node.h"
#include "MeshData.h"
#include "MZCamera.h"
#include "Vertex.h"
#include "Effects.h"

Node::Node(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, ID3D11RasterizerState* pRS)
	:m_d3dDevice(pDevice), m_d3dImmediateContext(pDeviceContext), m_pRS(pRS)
{
	XMStoreFloat4x4(&mTexTransform, XMMatrixIdentity());

	m_wScaleTM = XMMatrixIdentity();
	m_wRotateTM = XMMatrixIdentity();
	m_wTranslateTM = XMMatrixIdentity();

	m_scaleTM = XMMatrixIdentity();
	m_rotateTM = XMMatrixIdentity();
	m_translateTM = XMMatrixIdentity();
}

DirectX::XMMATRIX Node::GetRealWorldTM()
{
	if (m_parent)
	{
		return m_localTM * m_parent->GetRealWorldTM();
	}
	return m_localTM;
}

DirectX::XMMATRIX Node::CalculateWorldTM()
{
	/*XMMATRIX rotateX;
	float radian = 1.5f;
	rotateX.r[0] = { 1.0f, 0.0f, 0.0f, 0.0f };
	rotateX.r[1] = { 0.0f, cosf(radian), -sinf(radian), 0.0f};
	rotateX.r[2] = { 0.0f, sinf(radian), cosf(radian), 0.0f};
	rotateX.r[3] = { 0.0f, 0.0f, 0.0f, 1.0f };*/

	//DirectX::XMMATRIX w = m_wScaleTM * rotateX * m_wRotateTM * m_wTranslateTM;
	//DirectX::XMMATRIX w = m_wScaleTM * XMMatrixIdentity();
	DirectX::XMMATRIX w = m_wScaleTM * m_wRotateTM * m_wTranslateTM;

	if (m_parent != nullptr)
	{
		w = m_parent->CalculateWorldTM();
	}

	return m_localTM * w;
}

void Node::LoadGeometry(MZMeshData::Mesh* meshData)
{
	// 노드 이름 저장
	m_nodeName = meshData->m_nodename;
	// 부모 노드 이름 저장
	if (meshData->m_isparentexist)
	{
		m_parentName = meshData->m_nodeparent;
		m_isParentExist = true;
	}
	

	// NODE TM 저장
	m_world._11 = meshData->m_tm_row0.x;
	m_world._12 = meshData->m_tm_row0.y;
	m_world._13 = meshData->m_tm_row0.z;
	m_world._14 = 0.0f;

	m_world._21 = meshData->m_tm_row2.x;
	m_world._22 = meshData->m_tm_row2.y;
	m_world._23 = meshData->m_tm_row2.z;
	m_world._24 = 0.0f;

	m_world._31 = meshData->m_tm_row1.x;
	m_world._32 = meshData->m_tm_row1.y;
	m_world._33 = meshData->m_tm_row1.z;
	m_world._34 = 0.0f;

	m_world._41 = meshData->m_tm_row3.x;
	m_world._42 = meshData->m_tm_row3.y;
	m_world._43 = meshData->m_tm_row3.z;
	m_world._44 = 1.0f;

	// TM_ANIMATION 정보 저장
	if (!meshData->pos_sample.empty())
	{
		if (m_animation == nullptr)
		{
			m_animation = new CAnimation();
		}
		for (int i = 0; i < meshData->pos_sample.size(); ++i)
		{
			int temp = meshData->pos_sample[i].first;
			Vector3 tempVec = meshData->pos_sample[i].second;
			m_animation->posFrame.push_back(temp);
			m_animation->posInfo.push_back(tempVec);
		}
	}
	if (!meshData->rot_sample.empty())
	{
		if (m_animation == nullptr)
		{
			m_animation = new CAnimation();
		}
		for (int i = 0; i < meshData->rot_sample.size(); ++i)
		{
			int temp = meshData->rot_sample[i].first;
			Vector4 tempVec = meshData->rot_sample[i].second;
			m_animation->rotFrame.push_back(temp);
			m_animation->rotInfo.push_back(tempVec);
		}
	}

	initWorldTM = m_world;
	XMMATRIX worldTM = XMLoadFloat4x4(&m_world);
	XMVECTOR det = XMMatrixDeterminant(worldTM);
	XMStoreFloat4x4(&worldTMInv, XMMatrixInverse(&det, worldTM));

	UINT vcount = 0;
	UINT faceCount = 0;
	UINT tcount = 0;

	faceCount = meshData->m_meshface.size();

	if (faceCount <= 0)
		return;
	vcount = faceCount * 3;
	std::vector<VertexStruct::PosNormalTexTanSkinned> vertices(vcount);

	//for (ASEParser::Vertex* vertex : meshData->m_meshvertex)
	for (int i = 0; i < faceCount; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			int vIndex = meshData->m_meshface[i]->m_vertexindex[j];
			int tIndex = meshData->m_meshface[i]->m_TFace[j];
			//vertices[i * 3 + j].Pos = meshData->m_meshvertex[vIndex]->m_pos * scale * worldTMInv;
			// worldTM의 역행렬을 곱해 Local 좌표계로 이동시킴
			vertices[i * 3 + j].Pos.x = meshData->m_meshvertex[vIndex]->m_pos.x * worldTMInv._11 + meshData->m_meshvertex[vIndex]->m_pos.y * worldTMInv._21 +
				meshData->m_meshvertex[vIndex]->m_pos.z * worldTMInv._31 + worldTMInv._41;
			vertices[i * 3 + j].Pos.y = meshData->m_meshvertex[vIndex]->m_pos.x * worldTMInv._12 + meshData->m_meshvertex[vIndex]->m_pos.y * worldTMInv._22 +
				meshData->m_meshvertex[vIndex]->m_pos.z * worldTMInv._32 + worldTMInv._42;
			vertices[i * 3 + j].Pos.z = meshData->m_meshvertex[vIndex]->m_pos.x * worldTMInv._13 + meshData->m_meshvertex[vIndex]->m_pos.y * worldTMInv._23 +
				meshData->m_meshvertex[vIndex]->m_pos.z * worldTMInv._33 + worldTMInv._43;

			vertices[i * 3 + j].Normal.x = meshData->m_meshface[i]->m_normalvertex[j].x;
			vertices[i * 3 + j].Normal.y = meshData->m_meshface[i]->m_normalvertex[j].y;
			vertices[i * 3 + j].Normal.z = meshData->m_meshface[i]->m_normalvertex[j].z;

			if (!meshData->m_mesh_tvertex.empty())
			{
				vertices[i * 3 + j].Tex.x = meshData->m_mesh_tvertex[tIndex]->m_u;
				vertices[i * 3 + j].Tex.y = meshData->m_mesh_tvertex[tIndex]->m_v;
			}
			if (meshData->m_numbone != 0)
			{
				vertices[i * 3 + j].Weights0 = meshData->m_meshvertex[vIndex]->m_bone_blenging_weights[0];
				vertices[i * 3 + j].Weights1 = meshData->m_meshvertex[vIndex]->m_bone_blenging_weights[1];
				vertices[i * 3 + j].Weights2 = meshData->m_meshvertex[vIndex]->m_bone_blenging_weights[2];
				vertices[i * 3 + j].BoneIndices0 = meshData->m_meshvertex[vIndex]->m_bone_blending_indices[0];
				vertices[i * 3 + j].BoneIndices1 = meshData->m_meshvertex[vIndex]->m_bone_blending_indices[1];
				vertices[i * 3 + j].BoneIndices2 = meshData->m_meshvertex[vIndex]->m_bone_blending_indices[2];
				vertices[i * 3 + j].BoneIndices3 = meshData->m_meshvertex[vIndex]->m_bone_blending_indices[3];
			}			
		}
		/// CalculateTangents
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
	for (int i = 0; i < vcount; ++i)
	{
		XMVECTOR n = XMLoadFloat3(&(vertices[i].Normal));
		XMVECTOR t = XMLoadFloat3(&(vertices[i].TangentU));
		Vector3 Tangent = XMVector3Normalize(t - XMVector3Dot(t, n) * n);
		vertices[i].TangentU = Tangent;
	}

	tcount = meshData->m_mesh_numfaces;
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
	vbd.ByteWidth = sizeof(VertexStruct::PosNormalTexTanSkinned) * vcount;
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
}

void Node::Update(MZCamera* pCamera, float elapsedTime)
{
	// 애니메이션 끝난 상태에서 timeElapsed가 다시 0으로 돌아오면 애니메이션 다시 재생
	if (m_animation)
	{
		if (m_animation->IsFinished() == true && elapsedTime <= 8000.0f)
		{
			m_animation->SetFinished(false);
		}
	}

	// 애니메이션 있는 노드 로컬 매트릭스 갱신
	if (m_animation)
	{
		if (m_animation->IsFinished() == false)
		{
			m_animation->Update(elapsedTime);
			if (!m_animation->rotInfo.empty())
				UpdateRotLocal();
			if (!m_animation->posInfo.empty())
				UpdatePosLocal();
			UpdateLocalTM();
		}
	}

	XMMATRIX world = CalculateWorldTM();
	XMStoreFloat4x4(&m_world, world);
	XMStoreFloat4x4(&m_view, pCamera->View());
	XMStoreFloat4x4(&m_proj, pCamera->Proj());
}

void Node::Render()
{
	// 입력 배치 객체 셋팅
	m_d3dImmediateContext->IASetInputLayout(InputLayouts::SkinnedMeshGBufferInputLayout);
	m_d3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 렌더 스테이트
	m_d3dImmediateContext->RSSetState(m_pRS.Get());

	// 버텍스버퍼와 인덱스버퍼 셋팅
	UINT stride = sizeof(VertexStruct::PosNormalTexTanSkinned);
	UINT offset = 0;
	m_d3dImmediateContext->IASetVertexBuffers(0, 1, &m_VB, &stride, &offset);
	m_d3dImmediateContext->IASetIndexBuffer(m_IB, DXGI_FORMAT_R32_UINT, 0);

	/// WVP TM등을 셋팅
	// Set constants
	XMMATRIX view = XMLoadFloat4x4(&m_view);
	XMMATRIX proj = XMLoadFloat4x4(&m_proj);
	XMMATRIX world = XMLoadFloat4x4(&m_world);

	XMMATRIX worldViewProj = world * view * proj;

	// 월드의 역전치행렬
	XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(world);

	// Figure out which technique to use.
	ID3DX11EffectTechnique* mTech = Effects::SkinnedMeshFX->GetGBufferTech;

	// 테크닉은...
	D3DX11_TECHNIQUE_DESC techDesc;
	mTech->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		m_d3dImmediateContext->IASetVertexBuffers(0, 1, &m_VB, &stride, &offset);
		m_d3dImmediateContext->IASetIndexBuffer(m_IB, DXGI_FORMAT_R32_UINT, 0);

		Effects::SkinnedMeshFX->SetWorld(world);
		Effects::SkinnedMeshFX->SetWorldInvTranspose(worldInvTranspose);
		Effects::SkinnedMeshFX->SetWorldViewProj(worldViewProj);
		Effects::SkinnedMeshFX->SetTexTransform(XMLoadFloat4x4(&mTexTransform));
		//Effects::SkinnedMeshFX->SetMaterial(m_material);
		Effects::SkinnedMeshFX->SetDiffuseMap(m_diffuseMapSRV);
		//Effects::SkinnedMeshFX->SetNormalMap(m_diffuseMapSRV);

		mTech->GetPassByIndex(p)->Apply(0, m_d3dImmediateContext.Get());
		m_d3dImmediateContext->DrawIndexed(IndexCount, 0, 0);
	}
}

void Node::InitLocalTM()
{
	XMMATRIX parentTMInv = XMMatrixIdentity();
	//if (parent != nullptr)
	if (m_isParentExist)
	{
		XMMATRIX parentTM = XMLoadFloat4x4(&m_parent->m_world);
		XMVECTOR det = XMMatrixDeterminant(parentTM);
		parentTMInv = XMMatrixInverse(&det, parentTM);
	}
	m_localTM = m_world * parentTMInv;

	// 처음 초기화된 LocalTM 분해
	// 이 LocalTM에서 나온 Rotate행렬을 통해 쿼터니언을 초기화해야 한다.
	DecomposeLocalTM();
}

void Node::DecomposeLocalTM()
{
	XMFLOAT3 decomposeScale;
	XMFLOAT3 decomposeTranslate;

	XMVECTOR s;
	XMVECTOR r = XMLoadFloat4(&m_decomposeRotate);
	XMVECTOR t;

	XMMatrixDecompose(&s, &r, &t, m_localTM);

	XMStoreFloat3(&decomposeScale, s);
	XMStoreFloat4(&m_decomposeRotate, r);
	XMStoreFloat3(&decomposeTranslate, t);

	m_firstScale = XMMatrixScaling(decomposeScale.x, decomposeScale.y, decomposeScale.z);
	m_firstRotate = XMMatrixRotationQuaternion(r);
	m_firstTranslate = XMMatrixTranslation(decomposeTranslate.x, decomposeTranslate.y, decomposeTranslate.z);

	m_firstLocalTM = m_firstScale * m_firstRotate * m_firstTranslate;
	//m_localTM = m_firstScale * m_firstRotate * m_firstTranslate;
}

void Node::InitQuaternion()
{
	if (m_animation)
	{
		m_animation->quaternionList.push_back(m_decomposeRotate);
		m_animation->AccumulateQuaternion();
	}
}

void Node::InitTransformMatrix(XMMATRIX t, XMMATRIX r, XMMATRIX s)
{
	m_wTranslateTM = t;
	m_wRotateTM = r;
	m_wScaleTM = s;
}

void Node::UpdatePosLocal()
{
	// 애니메이션의 현재 프레임이 애니메이션 프레임 벡터의 마지막 값보다 커지면(애니메이션 종료)
	// 마지막에 있는 포지션 값 적용하고 리턴
	if (m_animation->GetCurrentFrame() >= m_animation->posFrame[m_animation->posFrame.size() - 1])
	{
		m_translateTM = XMMatrixTranslationFromVector(m_animation->posInfo[m_animation->posFrame.size() - 1]);
		return;
	}

	// 현재 프레임을 통해 인덱스 구하기
	UINT index = 0;
	for (int i = 0; i < m_animation->posFrame.size(); ++i)
	{
		if (m_animation->GetCurrentFrame() < m_animation->posFrame[i])
		{
			break;
		}
		index++;
	}

	// animation interpolation
	Vector3 interpolatedVec = m_animation->posInfo[index];
	if (index > 0)
	{
		float t = (static_cast<float>(m_animation->GetCurrentFrame() - m_animation->posFrame[index - 1])) / (m_animation->posFrame[index] - m_animation->posFrame[index - 1]);
		interpolatedVec = XMVectorLerp(m_animation->posInfo[index - 1], m_animation->posInfo[index], t);
	}

	m_translateTM = XMMatrixTranslationFromVector(interpolatedVec);
}

void Node::UpdateRotLocal()
{
	// 애니메이션의 현재 프레임이 애니메이션 프레임 벡터의 마지막 값보다 커지면(애니메이션 종료)
	// 마지막에 있는 쿼터니언 값 적용하고 리턴
	if (m_animation->GetCurrentFrame() >= m_animation->rotFrame[m_animation->rotFrame.size() - 1])
	{
		XMVECTOR q;
		q = m_animation->quaternionList[m_animation->rotFrame.size() - 1];
		m_rotateTM = XMMatrixRotationQuaternion(q);
		return;
	}

	// 현재 프레임을 통해 인덱스 구하기
	UINT index = 0;
	for (int i = 0; i < m_animation->rotFrame.size(); ++i)
	{
		if (m_animation->GetCurrentFrame() < m_animation->rotFrame[i])
		{
			break;
		}
		index++;
	}

	// animation interpolation
	Vector4 interpolatedQuat = m_animation->quaternionList[index];
	if (index > 0)
	{
		float t = (static_cast<float>(m_animation->GetCurrentFrame() - m_animation->rotFrame[index - 1])) / (m_animation->rotFrame[index] - m_animation->rotFrame[index - 1]);
		interpolatedQuat = XMQuaternionSlerp(m_animation->quaternionList[index - 1], m_animation->quaternionList[index], t);
	}

	m_rotateTM = XMMatrixRotationQuaternion(interpolatedQuat);
}

void Node::UpdateLocalTM()
{
	XMMATRIX rotation;
	XMMATRIX translation;

	// 회전 애니메이션 없으면 최초의 회전행렬 넣어줌.
	if (m_animation->rotInfo.size() == 0)
	{
		rotation = m_firstRotate;
	}
	else
	{
		rotation = m_rotateTM;
	}

	// 이동 애니메이션 없으면 최초의 이동행렬 넣어줌.
	if (m_animation->posInfo.size() == 0)
	{
		translation = m_firstTranslate;
	}
	else
	{
		translation = m_translateTM;
	}

	// 최종 LocalTM
	m_localTM = m_firstScale * rotation * translation;
}


