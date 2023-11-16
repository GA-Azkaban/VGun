#include "MZCamera.h"

MZCamera* MZCamera::mainCamera = nullptr;
MZCamera* MZCamera::GetMainCamera()
{
	return mainCamera;
}

MZCamera::MZCamera()
	: m_position(0.0f, 0.0f, 0.0f),
	m_right(1.0f, 0.0f, 0.0f),
	m_up(0.0f, 1.0f, 0.0f),
	m_look(0.0f, 0.0f, 1.0f)
{
	SetFrustum(0.25f * MathHelper::Pi, 1.0f, 0.0001f, 1000.0f);
}

MZCamera::~MZCamera()
{

}

void MZCamera::SetMain()
{
	mainCamera = this;
}

void MZCamera::SetWorldTM(const DirectX::XMMATRIX& tm)
{
	XMFLOAT3 pos = XMFLOAT3(tm.r[3].m128_f32[0], tm.r[3].m128_f32[1], tm.r[3].m128_f32[2]);
	SetPosition(pos);
}

DirectX::XMVECTOR MZCamera::GetPositionXM() const
{
	return XMLoadFloat3(&m_position);
}

DirectX::XMFLOAT3 MZCamera::GetPosition() const
{
	return m_position;
}

DirectX::XMMATRIX MZCamera::GetWorld() const
{
	return XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_view));
}

void MZCamera::SetPosition(float x, float y, float z)
{
	m_position = XMFLOAT3(x, y, z);
}

void MZCamera::SetPosition(const XMFLOAT3& p)
{
	m_position = p;
}

DirectX::XMVECTOR MZCamera::GetRightXM() const
{
	return XMLoadFloat3(&m_right);
}

DirectX::XMFLOAT3 MZCamera::GetRight() const
{
	return m_right;
}

DirectX::XMVECTOR MZCamera::GetUpXM() const
{
	return XMLoadFloat3(&m_up);
}

DirectX::XMFLOAT3 MZCamera::GetUp() const
{
	return m_up;
}

DirectX::XMVECTOR MZCamera::GetLookXM() const
{
	return XMLoadFloat3(&m_look);
}

DirectX::XMFLOAT3 MZCamera::GetLook() const
{
	return m_look;
}

float MZCamera::GetNearWindowWidth() const
{
	return m_aspect * m_nearWindowHeight;
}

float MZCamera::GetNearWindowHeight() const
{
	return m_nearWindowHeight;
}

float MZCamera::GetFarWindowWidth() const
{
	return m_aspect * m_farWindowHeight;
}

float MZCamera::GetFarWindowHeight() const
{
	return m_farWindowHeight;
}

DirectX::XMMATRIX MZCamera::View() const
{
	return XMLoadFloat4x4(&m_view);
}

DirectX::XMMATRIX MZCamera::Proj() const
{
	return XMLoadFloat4x4(&m_proj);
}

DirectX::XMMATRIX MZCamera::ViewProj() const
{
	return XMMatrixMultiply(View(), Proj());
}

void MZCamera::SetFrustum(float fovY, float aspect, float zn, float zf)
{
	m_fovY = fovY;
	m_aspect = aspect;
	m_nearZ = zn;
	m_farZ = zf;

	m_nearWindowHeight = 2.0f * m_nearZ * tanf(0.5f * m_fovY);
	m_farWindowHeight = 2.0f * m_farZ * tanf(0.5 * m_fovY);

	XMMATRIX P = XMMatrixPerspectiveFovLH(m_fovY, m_aspect, m_nearZ, m_farZ);
	XMStoreFloat4x4(&m_proj, P);
}

void MZCamera::LookAt(FXMVECTOR pos, FXMVECTOR target, FXMVECTOR worldUp)
{
	// 시선벡터 정규화
	XMVECTOR L = XMVector3Normalize(XMVectorSubtract(target, pos));
	// 시선벡터와 월드업 외적하면 right 벡터가 나옴
	XMVECTOR R = XMVector3Normalize(XMVector3Cross(worldUp, L));
	// right 벡터와 시선벡터를 외적하면 내 up 벡터가 나옴
	XMVECTOR U = XMVector3Cross(L, R);

	XMStoreFloat3(&m_position, pos);
	XMStoreFloat3(&m_look, L);
	XMStoreFloat3(&m_right, R);
	XMStoreFloat3(&m_up, U);
}

void MZCamera::LookAt(const XMFLOAT3& pos, const XMFLOAT3& target, const XMFLOAT3& up)
{
	XMVECTOR P = XMLoadFloat3(&pos);
	XMVECTOR T = XMLoadFloat3(&target);
	XMVECTOR U = XMLoadFloat3(&up);

	LookAt(P, T, U);
}

void MZCamera::Strafe(float dt)
{
	// m_position += dt * m_right
	XMVECTOR scale = XMVectorReplicate(dt);
	XMVECTOR r = XMLoadFloat3(&m_right);
	XMVECTOR p = XMLoadFloat3(&m_position);
	XMStoreFloat3(&m_position, XMVectorMultiplyAdd(scale, r, p));
}

void MZCamera::Walk(float dt)
{
	// m_position += dt * m_look
	XMVECTOR scale = XMVectorReplicate(dt);
	XMVECTOR l = XMLoadFloat3(&m_look);
	XMVECTOR p = XMLoadFloat3(&m_position);
	XMStoreFloat3(&m_position, XMVectorMultiplyAdd(scale, l, p));
}

void MZCamera::WorldUpDown(float dt)
{
	// m_position += dt * m_up
	XMVECTOR scale = XMVectorReplicate(dt);
	XMVECTOR u = XMLoadFloat3(&m_up);
	XMVECTOR p = XMLoadFloat3(&m_position);
	XMStoreFloat3(&m_position, XMVectorMultiplyAdd(scale, u, p));
}

void MZCamera::Pitch(float angle)
{
	// 업 벡터와 시선 벡터를 오른쪽 벡터에 대해 회전한다.
	XMMATRIX R = XMMatrixRotationAxis(XMLoadFloat3(&m_right), angle);

	XMStoreFloat3(&m_up, XMVector3TransformNormal(XMLoadFloat3(&m_up), R));
	XMStoreFloat3(&m_look, XMVector3TransformNormal(XMLoadFloat3(&m_look), R));
}

void MZCamera::RotateY(float angle)
{
	// 기저벡터들을 월드 y축에 대해 회전한다.
	XMMATRIX R = XMMatrixRotationY(angle);

	XMStoreFloat3(&m_right, XMVector3TransformNormal(XMLoadFloat3(&m_right), R));
	XMStoreFloat3(&m_up, XMVector3TransformNormal(XMLoadFloat3(&m_up), R));
	XMStoreFloat3(&m_look, XMVector3TransformNormal(XMLoadFloat3(&m_look), R));
}

void MZCamera::UpdateViewMatrix()
{
	//	right.x			up.x		look.x			0
	//	right.y			up.y		look.y			0
	//	right.z			up.z		look.z			0
	//	-pos * right	-pos * up	-pos * look		1

	XMVECTOR L = XMLoadFloat3(&m_look);
	XMVECTOR R = XMLoadFloat3(&m_right);
	XMVECTOR U = XMLoadFloat3(&m_up);
	XMVECTOR P = XMLoadFloat3(&m_position);

	L = XMVector3Normalize(L);
	U = XMVector3Normalize(XMVector3Cross(L, R));
	R = XMVector3Cross(U, L);

	float x = -XMVectorGetX(XMVector3Dot(P, R));
	float y = -XMVectorGetX(XMVector3Dot(P, U));
	float z = -XMVectorGetX(XMVector3Dot(P, L));

	XMStoreFloat3(&m_look, L);
	XMStoreFloat3(&m_up, U);
	XMStoreFloat3(&m_right, R);

	m_view(0, 0) = m_right.x;
	m_view(1, 0) = m_right.y;
	m_view(2, 0) = m_right.z;
	m_view(3, 0) = x;

	m_view(0, 1) = m_up.x;
	m_view(1, 1) = m_up.y;
	m_view(2, 1) = m_up.z;
	m_view(3, 1) = y;

	m_view(0, 2) = m_look.x;
	m_view(1, 2) = m_look.y;
	m_view(2, 2) = m_look.z;
	m_view(3, 2) = z;

	m_view(0, 3) = 0.0f;
	m_view(1, 3) = 0.0f;
	m_view(2, 3) = 0.0f;
	m_view(3, 3) = 1.0f;
}
