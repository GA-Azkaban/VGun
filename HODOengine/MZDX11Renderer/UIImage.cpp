#include "UIImage.h"
#include "MZCamera.h"
#include "Vertex.h"
#include "Effects.h"
#include "ResourceManager.h"
#include "DebugCube.h"

UIImage::UIImage(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, ID3D11RasterizerState* pRS, XMFLOAT3 position, XMFLOAT2 size, XMFLOAT4 color)
	: m_d3dDevice(pDevice), m_d3dImmediateContext(pDeviceContext), m_pRenderstate(pRS),
	m_VB(nullptr), m_IB(nullptr), m_isSetScreenSpace(true),
	m_world(XMMatrixIdentity()), m_view(), m_proj(),
	m_position(position), m_size(size), m_color(color),
	m_diffuseMapSRV(0)
{
	BuildGeometryBuffers();
	XMStoreFloat4x4(&m_texTransform, XMMatrixIdentity());

}

UIImage::~UIImage()
{

}

void UIImage::SetWorldTM(const XMMATRIX& tm)
{
	m_world = tm;

	m_position.x = m_world.r[3].m128_f32[0];
	m_position.y = m_world.r[3].m128_f32[1];
	m_position.z = m_world.r[3].m128_f32[2];

	m_scale.x = m_world.r[0].m128_f32[0];
	m_scale.y = m_world.r[1].m128_f32[1];
	m_scale.z = m_world.r[2].m128_f32[2];

	m_meshBox.Center.x = m_boundingVolumeInitPos.x + m_world.r[3].m128_f32[0];
	m_meshBox.Center.y = m_boundingVolumeInitPos.y - m_world.r[3].m128_f32[1];
	m_meshBox.Center.z = m_boundingVolumeInitPos.z + m_world.r[3].m128_f32[2];

	m_meshBox.Extents.x = m_boundingVolumeInitExtents.x * m_world.r[0].m128_f32[0];
	m_meshBox.Extents.y = m_boundingVolumeInitExtents.y * m_world.r[1].m128_f32[1];
	m_meshBox.Extents.z = m_boundingVolumeInitExtents.z * m_world.r[2].m128_f32[2];

	m_size = XMFLOAT2(2.0f * m_meshBox.Extents.x, 2.0f * m_meshBox.Extents.y);
	
	//m_debugCube->SetWorldTM(m_world);
}

void UIImage::SetImage(LPCSTR fileName)
{
	m_diffuseMapSRV = ResourceManager::Instance.Get().GetTextureSRV(fileName);
}

bool UIImage::Pick(MZCamera* pCamera, float x, float y)
{
	if (!m_isActive)
		return false;

	if (!m_isSetScreenSpace) 
		return false;

	if (!m_isPickingOn)
		return false;
	// x = normalizedX, y = normalizedY ==> 왼쪽상단 (0.0, 0.0) / 오른쪽하단 (1.0, 1.0)
	// screen space X = 2 * x - 1.0f; ==> -1.0 ~ 1.0
	// screen space Y = 2 * y - 1.0f; ==> -1.0 ~ 1.0
	XMVECTOR rayOrigin = XMVectorSet(x * 2 - 1.0f, y * 2 - 1.0f, 0.0f, 1.0f);
	XMVECTOR rayDir = XMVectorSet(x * 2 - 1.0f, y * 2 - 1.0f, 1.0f, 0.0f);

	float tmin = 0.0f;
	if (XNA::IntersectRayAxisAlignedBox(rayOrigin, rayDir, &m_meshBox, &tmin))
	{
		m_depth = tmin;
		return true;
	}
	return false;
}

void UIImage::Update(MZCamera* pCamera, float deltaTime)
{
	if (!m_isActive)
		return;

	if (m_isSetScreenSpace)
	{
		m_view = XMMatrixIdentity();
		m_proj = XMMatrixIdentity();
	}
	else
	{
		/*XMVECTOR normalizedToEye = XMVector2Normalize(XMVECTOR{ pCamera->GetPosition().x - m_position.x, pCamera->GetPosition().z - m_position.z });
		XMVECTOR normalizedFrontVec = XMVector2Normalize(XMVECTOR{ pCamera->GetPosition().x - pCamera->GetPosition().x, pCamera->GetPosition().z - m_position.z });
		XMVECTOR dot = XMVector2Dot(normalizedToEye, normalizedFrontVec);
		float theta = acos(dot.m128_f32[0]);

		m_world.r[0].m128_f32[0] = cos(theta);
		m_world.r[0].m128_f32[2] = sin(theta);
		m_world.r[2].m128_f32[0] = -sin(theta);
		m_world.r[2].m128_f32[2] = cos(theta); */

		m_world.r[0].m128_f32[0] = m_scale.x * pCamera->GetWorld().r[0].m128_f32[0];
		m_world.r[0].m128_f32[2] = pCamera->GetWorld().r[0].m128_f32[2];
		m_world.r[2].m128_f32[0] = pCamera->GetWorld().r[2].m128_f32[0];
		m_world.r[2].m128_f32[2] = m_scale.z * pCamera->GetWorld().r[2].m128_f32[2];

		m_world.r[1].m128_f32[1] = m_scale.y * pCamera->GetWorld().r[1].m128_f32[1];
		m_world.r[1].m128_f32[2] = pCamera->GetWorld().r[1].m128_f32[2];
		m_world.r[2].m128_f32[1] = pCamera->GetWorld().r[2].m128_f32[1];
		m_world.r[2].m128_f32[2] = pCamera->GetWorld().r[2].m128_f32[2];

		m_world.r[0].m128_f32[1] = pCamera->GetWorld().r[0].m128_f32[1];
		m_world.r[1].m128_f32[0] = pCamera->GetWorld().r[1].m128_f32[0];

		m_view = pCamera->View();
		m_proj = pCamera->Proj();
	}
	//if (m_debugCube != nullptr)
	//	m_debugCube->Update(XMMatrixIdentity(), XMMatrixIdentity());
}

void UIImage::Render()
{
	if (!m_isActive)
		return;
	// 입력 배치 객체 셋팅
	m_d3dImmediateContext->IASetInputLayout(InputLayouts::TexInputLayout);
	m_d3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 인덱스버퍼와 버텍스버퍼 세팅
	UINT stride = sizeof(VertexStruct::PosTex);
	UINT offset = 0;
	m_d3dImmediateContext->IASetVertexBuffers(0, 1, m_VB.GetAddressOf(), &stride, &offset);
	m_d3dImmediateContext->IASetIndexBuffer(m_IB.Get(), DXGI_FORMAT_R32_UINT, 0);

	// 렌더 스테이트
	m_d3dImmediateContext->RSSetState(m_pRenderstate.Get());

	// world view proj TM 셋팅
	XMMATRIX worldViewProj = m_world * m_view * m_proj;

	// 월드의 역행렬
	//XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(m_world);

	ID3DX11EffectTechnique* mTech = Effects::UIObjectFX->DrawUITech;
	Effects::UIObjectFX->SetDiffuseMap(m_diffuseMapSRV.Get());
	Effects::UIObjectFX->SetWorld(worldViewProj);

	// 테크닉
	D3DX11_TECHNIQUE_DESC techDesc;
	mTech->GetDesc(&techDesc);

	// 렌더패스
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		mTech->GetPassByIndex(p)->Apply(0, m_d3dImmediateContext.Get());
		m_d3dImmediateContext->DrawIndexed(6, 0, 0);
	}
	//if (m_debugCube != nullptr)
	//	m_debugCube->RenderToTexture();
}

void UIImage::BuildGeometryBuffers()
{
	vertices.resize(4);
	// 정점 버퍼를 만든다.
	vertices[0] = { XMFLOAT3(-1.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) };
	vertices[1] = { XMFLOAT3(-1.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) };
	vertices[2] = { XMFLOAT3(1.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) };
	vertices[3] = { XMFLOAT3(1.0f, -1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) };

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(VertexStruct::PosTex) * 4;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices[0];
	HR(m_d3dDevice->CreateBuffer(&vbd, &vinitData, &m_VB));

	// 인덱스 버퍼를 만든다.
	UINT indices[] = {
		0,1,2,
		0,2,3,
	};

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * 6;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indices;
	HR(m_d3dDevice->CreateBuffer(&ibd, &iinitData, &m_IB));

	m_size = XMFLOAT2(vertices[3].Pos.x - vertices[0].Pos.x, vertices[2].Pos.y - vertices[0].Pos.y);

	SetBoundingVolume();
}

void UIImage::SetBoundingVolume()
{
	// Bounding box
	XMFLOAT3 vMinf3(+MathHelper::Infinity, +MathHelper::Infinity, +MathHelper::Infinity);
	XMFLOAT3 vMaxf3(-MathHelper::Infinity, -MathHelper::Infinity, -MathHelper::Infinity);

	XMVECTOR vMax = XMLoadFloat3(&vMaxf3);
	XMVECTOR vMin = XMLoadFloat3(&vMinf3);

	for (UINT i = 0; i < vertices.size(); ++i)
	{
		XMVECTOR P = XMLoadFloat3(&vertices[i].Pos);
		vMin = XMVectorMin(vMin, P);
		vMax = XMVectorMax(vMax, P);
	}

	XMStoreFloat3(&m_meshBox.Center, 0.5f * (vMin + vMax));
	XMStoreFloat3(&m_meshBox.Extents, 0.5f * (vMax - vMin));
	m_boundingVolumeInitPos = m_meshBox.Center;
	m_boundingVolumeInitExtents = m_meshBox.Extents;

	//XMFLOAT3 cubeSize{ 2.0f * m_meshBox.Extents.x, 2.0f * m_meshBox.Extents.y, 2.0f * m_meshBox.Extents.z };
	//m_debugCube = std::unique_ptr<DebugCube>(new DebugCube(m_d3dDevice.Get(), m_d3dImmediateContext.Get(), RasterizerState::WireframeRS.Get(), m_meshBox.Center, cubeSize));
	//m_debugCube->SetWorldTM(m_world);
	//m_debugCube->SetColor(XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f));
}

