#pragma once

#include "DX11Define.h"
#include "d3dx11Effect.h"	// effect, tech
#include "Vertex.h"
#include "IMeshObject.h"
#include "xnacollision.h"

class MZCamera;
class DebugCube;

/// <summary>
/// 2023.07.07 MJKIM
/// </summary>
class StaticMesh : public IMeshObject
{
public:
	//StaticMesh();
	StaticMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, ID3D11RasterizerState* pRS);
	~StaticMesh();

public:
	virtual void Update(MZCamera* pCamera, float deltaTime) override;
	virtual void RenderToTexture() override;
	virtual bool Pick(MZCamera* pCamera, float x, float y) override;
	virtual void SetPickingMode(bool isPickingModeOn) { m_isPickingOn = isPickingModeOn; };

	virtual void SetWorldTM(const XMMATRIX& tm) override;
	virtual void SetActive(bool isActive) override;

	void SetName(std::string name) { m_meshName = name; }

	XMFLOAT3 GetCenterPos() { return m_meshBox.Center; }
	XMFLOAT3 GetColliderBoxLength() { return m_meshBox.Extents; }
	float GetDepth() { return m_depth; }

public:
	void SetDevice(ID3D11Device* pDevice) { m_d3dDevice = pDevice; }
	void SetDeviceContext(ID3D11DeviceContext* pDeviceContext) { m_d3dImmediateContext = pDeviceContext; }
	void SetRasterizerState(ID3D11RasterizerState* pRS) { m_pRS = pRS; }
	void LoadStaticMesh(LPCSTR filename);		// 기하구조로부터 버텍스/인덱스버퍼를 만든다.
	void LoadDiffuseTexture(const WCHAR* filename);
	void LoadNormalTexture(const WCHAR* filename);

private:
	std::string meshID;
	ComPtr<ID3D11Device> m_d3dDevice;						// D3D11 디바이스
	ComPtr<ID3D11DeviceContext> m_d3dImmediateContext;		// 디바이스 컨텍스트

	//ID3D11Buffer* m_VB;
	//ID3D11Buffer* m_IB;

	std::vector<VertexStruct::NormalMap> m_vertices;
	std::vector<UINT> m_indices;

	// Transform Matrix
	DirectX::XMMATRIX m_world;	// Define transformations from local spaces to world space.
	DirectX::XMFLOAT4X4 m_view;
	DirectX::XMFLOAT4X4 m_proj;

	// 렌더스테이트도 바깥에서 받아온다.
	ComPtr<ID3D11RasterizerState> m_pRS;

	UINT IndexCount;
	Material m_material;

	///----------------------------------------------------------------------------------------------------
	/// 텍스쳐에서 추가된 부분
	ID3D11ShaderResourceView* mDiffuseMapSRV;
	ID3D11ShaderResourceView* mNormalMapSRV;

	DirectX::XMFLOAT4X4 mTexTransform;
	
	//DirectX::XMMATRIX translateTM;
	//DirectX::XMMATRIX rotateTM;
	//DirectX::XMMATRIX scaleTM;

	/// Picking을 위한 bouding box
	XNA::AxisAlignedBox m_meshBox;
	std::string m_meshName;
	float m_depth = 1.0f;
	bool m_isPickingOn = true;

	std::unique_ptr<DebugCube> m_debugCube;

	bool m_isActive = true;
};
