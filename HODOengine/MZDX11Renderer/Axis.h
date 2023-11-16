#pragma once
#include "DX11Define.h"
#include "IDebugObject.h"

/// <summary>
/// 3D공간의 축을 나타내는 오브젝트
/// 
/// 2023.07.06 MJKIM
/// </summary>

class MZCamera;

class Axis : public IDebugObject
{
public:
	Axis();
	Axis(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, ID3D11RasterizerState* pRS);
	~Axis();

//public:
//	virtual void SetDevice(ID3D11Device* pDevice) override { m_d3dDevice = pDevice; }
//	virtual void SetDeviceContext(ID3D11DeviceContext* pDeviceContext) override { m_d3dImmediateContext = pDeviceContext; }
//	virtual void SetRasterizerState(ID3D11RasterizerState* pRS) override { m_pRS = pRS; }

public:
	virtual void Update(MZCamera* pCamera, float deltaTime) override;
	virtual void RenderToTexture() override;
	virtual void SetWorldTM(const XMMATRIX& tm) override { 
		if (!m_isActive)
			return;
		m_world = tm; 
	};
	virtual void SetActive(bool isActive) override { m_isActive = isActive; };
	//virtual bool Pick(MZCamera* pCamera, float x, float y) override { return false; };

private:
	void BuildGeometryBuffers();

private:
	ComPtr<ID3D11Device> m_d3dDevice;
	ComPtr<ID3D11DeviceContext> m_d3dImmediateContext;

	ComPtr<ID3D11Buffer> m_VB;
	ComPtr<ID3D11Buffer> m_IB;

	ComPtr<ID3D11RasterizerState> m_pRS;

	XMMATRIX m_world;
	XMMATRIX m_view;
	XMMATRIX m_proj;

	bool m_isActive = true;
};

