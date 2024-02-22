#include "Sampler.hlsli"

cbuffer externalData : register(b0)
{
    float4x4 view;
    float4x4 projection;
    float4x4 inverseProjection;
    float4 kernel[64];
    float4 cameraPosition;
    float radius;
    float power;
    float2 windowSize;
}

struct VertexToPixel
{
    float4 position     : SV_POSITION; // vertex position
    float2 uv           : TEXCOORD;
};

Texture2D DepthTex : register(t0);
Texture2D NormalTex : register(t1);
Texture2D Noise     : register(t2);

float3 CalculateViewSpaceFromDepth(float depth, float2 texCoord)
{
    // clip space between [-1, 1]
    // flip y so that +ve y is upwards
    float2 clipXY = texCoord * 2.0 - 1.0;
    clipXY.y = -clipXY.y;

    // NOTE: depth is not linearized
    // Also in range [0, 1] due to DirectX Convention
    float4 clipSpace = float4(clipXY, depth, 1);
   
    float4 viewSpace = mul(clipSpace, inverseProjection);
    viewSpace /= viewSpace.w;

    return viewSpace.xyz;
}

float4 main(VertexToPixel input) : SV_TARGET
{
    float depth = DepthTex.Sample(LinearBorderSampler, input.uv).r;
    float3 viewSpacePosition = CalculateViewSpaceFromDepth(depth, input.uv);
    float3 normal = NormalTex.Sample(LinearBorderSampler, input.uv).rgb;
    normal = normalize(normal * 2.0f - 1.0f);
    float3 viewSpaceNormal = normalize(mul(normal, (float3x3)view));

    float2 noiseScale = windowSize / 4.0f;
    float3 randDir = Noise.Sample(PointClampSampler, input.uv * noiseScale).rgb;
    randDir = normalize(2 * randDir - 1);

    float3 tangent = normalize(randDir - viewSpaceNormal * dot(randDir, viewSpaceNormal));
    float3 bitangent = cross(viewSpaceNormal, tangent);
    float3x3 tbn = float3x3(tangent, bitangent, viewSpaceNormal);

    float occlusion = 0.0f;
    for (int i = 0; i < 64; ++i)
    {
        float3 sampleDir = mul(kernel[i].xyz, transpose(tbn));
        float3 samplePos = viewSpacePosition + sampleDir * radius;

        float4 offset = mul(float4(samplePos, 1.0f), projection);
        offset.xy = ((offset.xy / offset.w) * float2(1.0f, -1.0f)) * 0.5f + 0.5f;
        float sampleDepth = DepthTex.Sample(PointClampSampler, offset.xy);
        sampleDepth = CalculateViewSpaceFromDepth(sampleDepth, offset.xy).z;

        float rangeCheck = smoothstep(0.0f, 1.0f, radius / abs(viewSpacePosition.z - sampleDepth));
        occlusion += step(sampleDepth, samplePos.z - 0.01) * rangeCheck;
    }
    occlusion /= 64.0f;
    float factor = 1 - occlusion;
    return pow(abs(factor), power);
}