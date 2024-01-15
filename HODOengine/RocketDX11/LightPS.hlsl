Texture2D shaderTexture;
SamplerState SampleType;

cbuffer LightBuffer
{
    float4 diffuseColor;
    float3 lightDirection;
    float padding;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

float4 main(PixelInputType input) : SV_TARGET
{
    float4 textureColor;
    float3 lightDir;
    float lightIntensity;
    float4 color;

    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
    textureColor = shaderTexture.Sample(SampleType, input.tex);
    
    // 계산을 위해 빛 방향을 반전
    lightDir = -lightDirection;
    
    // 이 픽셀의 빛의 양을 계산
    lightIntensity = saturate(dot(input.normal, lightDir));
    
    // 빛의 강도와 결합 된 확산 색을 기준으로 최종 색상을 결정
    color = saturate(diffuseColor * lightIntensity);
    
    // 텍스처 픽셀과 최종 확산 색을 곱하여 최종 픽셀 색상 결과를 얻음
    color = color * textureColor;

    return color;
}