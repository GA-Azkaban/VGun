#pragma once
#include "DX11Define.h"
#include "d3dx11Effect.h"
#include "Vertex.h"
#include "IUIObject.h"
#include "xnacollision.h"
#include <stdarg.h>
#include <tchar.h>

class DXTKFont;

class UIText : public IUIObject
{
public:
	UIText(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, ID3D11RasterizerState* pRS, 
		XMFLOAT3 position = XMFLOAT3(0.0f, 0.0f, 0.0f), 
		XMFLOAT2 size = XMFLOAT2(1.0f, 1.0f), 
		XMFLOAT4 backgroundColor = XMFLOAT4((const float*)&Colors::Black), 
		XMFLOAT4 textColor = XMFLOAT4((const float*)&Colors::White));
	~UIText();

public:
	virtual void Update(MZCamera* pCamera, float deltaTime);
	virtual void Render();
	virtual bool Pick(MZCamera* pCamera, float x, float y);
	virtual void SetPickingMode(bool isPickingModeOn) { m_isPickingOn = isPickingModeOn; };

	virtual void SetWorldTM(const XMMATRIX& tm);
	virtual void SetActive(bool isActive) { m_isActive = isActive; };

	virtual void SetScreenSpace() { m_isSetScreenSpace = true; }
	//virtual void SetScreenSpace() {  }
	virtual void SetWorldSpace() { m_isSetScreenSpace = false; }
	//virtual void SetWorldSpace() {  }

	void SetBackgroundColor(XMFLOAT4 color) { m_backgroundColor = color; }
	void SetTextColor(XMFLOAT4 color) { m_textColor = color; }
	void SetText(TCHAR* text, ...);
	const std::string GetText();

	virtual float GetDepth() { return m_depth; }

private:
	void BuildGeometryBuffers();
	void CreateResources();
	void SetBoundingVolume();
	void SetRenderTargets();
	void ClearRenderTargets(const float color[4]);
	void DrawText();

	void RenderToTexture();

private:
	ComPtr<ID3D11Device> m_d3dDevice;						/// D3D11 디바이스
	ComPtr<ID3D11DeviceContext> m_d3dImmediateContext;		/// 디바이스 컨텍스트

	ComPtr<ID3D11Buffer> m_VB;	// 버텍스버퍼
	ComPtr<ID3D11Buffer> m_VB_Quad;	// 버텍스버퍼
	ComPtr<ID3D11Buffer> m_IB;	// 인덱스버퍼
	ComPtr<ID3D11Buffer> m_IB_Quad;	// 인덱스버퍼

	// 렌더스테이트. 렌더링을 어떻게 할 것인가에 대한 것.
	ComPtr<ID3D11RasterizerState> m_pRenderstate;

	ComPtr<ID3D11Texture2D> m_pRenderTargetTexture;
	ComPtr<ID3D11RenderTargetView> m_pRenderTagetView;
	ComPtr<ID3D11ShaderResourceView> m_pShaderResourceView;
	ComPtr<ID3D11DepthStencilView> m_pDepthStencilView;

	ComPtr<ID3D11ShaderResourceView> m_diffuseMapSRV;

	std::vector<VertexStruct::PosColor> vertices;
	std::vector<VertexStruct::PosTex> vertices_quad;

	// 변환 관련
	DirectX::XMMATRIX m_world;	// 월드 변환 행렬 (로컬->월드)
	DirectX::XMMATRIX m_view;	// 시야 변환 행렬 (카메라 뷰)
	DirectX::XMMATRIX m_proj;	// 투영 변환 행렬 (원근/직교)

	std::unique_ptr<DXTKFont> m_pFont;
	TCHAR m_text[1024] = L"";
	float m_fontSize = 18.0f;

	XMFLOAT3 m_position;
	XMFLOAT2 m_size;
	XMFLOAT4 m_backgroundColor;
	XMFLOAT4 m_textColor;

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

