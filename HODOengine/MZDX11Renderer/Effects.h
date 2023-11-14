//***************************************************************************************
// Effects.h by Frank Luna (C) 2011 All Rights Reserved.
//
// Defines lightweight effect wrappers to group an effect and its variables.
// Also defines a static Effects class from which we can access all of our effects.
//***************************************************************************************

#ifndef EFFECTS_H
#define EFFECTS_H

#include <concepts>
#include <wrl.h>
#include "DX11Define.h"
#include "d3dx11Effect.h"	// effect, tech

#pragma region Effect
class Effect
{
public:
	Effect(ID3D11Device* device, const std::wstring& filename);
	virtual ~Effect();

private:
	Effect(const Effect& rhs);
	Effect& operator=(const Effect& rhs);

protected:
	ID3DX11Effect* mFX;
};
#pragma endregion


/// Light Skull
class BasicEffect : public Effect
{
public:
	BasicEffect(ID3D11Device* device, const std::wstring& filename);
	~BasicEffect();

	void SetWorldViewProj(CXMMATRIX M) { WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorld(CXMMATRIX M) { World->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorldInvTranspose(CXMMATRIX M) { WorldInvTranspose->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetEyePosW(const XMFLOAT3& v) { EyePosW->SetRawValue(&v, 0, sizeof(XMFLOAT3)); }
	void SetDirLights(const DirectionalLight* lights) { DirLights->SetRawValue(lights, 0, 3 * sizeof(DirectionalLight)); }
	void SetPointLights(const PointLight* lights) { PointLights->SetRawValue(lights, 0, 3 * sizeof(PointLight)); }
	void SetMaterial(const Material& mat) { Mat->SetRawValue(&mat, 0, sizeof(Material)); }

	ID3DX11EffectTechnique* Light1Tech;
	ID3DX11EffectTechnique* Light2Tech;
	ID3DX11EffectTechnique* Light3Tech;

	ID3DX11EffectMatrixVariable* WorldViewProj;
	ID3DX11EffectMatrixVariable* World;
	ID3DX11EffectMatrixVariable* WorldInvTranspose;
	ID3DX11EffectVectorVariable* EyePosW;
	ID3DX11EffectVariable* DirLights;
	ID3DX11EffectVariable* PointLights;
	ID3DX11EffectVariable* Mat;
};

/// Crate (Texture)
class LightTexEffect : public Effect
{
public:
	LightTexEffect(ID3D11Device* device, const std::wstring& filename);
	~LightTexEffect();

	void SetWorldViewProj(CXMMATRIX M) { WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorld(CXMMATRIX M) { World->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorldInvTranspose(CXMMATRIX M) { WorldInvTranspose->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetEyePosW(const XMFLOAT3& v) { EyePosW->SetRawValue(&v, 0, sizeof(XMFLOAT3)); }
	void SetDirLights(const DirectionalLight* lights) { DirLights->SetRawValue(lights, 0, 3 * sizeof(DirectionalLight)); }
	void SetPointLights(const PointLight* lights) { PointLights->SetRawValue(lights, 0, 3 * sizeof(PointLight)); }
	void SetMaterial(const Material& mat) { Mat->SetRawValue(&mat, 0, sizeof(Material)); }

	/// 텍스쳐 추가
	void SetTexTransform(CXMMATRIX M) { TexTransform->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetDiffuseMap(ID3D11ShaderResourceView* tex) { DiffuseMap->SetResource(tex); }


	ID3DX11EffectTechnique* Light1Tech;
	ID3DX11EffectTechnique* Light2Tech;
	ID3DX11EffectTechnique* Light3Tech;

	ID3DX11EffectMatrixVariable* WorldViewProj;
	ID3DX11EffectMatrixVariable* World;
	ID3DX11EffectMatrixVariable* WorldInvTranspose;
	ID3DX11EffectVectorVariable* EyePosW;

	ID3DX11EffectVariable* DirLights;
	ID3DX11EffectVariable* PointLights;
	ID3DX11EffectVariable* Mat;


	/// 텍스쳐 추가
	ID3DX11EffectTechnique* Light0TexTech;
	ID3DX11EffectTechnique* Light1TexTech;
	ID3DX11EffectTechnique* Light2TexTech;
	ID3DX11EffectTechnique* Light3TexTech;

	ID3DX11EffectMatrixVariable* TexTransform;
	ID3DX11EffectShaderResourceVariable* DiffuseMap;
};
#pragma endregion

class ColorEffect : public Effect
{
public:
	ColorEffect(ID3D11Device* device, const std::wstring& filename);
	~ColorEffect();

	void SetWorldViewProj(CXMMATRIX M) { WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorld(CXMMATRIX M) { World->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetColor(XMFLOAT4 color) { Color->SetRawValue(&color, 0, sizeof(XMFLOAT4)); }
	//void SetEyePosW(const XMFLOAT3& v) { EyePosW->SetRawValue(&v, 0, sizeof(XMFLOAT3)); }
	void SetWorldInvTranspose(CXMMATRIX M) { WorldInvTranspose->SetMatrix(reinterpret_cast<const float*>(&M)); }
	//void SetDirLights(const DirectionalLight* lights) { DirLights->SetRawValue(lights, 0, 3 * sizeof(DirectionalLight)); }
	//void SetPointLights(const PointLight* lights) { PointLights->SetRawValue(lights, 0, 3 * sizeof(PointLight)); }
	//void SetMaterial(const Material& mat) { Mat->SetRawValue(&mat, 0, sizeof(Material)); }

	ID3DX11EffectTechnique* ColorTech;

	ID3DX11EffectMatrixVariable* WorldViewProj;
	ID3DX11EffectMatrixVariable* World;
	ID3DX11EffectVariable* Color;
	//ID3DX11EffectVectorVariable* EyePosW;
	ID3DX11EffectMatrixVariable* WorldInvTranspose;

	//ID3DX11EffectVariable* DirLights;
	//ID3DX11EffectVariable* PointLights;
	//ID3DX11EffectVariable* Mat;
};
#pragma endregion

/// DebugObject
class DebugObjectEffect : public Effect
{
public:
	DebugObjectEffect(ID3D11Device* device, const std::wstring& filename);
	~DebugObjectEffect();

	void SetWorldViewProj(CXMMATRIX M) { WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorld(CXMMATRIX M) { World->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetColor(XMFLOAT4 color) { Color->SetRawValue(&color, 0, sizeof(XMFLOAT4)); }
	//void SetEyePosW(const XMFLOAT3& v) { EyePosW->SetRawValue(&v, 0, sizeof(XMFLOAT3)); }
	void SetWorldInvTranspose(CXMMATRIX M) { WorldInvTranspose->SetMatrix(reinterpret_cast<const float*>(&M)); }
	//void SetDirLights(const DirectionalLight* lights) { DirLights->SetRawValue(lights, 0, 3 * sizeof(DirectionalLight)); }
	//void SetPointLights(const PointLight* lights) { PointLights->SetRawValue(lights, 0, 3 * sizeof(PointLight)); }
	//void SetMaterial(const Material& mat) { Mat->SetRawValue(&mat, 0, sizeof(Material)); }

	ID3DX11EffectTechnique* ColorTech;

	ID3DX11EffectMatrixVariable* WorldViewProj;
	ID3DX11EffectMatrixVariable* World;
	ID3DX11EffectVariable* Color;
	//ID3DX11EffectVectorVariable* EyePosW;
	ID3DX11EffectMatrixVariable* WorldInvTranspose;

	//ID3DX11EffectVariable* DirLights;
	//ID3DX11EffectVariable* PointLights;
	//ID3DX11EffectVariable* Mat;
};
#pragma endregion

class StaticMeshEffect : public Effect
{
public:
	StaticMeshEffect(ID3D11Device* device, const std::wstring& filename);
	~StaticMeshEffect();

	void SetWorldViewProj(CXMMATRIX M) { WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorld(CXMMATRIX M) { World->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorldInvTranspose(CXMMATRIX M) { WorldInvTranspose->SetMatrix(reinterpret_cast<const float*>(&M)); }
	//void SetEyePosW(const XMFLOAT3& v) { EyePosW->SetRawValue(&v, 0, sizeof(XMFLOAT3)); }
	void SetMaterial(const Material& mat) { Mat->SetRawValue(&mat, 0, sizeof(Material)); }

	/// 텍스쳐 추가
	void SetTexTransform(CXMMATRIX M) { TexTransform->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetDiffuseMap(ID3D11ShaderResourceView* tex) { DiffuseMap->SetResource(tex); }
	void SetNormalMap(ID3D11ShaderResourceView* normalMap) { NormalMap->SetResource(normalMap); }
	
	//void SetShaderResource(ID3D11ShaderResourceView* srv, int index) { ShaderResourceViews[index]->SetResource(srv); }


	ID3DX11EffectTechnique* GetGBufferTech;

	ID3DX11EffectMatrixVariable* WorldViewProj;
	ID3DX11EffectMatrixVariable* World;
	ID3DX11EffectMatrixVariable* WorldInvTranspose;
	//ID3DX11EffectVectorVariable* EyePosW;

	ID3DX11EffectVariable* Mat;

	ID3DX11EffectMatrixVariable* TexTransform;
	ID3DX11EffectShaderResourceVariable* DiffuseMap;
	ID3DX11EffectShaderResourceVariable* NormalMap;
	//ID3DX11EffectShaderResourceVariable* ShaderResourceViews[5];
};
#pragma endregion

class SkinnedMeshEffect : public Effect
{
public:
	SkinnedMeshEffect(ID3D11Device* device, const std::wstring& filename);
	~SkinnedMeshEffect();

	void SetWorldViewProj(CXMMATRIX M) { WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorld(CXMMATRIX M) { World->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorldInvTranspose(CXMMATRIX M) { WorldInvTranspose->SetMatrix(reinterpret_cast<const float*>(&M)); }
	//void SetEyePosW(const XMFLOAT3& v) { EyePosW->SetRawValue(&v, 0, sizeof(XMFLOAT3)); }
	void SetMaterial(const Material& mat) { Mat->SetRawValue(&mat, 0, sizeof(Material)); }
	
	void SetBoneTransforms(const XMFLOAT4X4* M, int cnt) { BoneTransforms->SetMatrixArray(reinterpret_cast<const float*>(M), 0, cnt); }

	/// 텍스쳐 추가
	void SetTexTransform(CXMMATRIX M) { TexTransform->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetDiffuseMap(ID3D11ShaderResourceView* tex) { DiffuseMap->SetResource(tex); }
	void SetNormalMap(ID3D11ShaderResourceView* normalMap) { NormalMap->SetResource(normalMap); }

	//void SetShaderResource(ID3D11ShaderResourceView* srv, int index) { ShaderResourceViews[index]->SetResource(srv); }


	ID3DX11EffectTechnique* GetGBufferTech;

	ID3DX11EffectMatrixVariable* WorldViewProj;
	ID3DX11EffectMatrixVariable* World;
	ID3DX11EffectMatrixVariable* WorldInvTranspose;
	ID3DX11EffectMatrixVariable* BoneTransforms;
	//ID3DX11EffectVectorVariable* EyePosW;

	ID3DX11EffectVariable* Mat;

	ID3DX11EffectMatrixVariable* TexTransform;
	ID3DX11EffectShaderResourceVariable* DiffuseMap;
	ID3DX11EffectShaderResourceVariable* NormalMap;
	//ID3DX11EffectShaderResourceVariable* ShaderResourceViews[5];
};
#pragma endregion

class ComputeLightEffect : public Effect
{
public:
	ComputeLightEffect(ID3D11Device* device, const std::wstring& filename);
	~ComputeLightEffect();

	void SetEyePosW(const XMFLOAT3& v) { EyePosW->SetRawValue(&v, 0, sizeof(XMFLOAT3)); }
	void SetDirLights(const DirectionalLight* lights) { DirLights->SetRawValue(lights, 0, 3 * sizeof(DirectionalLight)); }
	void SetPointLights(const PointLight* lights) { PointLights->SetRawValue(lights, 0, 3 * sizeof(PointLight)); }
	void SetSpotLights(const SpotLight* lights) { SpotLights->SetRawValue(lights, 0, 3 * sizeof(SpotLight)); }
	void SetMaterial(const Material& mat) { Mat->SetRawValue(&mat, 0, sizeof(Material)); }

	void SetShaderResource(ID3D11ShaderResourceView* srv, int index) { ShaderResourceViews[index]->SetResource(srv); }


	ID3DX11EffectTechnique* ComputeLightingTech;

	ID3DX11EffectVectorVariable* EyePosW;

	ID3DX11EffectVariable* DirLights;
	ID3DX11EffectVariable* PointLights;
	ID3DX11EffectVariable* SpotLights;
	ID3DX11EffectVariable* Mat;

	ID3DX11EffectShaderResourceVariable* ShaderResourceViews[5];
};
#pragma endregion

template<typename Container, typename ValueType>
concept ContiguousRange =
std::ranges::range<Container> &&
std::is_same_v<std::ranges::range_value_t<Container>, ValueType>&&
std::contiguous_iterator<std::ranges::iterator_t<Container>>;
class YunuSkinnedMeshEffect : public Effect
{
public:
    YunuSkinnedMeshEffect(ID3D11Device* device, const std::wstring& filename) : Effect(device, filename)
    {
        bones = mFX->GetVariableByName("bones")->AsMatrix();

        WorldViewProj = mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
        World = mFX->GetVariableByName("gWorld")->AsMatrix();
        WorldInvTranspose = mFX->GetVariableByName("gWorldInvTranspose")->AsMatrix();
        EyePosW = mFX->GetVariableByName("gEyePosW")->AsVector();
        DirLights = mFX->GetVariableByName("gDirLights");
        PointLights = mFX->GetVariableByName("gPointLights");
        SpotLights = mFX->GetVariableByName("gSpotLights");
        Mat = mFX->GetVariableByName("gMaterial");

        /// 텍스쳐
        defaultTech = mFX->GetTechniqueByName("DefaultTech");
        deferredGeometryTech = mFX->GetTechniqueByName("DeferredGeometry");

        TexTransform = mFX->GetVariableByName("gTexTransform")->AsMatrix();
        DiffuseMap = mFX->GetVariableByName("gDiffuseMap")->AsShaderResource();
    }

    void SetWorldViewProj(CXMMATRIX M) { WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
    void SetWorld(CXMMATRIX M) { World->SetMatrix(reinterpret_cast<const float*>(&M)); }
    void SetWorldInvTranspose(CXMMATRIX M) { WorldInvTranspose->SetMatrix(reinterpret_cast<const float*>(&M)); }
    void SetEyePosW(const XMFLOAT3& v) { EyePosW->SetRawValue(&v, 0, sizeof(XMFLOAT3)); }
    void SetDirLights(const DirectionalLight* lights) { DirLights->SetRawValue(lights, 0, 3 * sizeof(DirectionalLight)); }
    void SetPointLights(const PointLight* lights) { PointLights->SetRawValue(lights, 0, 3 * sizeof(PointLight)); }
    void SetSpotLights(const SpotLight* lights) { SpotLights->SetRawValue(lights, 0, 3 * sizeof(SpotLight)); }
    void SetMaterial(const Material& mat) { Mat->SetRawValue(&mat, 0, sizeof(Material)); }
    void SetTexTransform(CXMMATRIX M) { TexTransform->SetMatrix(reinterpret_cast<const float*>(&M)); }
    void SetDiffuseMap(ID3D11ShaderResourceView* tex) { DiffuseMap->SetResource(tex); }

    // 월드 변환 행렬에 관련된 정보들
    Microsoft::WRL::ComPtr<ID3DX11EffectMatrixVariable> WorldViewProj;
    Microsoft::WRL::ComPtr<ID3DX11EffectMatrixVariable> World;
    Microsoft::WRL::ComPtr<ID3DX11EffectMatrixVariable> WorldInvTranspose;
    Microsoft::WRL::ComPtr<ID3DX11EffectVectorVariable> EyePosW;

    // 매우 레거시한 라이트 정보들
    Microsoft::WRL::ComPtr<ID3DX11EffectVariable> DirLights;
    Microsoft::WRL::ComPtr<ID3DX11EffectVariable> PointLights;
    Microsoft::WRL::ComPtr<ID3DX11EffectVariable> SpotLights;
    Microsoft::WRL::ComPtr<ID3DX11EffectVariable> Mat;

    // 텍스처 좌표 위치에 행렬곱을 일괄적으로 적용하고 싶을때 쓰이는 트랜스폼
    Microsoft::WRL::ComPtr<ID3DX11EffectMatrixVariable> TexTransform;
    // 디퓨즈 텍스처
    Microsoft::WRL::ComPtr<ID3DX11EffectShaderResourceVariable> DiffuseMap;
    // 메시 본 트랜스폼
    Microsoft::WRL::ComPtr<ID3DX11EffectMatrixVariable> bones{ nullptr };

    // 테크닉
    Microsoft::WRL::ComPtr<ID3DX11EffectTechnique> defaultTech;
    Microsoft::WRL::ComPtr<ID3DX11EffectTechnique> deferredGeometryTech;

    template<typename Container>
        requires ContiguousRange<Container, XMMATRIX>&&
        requires (const Container& c) { {c.data()}->std::same_as<const XMMATRIX*>; }&&
        requires (const Container& c) { {c.size()}->std::same_as<size_t>; }
    void SetBoneFinalMatrix(const Container& matrixContainer)
    {
        assert(matrixContainer.size() <= 96);
        bones->SetRawValue(matrixContainer.data(), 0, matrixContainer.size() * sizeof(XMMATRIX));
    }
    void SetBoneFinalMatrix2(const std::vector<XMMATRIX>& matrixContainer)
    {
        assert(matrixContainer.size() <= 96);
        //bones->SetRawValue(&matrixContainer[0], 0, matrixContainer.size() * sizeof(XMMATRIX));
        //bones->setmatrixarr(&matrixContainer[0], 0 * sizeof(XMMATRIX), sizeof(XMMATRIX));
        bones->SetMatrixArray(reinterpret_cast<const float*>(&matrixContainer.data()[0]), 0, matrixContainer.size());
    }
};

class PBREffect : public Effect
{
public:
	PBREffect(ID3D11Device* device, const std::wstring& filename);
	~PBREffect();

	void SetEyePosW(const XMFLOAT3& v) { EyePosW->SetRawValue(&v, 0, sizeof(XMFLOAT3)); }
	void SetDirLights(const DirectionalLight* lights) { DirLights->SetRawValue(lights, 0, 3 * sizeof(DirectionalLight)); }
	void SetPointLights(const PointLight* lights) { PointLights->SetRawValue(lights, 0, 3 * sizeof(PointLight)); }
	void SetSpotLights(const SpotLight* lights) { SpotLights->SetRawValue(lights, 0, 3 * sizeof(SpotLight)); }
	void SetMaterial(const Material& mat) { Mat->SetRawValue(&mat, 0, sizeof(Material)); }

	void SetShaderResource(ID3D11ShaderResourceView* srv, int index) { ShaderResourceViews[index]->SetResource(srv); }

	ID3DX11EffectTechnique* ComputeLightingTech;

	ID3DX11EffectVectorVariable* EyePosW;

	ID3DX11EffectVariable* DirLights;
	ID3DX11EffectVariable* PointLights;
	ID3DX11EffectVariable* SpotLights;
	ID3DX11EffectVariable* Mat;

	ID3DX11EffectShaderResourceVariable* ShaderResourceViews[5];
};

class UIObjectEffect : public Effect
{
public:
	UIObjectEffect(ID3D11Device* device, const std::wstring& filename);
	~UIObjectEffect();

	void SetWorld(CXMMATRIX M) { World->SetMatrix(reinterpret_cast<const float*>(&M)); }
	/// 텍스쳐 추가
	void SetDiffuseMap(ID3D11ShaderResourceView* tex) { DiffuseMap->SetResource(tex); }

	ID3DX11EffectTechnique* DrawUITech;
	ID3DX11EffectMatrixVariable* World;
	ID3DX11EffectShaderResourceVariable* DiffuseMap;
};

class UITextEffect : public Effect
{
public:
	UITextEffect(ID3D11Device* device, const std::wstring& filename);
	~UITextEffect();

	ID3DX11EffectTechnique* DrawUITextTech;	
};

#pragma region Effects
class Effects
{
public:
	static void InitAll(ID3D11Device* device);
	static void DestroyAll();

	static BasicEffect* BasicFX;
	static LightTexEffect* BasicTexFX;
	static ColorEffect* ColorFX;
	static DebugObjectEffect* DebugObjectFX;
	static StaticMeshEffect* StaticMeshFX;
	static SkinnedMeshEffect* SkinnedMeshFX;
	static YunuSkinnedMeshEffect* YunuSkinnedMeshFX;
	static ComputeLightEffect* ComputeLightFX;
	static PBREffect* PBRFX;
	static UIObjectEffect* UIObjectFX;
	static UITextEffect* UITextFX;

};
#pragma endregion

#endif // EFFECTS_H