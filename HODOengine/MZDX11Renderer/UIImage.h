#pragma once
#include "DX11Define.h"
#include "d3dx11Effect.h"
#include "Vertex.h"
#include "IUIObject.h"
#include "xnacollision.h"

/// <summary>
/// 네모 박스 모양에 Image를 출력하는 UI 오브젝트
/// </summary>

class MZCamera;
class DebugCube;

class UIImage : public IUIObject
{
public:
	UIImage(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, ID3D11RasterizerState* pRS, XMFLOAT3 position = XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2 size = XMFLOAT2(1.0f, 1.0f), XMFLOAT4 color = XMFLOAT4((const float*)&Colors::White));
	~UIImage();

public:
	virtual void Update(MZCamera* pCamera, float deltaTime);
	virtual void Render();
	virtual bool Pick(MZCamera* pCamera, float x, float y);
	virtual void SetPickingMode(bool isPickingModeOn) { m_isPickingOn = isPickingModeOn; };

	virtual void SetWorldTM(const XMMATRIX& tm);
	virtual void SetActive(bool isActive) { m_isActive = isActive; };

	virtual void SetScreenSpace() { m_isSetScreenSpace = true; }
	virtual void SetWorldSpace() { m_isSetScreenSpace = false; }
	void SetImage(LPCSTR fileName);

	virtual float GetDepth() { return m_depth; }

private:
	void BuildGeometryBuffers();
	void SetBoundingVolume();

private:
	ComPtr<ID3D11Device> m_d3dDevice;						/// D3D11 디바이스
	ComPtr<ID3D11DeviceContext> m_d3dImmediateContext;		/// 디바이스 컨텍스트

	ComPtr<ID3D11Buffer> m_VB;	// 버텍스버퍼
	ComPtr<ID3D11Buffer> m_IB;	// 인덱스버퍼

	// 렌더스테이트. 렌더링을 어떻게 할 것인가에 대한 것.
	ComPtr<ID3D11RasterizerState> m_pRenderstate;

	std::vector<VertexStruct::PosTex> vertices;

	// 변환 관련
	DirectX::XMMATRIX m_world;	// 월드 변환 행렬 (로컬->월드)
	DirectX::XMMATRIX m_view;	// 시야 변환 행렬 (카메라 뷰)
	DirectX::XMMATRIX m_proj;	// 투영 변환 행렬 (원근/직교)

	ComPtr<ID3D11ShaderResourceView> m_diffuseMapSRV;
	XMFLOAT4X4 m_texTransform;

	XMFLOAT3 m_position;
	XMFLOAT2 m_size;
	XMFLOAT4 m_color;

	/// Picking을 위한 bouding box
	XNA::AxisAlignedBox m_meshBox;
	std::string m_meshName;
	DirectX::XMFLOAT3 m_boundingVolumeInitPos;
	DirectX::XMFLOAT3 m_boundingVolumeInitExtents;
	bool m_isPickingOn = true;

	//std::unique_ptr<DebugCube> m_debugCube;

	bool m_isSetScreenSpace;
	float m_depth = 1.0f;

	DirectX::XMFLOAT3 m_scale{ 1.0f, 1.0f, 1.0f };

	bool m_isActive = true;
};

