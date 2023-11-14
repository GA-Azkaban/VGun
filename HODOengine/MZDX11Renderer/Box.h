#pragma once

#include "DX11Define.h"
#include "d3dx11Effect.h"
#include "Vertex.h"
#include "IRenderableObject.h"

/// <summary>
/// PBXLoader 테스트용 오브젝트
/// PBXLoader로부터 데이터가 잘 들어왔는지 테스트한다.
/// 2023.08.01 MJKIM
/// </summary>

class MZCamera;

class Box
{
public:
	Box(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, ID3D11RasterizerState* pRS);
	~Box();

public:
	void Initialize();
	void Update(MZCamera* pCamera);
	void Render();

private:
	void BuildGeometryBuffers();		// 기하구조로부터 버텍스/인덱스버퍼를 만든다.

private:
	ComPtr<ID3D11Device> m_d3dDevice;						// D3D11 디바이스
	ComPtr<ID3D11DeviceContext> m_d3dImmediateContext;		// 디바이스 컨텍스트

	ID3D11Buffer* m_VB;
	ID3D11Buffer* m_IB;

	// Transform Matrix
	XMFLOAT4X4 m_world;	// Define transformations from local spaces to world space.
	XMFLOAT4X4 m_view;
	XMFLOAT4X4 m_proj;

	XMFLOAT3 mEyePosW;

	// 렌더스테이트도 바깥에서 받아온다.
	ComPtr<ID3D11RasterizerState> m_pRS;

	UINT IndexCount;
	Material m_material;

	///----------------------------------------------------------------------------------------------------
	/// 텍스쳐에서 추가된 부분
	XMMATRIX mWorldPos;
	XMMATRIX translateTM;
	XMMATRIX rotateTM;
	XMMATRIX scaleTM;

	DirectionalLight m_dirLights[3];
};
