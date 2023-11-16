//***************************************************************************************
// color.fx by Frank Luna (C) 2011 All Rights Reserved.
//
// Transforms and colors geometry.
//***************************************************************************************

cbuffer cbPerObject
{
    float4x4 gWorld;
    float4x4 gWorldViewProj;
    float4 gColor;
};

struct VertexIn
{
    float3 PosL  : POSITION;
    float4 Color : COLOR;
};

struct VertexOut
{
    float4 PosH  : SV_POSITION;
    float3 PosW    : POSITION;
    float4 Color : COLOR;
};

struct PixelOut
{
    float4 Color0    : SV_Target0;
    float4 Color1    : SV_Target1;
    float4 Color2    : SV_Target2;
    float4 Color3    : SV_Target3;
    float4 Color4    : SV_Target4;
};

VertexOut VS(VertexIn vin)
{
    VertexOut vout;

    vout.PosW = mul(float4(vin.PosL, 1.0f), gWorld).xyz;

    // Transform to homogeneous clip space.
    vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);

    vin.Color = gColor;
    // Just pass vertex color into the pixel shader.
    vout.Color = vin.Color;

    //vout.Color.x = 1.0f;
    //vout.Color.y = 0;
    //vout.Color.z = 0;
    //vout.Color.w = 1.0f;

    return vout;
}

PixelOut PS(VertexOut pin)
{
    PixelOut output;
    output.Color0 = pin.Color;
    output.Color1 = pin.Color;
    output.Color2 = pin.Color;
    output.Color3 = pin.Color;
    output.Color4 = pin.Color;

    return output;
}


technique11 ColorTech
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }
}
