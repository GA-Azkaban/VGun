#include "ConstantBuffer.hlsli"

struct VertexShaderInput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 uv : TEXCOORD;
};

struct VertexToPixel
{
    float4 position : SV_POSITION;
};

VertexToPixel main(VertexShaderInput input)
{
    VertexToPixel output;

    float4 pos = mul(float4(input.position, 1.0f), world);
    output.position = mul(pos, viewProjection);

    return output;
}