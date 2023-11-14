#pragma once
#include "DX11Define.h"
#include "CAnimation.h"

namespace MZMeshData
{
	struct Mesh;
}
class MZCamera;


class Node
{
public:
	Node(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, ID3D11RasterizerState* pRS);
	DirectX::XMFLOAT4X4 GetWorldTM() { return m_world; }
	DirectX::XMMATRIX GetLocalTM() { return m_localTM; }
	DirectX::XMMATRIX GetInitLocalTM() { return m_firstLocalTM; }
	DirectX::XMMATRIX GetRealWorldTM();
	bool IsHaveAnimation() { return m_animation != nullptr; }
	bool IsAnimFinished() { 
		if (m_animation)
			return (m_animation->IsFinished()) ? true : false; 
	}
	
	void SetDiffuseMapSRV(ID3D11ShaderResourceView* srv)
	{
		m_diffuseMapSRV = srv;
	};
	void SetNormalMapSRV(ID3D11ShaderResourceView* srv)
	{
		m_normalMapSRV = srv;
	};
	int GetPosAnimStartFrame() {
		if (!m_animation->posInfo.empty())
			return m_animation->posFrame[0];
		return 100000;
	}
	int GetRotAnimStartFrame() { 
		if (!m_animation->rotInfo.empty())
			return m_animation->rotFrame[0]; 
		return 100000;
	}
	int GetPosAnimEndFrame() { 
		if (!m_animation->posInfo.empty())
			return m_animation->posFrame[m_animation->posFrame.size() - 1]; 
		return 100000;
	}
	int GetRotAnimEndFrame() { 
		if (!m_animation->rotInfo.empty())
			return m_animation->rotFrame[m_animation->rotFrame.size() - 1]; 
		return 100000;
	}

	void LoadGeometry(MZMeshData::Mesh* meshData);
	void Update(MZCamera* pCamera, float elapsedTime);
	void Render();

public:
	void InitLocalTM();
	void DecomposeLocalTM();
	void InitQuaternion();
	void InitTransformMatrix(XMMATRIX t, XMMATRIX r, XMMATRIX s);

private:
	void UpdatePosLocal();
	void UpdateRotLocal();
	void UpdateLocalTM();

	DirectX::XMMATRIX CalculateWorldTM();

public:
	/// Hierarchy
	Node* m_parent;
	std::string m_nodeName;
	std::string m_parentName;
	bool m_isParentExist = false;

	XMFLOAT4X4 worldTMInv; // 초기 worldTM의 역행렬
	XMFLOAT4X4 initWorldTM; // 초기 worldTM

private:
	ComPtr<ID3D11Device> m_d3dDevice;						
	ComPtr<ID3D11DeviceContext> m_d3dImmediateContext;
	ComPtr<ID3D11RasterizerState> m_pRS;
	ID3D11Buffer* m_VB;
	ID3D11Buffer* m_IB;

	/// Animation
	CAnimation* m_animation;

	// Transform Matrix
	XMFLOAT4X4 m_world;	// Define transformations from local spaces to world space.
	XMFLOAT4X4 m_view;
	XMFLOAT4X4 m_proj;

	//XMFLOAT4X4 m_invWorldTM;

	XMMATRIX m_wTranslateTM;
	XMMATRIX m_wRotateTM;
	XMMATRIX m_wScaleTM;

	// 애니메이션 정보가 들어간 TM
	XMMATRIX m_localTM;
	XMMATRIX m_scaleTM;	
	XMMATRIX m_rotateTM;
	XMMATRIX m_translateTM;

	// 처음 초기화된 Local Matrix
	XMMATRIX m_firstScale;
	XMMATRIX m_firstRotate;
	XMMATRIX m_firstTranslate;
	XMMATRIX m_firstLocalTM;

	// 처음 초기화된 LocalTM에서 나온 RotateVector
	XMFLOAT4 m_decomposeRotate;
	XMVECTOR m_rotateAxis;

	UINT IndexCount;

	// Texture
	ID3D11ShaderResourceView* m_diffuseMapSRV;
	ID3D11ShaderResourceView* m_normalMapSRV;
	XMFLOAT4X4 mTexTransform;
	
};

