#pragma once
#include "DX11Define.h"

/// <summary>
/// DX11을 이용한 3D 게임 프로그래밍 입문 책을 
/// 참고하여 만든 카메라 클래스.
/// 2023.07.02 MJKIM
/// </summary>

class MZCamera
{
public:
    static MZCamera* GetMainCamera();
    MZCamera();
    ~MZCamera();

    // 카메라 월드 포지션 getter/setter
    XMVECTOR GetPositionXM() const;
    XMFLOAT3 GetPosition() const;
    XMMATRIX GetWorld() const;
    void SetMain();
    void SetWorldTM(const DirectX::XMMATRIX& tm);
    void SetViewMatrix(const XMFLOAT4X4& view) { m_view = view; }
    void SetViewMatrix(const XMMATRIX& matrix) { SetViewMatrix(reinterpret_cast<const XMFLOAT4X4&>(matrix)); }
    void SetPosition(float x, float y, float z);
    void SetPosition(const XMFLOAT3& p);

    // 카메라 기저 벡터 getter
    XMVECTOR GetRightXM() const;
    XMFLOAT3 GetRight() const;
    XMVECTOR GetUpXM() const;
    XMFLOAT3 GetUp() const;
    XMVECTOR GetLookXM() const;
    XMFLOAT3 GetLook() const;

    // 프러스텀 관련 getter

    // near plane과 far plane의 너비, 높이 getter
    float GetNearWindowWidth() const;
    float GetNearWindowHeight() const;
    float GetFarWindowWidth() const;
    float GetFarWindowHeight() const;

    // Get view/proj matrices
    XMMATRIX View() const;
    XMMATRIX Proj() const;
    XMMATRIX ViewProj() const;

public:
    // 프러스텀 세팅
    void SetFrustum(float fovY, float aspect, float zn, float zf);
    // 카메라 로컬 세팅
    void LookAt(FXMVECTOR pos, FXMVECTOR target, FXMVECTOR worldUp);
    void LookAt(const XMFLOAT3& pos, const XMFLOAT3& target, const XMFLOAT3& up);

    // 카메라 이동
    void Strafe(float dt);
    void Walk(float dt);
    void WorldUpDown(float dt);

    // 카메라 회전
    void Pitch(float angle);
    void RotateY(float angle);

    // 시야행렬 업데이트
    void UpdateViewMatrix();

private:
    static MZCamera* mainCamera;
    // 카메라 좌표계
    XMFLOAT3 m_position;	// 카메라 위치
    XMFLOAT3 m_right;		// 카메라의 오른쪽 방향
    XMFLOAT3 m_up;			// 카메라의 위쪽 방향
    XMFLOAT3 m_look;		// 카메라가 바라보는 방향

    // 뷰 프러스텀 관련
    float m_nearZ;
    float m_farZ;
    float m_aspect;
    float m_fovY;
    float m_nearWindowHeight;
    float m_farWindowHeight;

    // 시야 행렬, 투영 행렬
    XMFLOAT4X4 m_view;
    XMFLOAT4X4 m_proj;
};
