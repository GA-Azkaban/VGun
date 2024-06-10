#include "Sampler.hlsli"
#include "Common.hlsli"

struct VertexToPixel
{
    float4 position     : SV_POSITION; // vertex position
    float2 uv           : TEXCOORD;
};

Texture2D DepthTex : register(t0);
Texture2D NormalTex : register(t1);
Texture2D Noise     : register(t2);

float4 main(VertexToPixel input) : SV_TARGET
{
    float depth = DepthTex.Sample(LinearBorderSampler, input.uv).r;
    float3 viewSpacePosition = CalculateViewSpaceFromDepth(depth, input.uv);
    float3 normal = NormalTex.Sample(LinearBorderSampler, input.uv).rgb;
    normal = normalize(normal * 2.0f - 1.0f);
    float3 viewSpaceNormal = normalize(mul(normal, (float3x3)view));

    float3 randDir = Noise.Sample(PointClampSampler, input.uv * ssaoNoiseScale).rgb;
    randDir = normalize(2 * randDir - 1);

    float3 tangent = normalize(randDir - viewSpaceNormal * dot(randDir, viewSpaceNormal));
    float3 bitangent = cross(viewSpaceNormal, tangent);
    float3x3 tbn = float3x3(tangent, bitangent, viewSpaceNormal);

    float occlusion = 0.0f;
    for (int i = 0; i < 64; ++i)
    {
        float3 sampleDir = mul(ssaoSamples[i].xyz, transpose(tbn));
        float3 samplePos = viewSpacePosition + sampleDir * ssaoRadius;

        float4 offset = mul(float4(samplePos, 1.0f), projection);
        offset.xy = ((offset.xy / offset.w) * float2(1.0f, -1.0f)) * 0.5f + 0.5f;
        float sampleDepth = DepthTex.Sample(PointClampSampler, offset.xy);
        sampleDepth = CalculateViewSpaceFromDepth(sampleDepth, offset.xy).z;

        float rangeCheck = smoothstep(0.0f, 1.0f, ssaoRadius / abs(viewSpacePosition.z - sampleDepth));
        occlusion += step(sampleDepth, samplePos.z - 0.01) * rangeCheck;
    }
    occlusion /= 64.0f;
    float factor = 1 - occlusion;
    return pow(abs(factor), ssaoPower);
}