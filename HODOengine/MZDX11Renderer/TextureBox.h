#pragma once
#include "DX11Define.h"

/// <summary>
/// GBuffer에 들어간 정보들을 그릴 박스
/// 일단 최대 5개까지만 그려지게 만들었다.
/// 2023.07.13 MJKIM
/// </summary>

class MZCamera;

class TextureBox
{
public:
	//TextureBox();
	TextureBox(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, ID3D11RasterizerState* pRS);
	~TextureBox();

public:
	void SetTexture(ID3D11ShaderResourceView* tex);
	void Update(MZCamera* pCamera);
	void Render();

private:
	void BuildGeometryBuffers();
	

private:
	ID3D11Device* m_d3dDevice;
	ID3D11DeviceContext* m_d3dImmediateContext;

	ID3D11Buffer* m_VB;
	ID3D11Buffer* m_IB;

	//ID3DX11Effect* m_FX;
	//ID3DX11EffectTechnique* m_effectTech;
	//ID3DX11EffectMatrixVariable* m_fxWorldViewProj;

	//ID3D11InputLayout* m_inputLayout;

	XMMATRIX m_world;
	XMMATRIX m_view;
	XMMATRIX m_proj;

	ID3D11RasterizerState* m_pRS;

	ID3D11ShaderResourceView* mDiffuseMapSRV;
	XMFLOAT4X4 mTexTransform;

	static UINT currentIndex;	// 박스가 생성될 때마다 하나씩 증가
	UINT uniqueIndex;	// 박스마다 고유한 인덱스
};



