#pragma once

#include "DX11Define.h"
#include "d3dx11Effect.h"	// effect, tech
#include "IDebugObject.h"

/// <summary>
/// 디버그용 큐브 모양 객체
/// 
/// 2023.08.10 MJKIM
/// </summary>

class MZCamera;

class DebugCube : public IDebugObject
{
public:
	DebugCube(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, ID3D11RasterizerState* pRS, XMFLOAT3 position = XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3 size = XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT4 color = XMFLOAT4((const float*)&Colors::Red));
	~DebugCube();

	void SetPosition(XMFLOAT3 pos) { m_position = pos; }
	void SetColor(XMFLOAT4 color);

public:
	virtual void Update(MZCamera* pCamera, float deltaTime) override;
	void Update(const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& proj);
	virtual void RenderToTexture() override;
	virtual void SetWorldTM(const XMMATRIX& tm) override 
	{ 
		m_world = tm; 
		m_world.r[3].m128_f32[0] += m_position.x;
		m_world.r[3].m128_f32[1] += m_position.y;
		m_world.r[3].m128_f32[2] += m_position.z;
	};
	virtual void SetActive(bool isActive) override { 
		s_IsActive = isActive;
		m_isActive = isActive; 
	};
	//virtual bool Pick(MZCamera* pCamera, float x, float y) override { return false; }

public:
	void SetFillModeSolid() { m_pRenderstate = RasterizerState::SolidRS.Get(); };
	void SetFillModeWireframe() { m_pRenderstate = RasterizerState::WireframeRS.Get(); };

private:
	void BuildGeometryBuffers();

private:
	ComPtr<ID3D11Device> m_d3dDevice;						/// D3D11 디바이스
	ComPtr<ID3D11DeviceContext> m_d3dImmediateContext;		/// 디바이스 컨텍스트

	ComPtr<ID3D11Buffer> m_VB;	// 버텍스버퍼
	ComPtr<ID3D11Buffer> m_IB;	// 인덱스버퍼

	// 렌더스테이트. 렌더링을 어떻게 할 것인가에 대한 것.
	ComPtr<ID3D11RasterizerState> m_pRenderstate;
	
	// 변환 관련
	DirectX::XMMATRIX m_world;	// 월드 변환 행렬 (로컬->월드)
	DirectX::XMFLOAT4X4 m_view;	// 시야 변환 행렬 (카메라 뷰)
	DirectX::XMFLOAT4X4 m_proj;	// 투영 변환 행렬 (원근/직교)

	XMFLOAT3 m_position;
	XMFLOAT3 m_size;
	XMFLOAT4 m_color;

	static bool s_IsActive;
	bool m_isActive = true;
};

