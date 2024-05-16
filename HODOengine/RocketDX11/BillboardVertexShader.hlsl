#include "ConstantBuffer.hlsli"

struct VertexShaderInput
{
    float3 position : POSITION;
    float2 uv       : TEXCOORD;
};

struct VertexToPixel
{
    float4 position : SV_POSITION; // vertex position
    float2 uv : TEXCOORD;
};

VertexToPixel main(VertexShaderInput input)
{
    VertexToPixel output;

    float4 pos = mul(float4(input.position, 1.0f), world);
    output.position = mul(pos, viewProjection);
    output.uv = input.uv;

    return output;
}