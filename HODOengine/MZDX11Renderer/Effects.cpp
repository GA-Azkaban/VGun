//***************************************************************************************
// Effects.cpp by Frank Luna (C) 2011 All Rights Reserved.
//***************************************************************************************

#include "Effects.h"

#pragma region Effect
Effect::Effect(ID3D11Device* device, const std::wstring& filename)
    : mFX(0)
{
    std::ifstream fin(filename, std::ios::binary);

    fin.seekg(0, std::ios_base::end);
    int size = (int)fin.tellg();
    fin.seekg(0, std::ios_base::beg);
    std::vector<char> compiledShader(size);

    fin.read(&compiledShader[0], size);
    fin.close();

    HR(D3DX11CreateEffectFromMemory(&compiledShader[0], size,
        0, device, &mFX));
}

Effect::~Effect()
{
    SafeRelease(mFX);
}
#pragma endregion

#pragma region BasicEffect
BasicEffect::BasicEffect(ID3D11Device* device, const std::wstring& filename)
    : Effect(device, filename)
{
    Light1Tech = mFX->GetTechniqueByName("Light1");
    Light2Tech = mFX->GetTechniqueByName("Light2");
    Light3Tech = mFX->GetTechniqueByName("Light3");
    WorldViewProj = mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
    World = mFX->GetVariableByName("gWorld")->AsMatrix();
    WorldInvTranspose = mFX->GetVariableByName("gWorldInvTranspose")->AsMatrix();
    EyePosW = mFX->GetVariableByName("gEyePosW")->AsVector();
    DirLights = mFX->GetVariableByName("gDirLights");
    PointLights = mFX->GetVariableByName("gPointLights");
    Mat = mFX->GetVariableByName("gMaterial");
}

BasicEffect::~BasicEffect()
{
}
#pragma endregion

LightTexEffect::LightTexEffect(ID3D11Device* device, const std::wstring& filename)
    : Effect(device, filename)
{
    Light1Tech = mFX->GetTechniqueByName("Light1");
    Light2Tech = mFX->GetTechniqueByName("Light2");
    Light3Tech = mFX->GetTechniqueByName("Light3");
    WorldViewProj = mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
    World = mFX->GetVariableByName("gWorld")->AsMatrix();
    WorldInvTranspose = mFX->GetVariableByName("gWorldInvTranspose")->AsMatrix();
    EyePosW = mFX->GetVariableByName("gEyePosW")->AsVector();
    DirLights = mFX->GetVariableByName("gDirLights");
    PointLights = mFX->GetVariableByName("gPointLights");
    Mat = mFX->GetVariableByName("gMaterial");

    /// ÅØ½ºÃÄ
    Light0TexTech = mFX->GetTechniqueByName("Light0Tex");
    Light1TexTech = mFX->GetTechniqueByName("Light1Tex");
    Light2TexTech = mFX->GetTechniqueByName("Light2Tex");
    Light3TexTech = mFX->GetTechniqueByName("Light3Tex");

    TexTransform = mFX->GetVariableByName("gTexTransform")->AsMatrix();
    DiffuseMap = mFX->GetVariableByName("gDiffuseMap")->AsShaderResource();

}

LightTexEffect::~LightTexEffect()
{
}

ColorEffect::ColorEffect(ID3D11Device* device, const std::wstring& filename)
    : Effect(device, filename)
{
    ColorTech = mFX->GetTechniqueByName("ColorTech");
    WorldViewProj = mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
    World = mFX->GetVariableByName("gWorld")->AsMatrix();
    Color = mFX->GetVariableByName("gColor");
    WorldInvTranspose = mFX->GetVariableByName("gWorldInvTranspose")->AsMatrix();
    //EyePosW = mFX->GetVariableByName("gEyePosW")->AsVector();	
}

ColorEffect::~ColorEffect()
{
}

DebugObjectEffect::DebugObjectEffect(ID3D11Device* device, const std::wstring& filename)
    : Effect(device, filename)
{
    ColorTech = mFX->GetTechniqueByName("ColorTech");
    WorldViewProj = mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
    World = mFX->GetVariableByName("gWorld")->AsMatrix();
    Color = mFX->GetVariableByName("gColor");
    WorldInvTranspose = mFX->GetVariableByName("gWorldInvTranspose")->AsMatrix();
    //EyePosW = mFX->GetVariableByName("gEyePosW")->AsVector();	
}

DebugObjectEffect::~DebugObjectEffect()
{
}

StaticMeshEffect::StaticMeshEffect(ID3D11Device* device, const std::wstring& filename)
    :Effect(device, filename)
{
    GetGBufferTech = mFX->GetTechniqueByName("BasicTech");

    WorldViewProj = mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
    World = mFX->GetVariableByName("gWorld")->AsMatrix();
    WorldInvTranspose = mFX->GetVariableByName("gWorldInvTranspose")->AsMatrix();
    //EyePosW = mFX->GetVariableByName("EyePosW")->AsVector();
    Mat = mFX->GetVariableByName("gMaterial");

    TexTransform = mFX->GetVariableByName("gTexTransform")->AsMatrix();

    DiffuseMap = mFX->GetVariableByName("gDiffuseMap")->AsShaderResource();
    NormalMap = mFX->GetVariableByName("gNormalMap")->AsShaderResource();

    //ShaderResourceViews[0] = mFX->GetVariableByName("PositionTexture")->AsShaderResource();
    //ShaderResourceViews[1] = mFX->GetVariableByName("NormalTexture")->AsShaderResource();
    //ShaderResourceViews[2] = mFX->GetVariableByName("DiffuseAlbedoTexture")->AsShaderResource();
    //ShaderResourceViews[3] = mFX->GetVariableByName("TangentTexture")->AsShaderResource();
    //ShaderResourceViews[4] = mFX->GetVariableByName("BumpedNormalTexture")->AsShaderResource();
    //ShaderResourceViews[3] = mFX->GetVariableByName("ZBufferTexture")->AsShaderResource();
}

StaticMeshEffect::~StaticMeshEffect()
{

}

SkinnedMeshEffect::SkinnedMeshEffect(ID3D11Device* device, const std::wstring& filename)
    :Effect(device, filename)
{
    GetGBufferTech = mFX->GetTechniqueByName("BasicTech");

    World = mFX->GetVariableByName("gWorld")->AsMatrix();
    WorldInvTranspose = mFX->GetVariableByName("gWorldInvTranspose")->AsMatrix();
    WorldViewProj = mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
    //EyePosW = mFX->GetVariableByName("EyePosW")->AsVector();
    TexTransform = mFX->GetVariableByName("gTexTransform")->AsMatrix();
    Mat = mFX->GetVariableByName("gMaterial");

    BoneTransforms = mFX->GetVariableByName("gBoneTransforms")->AsMatrix();

    DiffuseMap = mFX->GetVariableByName("gDiffuseMap")->AsShaderResource();
    NormalMap = mFX->GetVariableByName("gNormalMap")->AsShaderResource();

    //ShaderResourceViews[0] = mFX->GetVariableByName("PositionTexture")->AsShaderResource();
    //ShaderResourceViews[1] = mFX->GetVariableByName("NormalTexture")->AsShaderResource();
    //ShaderResourceViews[2] = mFX->GetVariableByName("DiffuseAlbedoTexture")->AsShaderResource();
    //ShaderResourceViews[3] = mFX->GetVariableByName("TangentTexture")->AsShaderResource();
    //ShaderResourceViews[4] = mFX->GetVariableByName("BumpedNormalTexture")->AsShaderResource();
    //ShaderResourceViews[3] = mFX->GetVariableByName("ZBufferTexture")->AsShaderResource();
}

SkinnedMeshEffect::~SkinnedMeshEffect()
{

}

ComputeLightEffect::ComputeLightEffect(ID3D11Device* device, const std::wstring& filename)
    :Effect(device, filename)
{
    ComputeLightingTech = mFX->GetTechniqueByName("LightingTech");

    DirLights = mFX->GetVariableByName("DirLights");
    PointLights = mFX->GetVariableByName("PointLights");
    SpotLights = mFX->GetVariableByName("SpotLights");
    EyePosW = mFX->GetVariableByName("EyePosW")->AsVector();
    Mat = mFX->GetVariableByName("gMaterial");

    ShaderResourceViews[0] = mFX->GetVariableByName("PositionTexture")->AsShaderResource();
    ShaderResourceViews[1] = mFX->GetVariableByName("NormalTexture")->AsShaderResource();
    ShaderResourceViews[2] = mFX->GetVariableByName("DiffuseAlbedoTexture")->AsShaderResource();
    ShaderResourceViews[3] = mFX->GetVariableByName("BumpedNormalTexture")->AsShaderResource();
    ShaderResourceViews[4] = mFX->GetVariableByName("TangentTexture")->AsShaderResource();
    //ShaderResourceViews[3] = mFX->GetVariableByName("ZBufferTexture")->AsShaderResource();
}

ComputeLightEffect::~ComputeLightEffect()
{

}

PBREffect::PBREffect(ID3D11Device* device, const std::wstring& filename)
	:Effect(device, filename)
{
	ComputeLightingTech = mFX->GetTechniqueByName("LightingTech");

	DirLights = mFX->GetVariableByName("DirLights");
	PointLights = mFX->GetVariableByName("PointLights");
	SpotLights = mFX->GetVariableByName("SpotLights");
	EyePosW = mFX->GetVariableByName("EyePosW")->AsVector();
	Mat = mFX->GetVariableByName("gMaterial");

	ShaderResourceViews[0] = mFX->GetVariableByName("PositionTexture")->AsShaderResource();
	ShaderResourceViews[1] = mFX->GetVariableByName("NormalTexture")->AsShaderResource();
	ShaderResourceViews[2] = mFX->GetVariableByName("DiffuseAlbedoTexture")->AsShaderResource();
	ShaderResourceViews[3] = mFX->GetVariableByName("BumpedNormalTexture")->AsShaderResource();
	ShaderResourceViews[4] = mFX->GetVariableByName("TangentTexture")->AsShaderResource();
}

PBREffect::~PBREffect()
{

}


UIObjectEffect::UIObjectEffect(ID3D11Device* device, const std::wstring& filename)
	:Effect(device, filename)
{
	DrawUITech = mFX->GetTechniqueByName("DrawUITech");
    World = mFX->GetVariableByName("gWorld")->AsMatrix();
	DiffuseMap = mFX->GetVariableByName("gDiffuseMap")->AsShaderResource();
}

UIObjectEffect::~UIObjectEffect()
{

}

UITextEffect::UITextEffect(ID3D11Device* device, const std::wstring& filename)
	:Effect(device, filename)
{
	DrawUITextTech = mFX->GetTechniqueByName("DrawUITextTech");

}

UITextEffect::~UITextEffect()
{

}

#pragma region Effects

BasicEffect* Effects::BasicFX = 0;
LightTexEffect* Effects::BasicTexFX = 0;
ColorEffect* Effects::ColorFX = 0;
DebugObjectEffect* Effects::DebugObjectFX = 0;
StaticMeshEffect* Effects::StaticMeshFX = 0;
SkinnedMeshEffect* Effects::SkinnedMeshFX = 0;
ComputeLightEffect* Effects::ComputeLightFX = 0;
PBREffect* Effects::PBRFX = 0;
UIObjectEffect* Effects::UIObjectFX = 0;
UITextEffect* Effects::UITextFX = 0;
YunuSkinnedMeshEffect* Effects::YunuSkinnedMeshFX = 0;


void Effects::InitAll(ID3D11Device* device)
{
    BasicFX = new BasicEffect(device, L"./Basic.cso");
    BasicTexFX = new LightTexEffect(device, L"./BasicTex.cso");
    ColorFX = new ColorEffect(device, L"./color.cso");
    DebugObjectFX = new DebugObjectEffect(device, L"./DebugObject.cso");
    StaticMeshFX = new StaticMeshEffect(device, L"./StaticMeshDeferred.cso");
    SkinnedMeshFX = new SkinnedMeshEffect(device, L"./SkinnedMeshDeferred.cso");
    YunuSkinnedMeshFX = new YunuSkinnedMeshEffect(device, L"./YunuSkinnedMeshDeferred.cso");
    ComputeLightFX = new ComputeLightEffect(device, L"./ComputeLight.cso");
    PBRFX = new PBREffect(device, L"./PBR.cso");
    UIObjectFX = new UIObjectEffect(device, L"./UIObject.cso");
    UITextFX = new UITextEffect(device, L"./UIText.cso");
}

void Effects::DestroyAll()
{
    SafeDelete(BasicFX);
    SafeDelete(BasicTexFX);
    SafeDelete(ColorFX);
    SafeDelete(DebugObjectFX);
    SafeDelete(StaticMeshFX);
    SafeDelete(SkinnedMeshFX);
    SafeDelete(YunuSkinnedMeshFX);
    SafeDelete(ComputeLightFX);
    SafeDelete(PBRFX);
    SafeDelete(UIObjectFX);
    SafeDelete(UITextFX);
}
#pragma endregion
