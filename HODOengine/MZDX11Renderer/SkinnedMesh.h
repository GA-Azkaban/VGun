#pragma once

//#include "DX11Define.h"
//#include "d3dx11Effect.h"	// effect, tech
//#include "IMeshObject.h"
//#include <map>
//#include "d3dx11Effect.h"	// effect, tech
//#include "Vertex.h"
//#include "Effects.h"
//#include "YunuAnimInstance.h"
//#include "ASEParser.h"
//#include "Animation.h"
//#include "ParsingDataClass.h"
//
//
//class MZCamera;
//class SkinnedMesh : public IMeshObject
//{
//public:
//    SkinnedMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, ID3D11RasterizerState* pRS);
//    ~SkinnedMesh();
//
//public:
//    void LoadGeometry(ASE::Mesh* meshData, const XMMATRIX& basisTransform = XMMatrixIdentity());
//    void LoadGeometry(vector<ASE::Mesh*> meshDatas, const map<int, int>& nodeIdxByBoneIdx);
//    void LoadAnimation(const list<shared_ptr<Animation>>& nodeAnimList, const ASE::Scenedata& sceneData);
//    void LoadMesh(LPCSTR fileName);
//    void SetDiffuseMap(LPCSTR resourceID);
//    void SetNormalMap(LPCSTR resourceID);
//    void LoadDiffuseMap(const wstring& filename);
//    void LoadDiffuseMap(const string& filename);
//    void SetRootTM(const XMMATRIX& tm);
//    XMMATRIX GetRootTM();
//    XMMATRIX GetBoneWorldTM(const string& boneName)const;
//    void SetBoneLocalTM(const string& boneName, const XMMATRIX& tm);
//    XMMATRIX GetBoneLocalTM(const string& boneName)const;
//    ID3DX11EffectTechnique* mTech;
//    void SetAnimElapsed(float elapsed) { 
//		if (!m_isActive)
//			return;
//        m_animElapsed = elapsed;
//    }
//
//    void SetDevice(ID3D11Device* pDevice) {};
//    void SetDeviceContext(ID3D11DeviceContext* pDeviceContext) {};
//    void SetRasterizerState(ID3D11RasterizerState* pRS) {};
//
//    virtual void Update(MZCamera* pCamera, float deltaTime) override;
//    virtual void RenderDeferred() override;
//    virtual void SetWorldTM(const XMMATRIX& tm) override { 
//		if (!m_isActive)
//			return;
//        mWorld = tm; 
//    };
//    virtual void SetActive(bool isActive) override;
//    virtual bool Pick(MZCamera* pCamera, float x, float y) override { return false; };
//    virtual void SetPickingMode(bool isPickingModeOn) {};
//    virtual float GetDepth() { return m_depth; };
//
//    void SetTranslateMatrix(float offsetX, float offsetY, float offsetZ) {};
//    void SetRotateMatrix(float radian) {};
//    void SetScaleMatrix(float scale) {};
//
//public:
//    wstring materialPath;
//    YunuAnimInstance& GetAnim() { return animInstance; };
//private:
//    string meshID;
//    string diffuseMapID;
//    string normalMapID;
//    YunuAnimInstance animInstance;
//    XMMATRIX GetBoneWorldTM(int boneIndex)const;
//    void UpdateAnimation(float deltaTime);
//    XMMATRIX m_rootTM = XMMatrixIdentity();
//    ID3D11Device* md3dDevice;						// D3D11 디바이스
//    ID3D11DeviceContext* md3dDeviceContext;		// 디바이스 컨텍스트
//
//    ID3D11Buffer* mVB;
//    ID3D11Buffer* mIB;
//
//    // Transform Matrix
//    XMMATRIX mWorld;	// Define transformations from local spaces to world space.
//    XMFLOAT4X4 mView;
//    XMFLOAT4X4 mProj;
//
//    // 렌더스테이트도 바깥에서 받아온다.
//    ID3D11RasterizerState* m_pRenderstate;
//
//    UINT IndexCount;
//    Material m_Material;
//
//    /// 빛은 외부에서 받아오도록 하자
//    DirectionalLight mDirLights[3];
//    PointLight mPointLights[3];
//    SpotLight mSpotLights[3];
//    XMFLOAT3 mEyePosW;
//    //UINT mLightCount = 3;
//
//
//    ///----------------------------------------------------------------------------------------------------
//    /// 텍스쳐에서 추가된 부분
//    ID3D11ShaderResourceView* mDiffuseMapSRV;
//    ID3D11ShaderResourceView* mNormalMapSRV;
//
//    XMFLOAT4X4 mTexTransform;
//    XMFLOAT4X4 mBoxWorld;
//
//    // Mth bone is the parent bone of Nth node when parentIndexes[n] == M is true.
//    vector<int> m_nodeParents;
//    vector<XMMATRIX> m_nodeLocalTM;
//    vector<XMVECTOR> m_nodeLocalInitialPos;
//    vector<XMVECTOR> m_nodeLocalInitialRot;
//    vector<XMVECTOR> m_nodeLocalInitialScl;
//
//    // whole length of animation
//    float m_animLength = 0;
//    float m_animStartOffset = 0;
//
//    // duration of 1 frame of animation.
//    float m_animTickLength = 0.0001f;
//
//    // current elapsed time of animation.
//    float m_animElapsed = 0;
//    vector<shared_ptr<Animation>> m_Anim;
//    vector<int> m_animPosIndices;
//    vector<int> m_animRotIndices;
//    vector<int> m_animScaleIndices;
//
//    mutable vector<XMMATRIX> m_nodeWorldTM;
//    mutable vector<bool> m_nodeWorldTMCached;
//
//    vector<XMMATRIX> m_nodeInitialWorldTM;
//    vector<XMMATRIX> m_nodeInitialWorldTM_Inverse;
//    vector<int> m_nodeVertexSize;
//    vector<int> m_nodeVertexOffset;
//    map<std::string, int> m_nodeIndexByName;
//    int m_nodeSize;
//
//    int mBoxVertexOffset;
//    UINT mBoxIndexOffset;
//    UINT mBoxIndexCount;
//    UINT vertexCount = 0;
//
//    float m_depth = 1.0f;
//
//    bool m_isActive = true;
//};
