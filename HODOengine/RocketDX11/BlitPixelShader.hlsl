#include "Sampler.hlsli"

struct VertexToPixel
{
    float4 position     : SV_POSITION;
    float2 uv           : TEXCOORD;
};

Texture2D src   : register(t0);

float4 main(VertexToPixel input) : SV_TARGET
{
    float2 texCoord = input.uv;
    return src.Sample(PointClampSampler, texCoord);
}