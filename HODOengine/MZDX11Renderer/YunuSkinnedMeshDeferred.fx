
//=============================================================================
// Basic.fx by Frank Luna (C) 2011 All Rights Reserved.
//
// Basic effect that currently supports transformations, lighting, and texturing.
//=============================================================================

#include "LightHelper.fx"

cbuffer cbPerFrame
{
    DirectionalLight gDirLights[3];
    PointLight gPointLights[3];
    SpotLight gSpotLights[3];
    float4x4 bones[96];

    float3 gEyePosW;

    float  gFogStart;
    float  gFogRange;
    float4 gFogColor;
};

cbuffer cbPerObject
{
    float4x4 gWorld;
    float4x4 gWorldInvTranspose;
    float4x4 gWorldViewProj;
    float4x4 gTexTransform;
    Material gMaterial;
};

// Nonnumeric values cannot be added to a cbuffer.
Texture2D gDiffuseMap;

SamplerState samAnisotropic
{
    Filter = ANISOTROPIC;
    MaxAnisotropy = 4;

    AddressU = WRAP;
    AddressV = WRAP;
};

struct VertexIn
{
    float3 PosL         : POSITION;
    float3 NormalL      : NORMAL;
    float2 Tex          : TEXCOORD;
	float3 TangentL		: TANGENT;
    float node0Weight   : BLENDWEIGHT0;
    float node1Weight   : BLENDWEIGHT1;
    float node2Weight   : BLENDWEIGHT2;
    uint nodeIndex0     : BLENDINDICES0;
    uint nodeIndex1     : BLENDINDICES1;
    uint nodeIndex2     : BLENDINDICES2;
    uint nodeIndex3     : BLENDINDICES3;
};

struct VertexOut
{
    float4 PosH    : SV_POSITION;
    float3 PosW    : POSITION;
    float3 NormalW : NORMAL;
    float3 TangentW : TANGENT;
    float2 Tex     : TEXCOORD;
};

struct DeferredPSOut
{
    float4 Position         : SV_Target0;
    float4 Normal           : SV_Target1;
    float4 DiffuseAlbedo    : SV_Target2;
    float4 SpecularAlbedo   : SV_Target3;
    float4 Tangent   : SV_Target4;
};

VertexOut VS(VertexIn vin)
{
    VertexOut vout;

    // Transform to world space space.
    vout.PosW = float4(0.f, 0.f, 0.f, 1.0f);
    vout.NormalW = float4(0.f, 0.f, 0.f, 1.0f);
    //vout.PosW = mul(float4(vin.PosL, 1.0f), gWorld).xyz;
    //vout.NormalW = mul(vin.NormalL, (float3x3)gWorldInvTranspose);
    float node3Weight = 1.0f - vin.node0Weight - vin.node1Weight - vin.node2Weight;
    vout.PosW += vin.node0Weight * mul(float4(vin.PosL, 1.0f), bones[vin.nodeIndex0]).xyz;
    vout.PosW += vin.node1Weight * mul(float4(vin.PosL, 1.0f), bones[vin.nodeIndex1]).xyz;
    vout.PosW += vin.node2Weight * mul(float4(vin.PosL, 1.0f), bones[vin.nodeIndex2]).xyz;
    vout.PosW += node3Weight * mul(float4(vin.PosL, 1.0f), bones[vin.nodeIndex3]);

    vout.NormalW += vin.node0Weight * mul(float4(vin.NormalL, 0.0f), bones[vin.nodeIndex0]);
    vout.NormalW += vin.node1Weight * mul(float4(vin.NormalL, 0.0f), bones[vin.nodeIndex1]);
    vout.NormalW += vin.node2Weight * mul(float4(vin.NormalL, 0.0f), bones[vin.nodeIndex2]);
    vout.NormalW += node3Weight * mul(float4(vin.NormalL, 0.0f), bones[vin.nodeIndex3]);

    vout.TangentW = mul(vin.TangentL,(float3x3)gWorld);

    // Transform to homogeneous clip space.
    vout.PosH = mul(float4(vout.PosW, 1.0f), gWorldViewProj);

    // Output vertex attributes for interpolation across triangle.
    vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTransform).xy;
    ///vout.Tex = vin.Tex;


    return vout;
}

float4 PS(VertexOut pin, uniform int gLightCount, uniform bool gUseTexure) : SV_Target
{
    // Interpolating normal can unnormalize it, so normalize it.
    pin.NormalW = normalize(pin.NormalW);

// The toEye vector is used in lighting.
float3 toEye = gEyePosW - pin.PosW;

// Cache the distance to the eye from this surface point.
float distToEye = length(toEye);

// Normalize.
toEye /= distToEye;

// Default to multiplicative identity.
float4 texColor = float4(1, 1, 1, 1);

if (gUseTexure)
{
    // Sample texture.
    texColor = gDiffuseMap.Sample(samAnisotropic, pin.Tex);
}

//
// Lighting.
//

float4 litColor = texColor;
if (gLightCount > 0)
{
    // Start with a sum of zero. 
    float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

    // Sum the light contribution from each light source.  
    [unroll]
    for (int i = 0; i < gLightCount; ++i)
    {
    float4 A, D, S;
    ComputeDirectionalLight(gMaterial, gDirLights[i], pin.NormalW, toEye,
    A, D, S);

    ambient += A;
    diffuse += D;
    spec += S;

    ComputePointLight(gMaterial, gPointLights[i], pin.PosW, pin.NormalW, toEye,
    A, D, S);

    ambient += A;
    diffuse += D;
    spec += S;

    ComputeSpotLight(gMaterial, gSpotLights[i], pin.PosW, pin.NormalW, toEye,
    A, D, S);

    ambient += A;
    diffuse += D;
    spec += S;
    }

    // Modulate with late add.
    litColor = texColor * (ambient + diffuse) + spec;
    }

// Common to take alpha from diffuse material and texture.
litColor.a = gMaterial.Diffuse.a * texColor.a;

//litColor.r = 1.0f;
//litColor.g = 0.0f;
//litColor.b = 0.0f;

return litColor;
}

DeferredPSOut DeferredPS(VertexOut pin)
{
    DeferredPSOut output;
    output.Normal = float4(normalize(pin.NormalW), 1);
    output.DiffuseAlbedo = float4(gDiffuseMap.Sample(samAnisotropic, pin.Tex).rgb, 1);
    output.SpecularAlbedo = float4(gDiffuseMap.Sample(samAnisotropic, pin.Tex).rgb, 1);
    output.Position = float4(pin.PosH.xyz/pin.PosH.w, 1);
    
    //output.Normal = float4(0, 0, 0, 0);
    //output.DiffuseAlbedo = float4(0, 0, 0, 0);
    //output.SpecularAlbedo = float4(0, 0, 0, 0);
    //output.Position = float4(0, 0, 0, 0);

    return output;
}

technique11 DefaultTech
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PS(3, true)));
    }
}
technique11 DeferredGeometry
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetGeometryShader(NULL);
        //SetPixelShader(CompileShader(ps_5_0, PS(3, true)));
        SetPixelShader(CompileShader(ps_5_0, DeferredPS()));
    }
}
