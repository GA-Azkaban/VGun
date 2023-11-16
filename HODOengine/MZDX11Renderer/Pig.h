#pragma once

#include "DX11Define.h"
#include "d3dx11Effect.h"	// effect, tech
#include "Vertex.h"
#include "IMeshObject.h"

class MZCamera;
class Node;

/// <summary>
/// Skinned Mesh Test용 돼지를 출력
///
/// 2023.07.07 MJKIM
/// </summary>
class Pig : public IMeshObject
{
public:
	Pig(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, ID3D11RasterizerState* pRS);
	~Pig();

public:
	virtual void Update(MZCamera* pCamera, float deltaTime) override;
	virtual void RenderToTexture() override;
	virtual void SetWorldTM(const XMMATRIX& tm) override { };
	virtual void SetActive(bool isActive) override { m_isActive = isActive; };
	virtual bool Pick(MZCamera* pCamera, float x, float y) override { return false; };
	virtual void SetPickingMode(bool isPickingModeOn) { };

	void SetTranslateMatrix(float offsetX, float offsetY, float offsetZ);
	void SetRotateMatrix(float radian);
	void SetScaleMatrix(float scale);

public:
	void SetDevice(ID3D11Device* pDevice) { m_d3dDevice = pDevice; }
	void SetDeviceContext(ID3D11DeviceContext* pDeviceContext) { m_d3dImmediateContext = pDeviceContext; }
	void SetRasterizerState(ID3D11RasterizerState* pRS) { m_pRS = pRS; }

private:
	void BuildGeometryBuffers();
	void UpdateBoneTransformsTM();

private:
	std::vector<Node*> nodeList;
	std::map<int, Node*> boneList;
	std::vector<XMFLOAT4X4> boneTransforms;
	int boneCount = 0;

private:
	ComPtr<ID3D11Device> m_d3dDevice;						// D3D11 디바이스
	ComPtr<ID3D11DeviceContext> m_d3dImmediateContext;		// 디바이스 컨텍스트
	ComPtr<ID3D11RasterizerState> m_pRS;

	Material m_Material;

	XMMATRIX m_translateTM;
	XMMATRIX m_rotateTM;
	XMMATRIX m_scaleTM;

	float m_angle;
	float m_timeElapsed;
	int m_startFrame;
	int m_endFrame;

	///----------------------------------------------------------------------------------------------------
	/// 텍스쳐에서 추가된 부분
	ID3D11ShaderResourceView* m_diffuseMapSRV;
	ID3D11ShaderResourceView* m_normalMapSRV;
	
	bool m_isActive = true;
};
