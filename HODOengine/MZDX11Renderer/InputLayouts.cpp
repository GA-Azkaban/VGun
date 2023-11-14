#include "InputLayouts.h"
#include "Effects.h"

#pragma region InputLayoutDesc

const D3D11_INPUT_ELEMENT_DESC InputLayoutDesc::PosColor[2] =
{
    {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
};

const D3D11_INPUT_ELEMENT_DESC InputLayoutDesc::PosNormal[2] =
{
    {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"NORMAL",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
};

const D3D11_INPUT_ELEMENT_DESC InputLayoutDesc::PosTex[2] =
{
    {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
};

const D3D11_INPUT_ELEMENT_DESC InputLayoutDesc::Basic32[3] =
{
    {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}
};
const D3D11_INPUT_ELEMENT_DESC InputLayoutDesc::PosNorTexTan[4] =
{
    {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0}
};
const D3D11_INPUT_ELEMENT_DESC InputLayoutDesc::PosNormalTexTanSkinned[11] =
{
    {.SemanticName = "POSITION",.SemanticIndex = 0,.Format = DXGI_FORMAT_R32G32B32_FLOAT,.InputSlot = 0,
    .AlignedByteOffset = 0,.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA,.InstanceDataStepRate = 0},
    {.SemanticName = "NORMAL",.SemanticIndex = 0,.Format = DXGI_FORMAT_R32G32B32_FLOAT,.InputSlot = 0,
    .AlignedByteOffset = 12,.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA,.InstanceDataStepRate = 0},
    {.SemanticName = "TEXCOORD",.SemanticIndex = 0,.Format = DXGI_FORMAT_R32G32_FLOAT,.InputSlot = 0,
    .AlignedByteOffset = 24,.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA,.InstanceDataStepRate = 0},
    {.SemanticName = "TANGENT",.SemanticIndex = 0,.Format = DXGI_FORMAT_R32G32B32_FLOAT,.InputSlot = 0,
    .AlignedByteOffset = 32,.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA,.InstanceDataStepRate = 0},
    {.SemanticName = "BLENDWEIGHT",.SemanticIndex = 0,.Format = DXGI_FORMAT_R32_FLOAT,.InputSlot = 0,
    .AlignedByteOffset = 44,.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA,.InstanceDataStepRate = 0},
    {.SemanticName = "BLENDWEIGHT",.SemanticIndex = 1,.Format = DXGI_FORMAT_R32_FLOAT,.InputSlot = 0,
    .AlignedByteOffset = 48,.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA,.InstanceDataStepRate = 0},
    {.SemanticName = "BLENDWEIGHT",.SemanticIndex = 2,.Format = DXGI_FORMAT_R32_FLOAT,.InputSlot = 0,
    .AlignedByteOffset = 52,.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA,.InstanceDataStepRate = 0},
    {.SemanticName = "BLENDINDICES",.SemanticIndex = 0,.Format = DXGI_FORMAT_R32_UINT,.InputSlot = 0,
    .AlignedByteOffset = 56,.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA,.InstanceDataStepRate = 0},
    {.SemanticName = "BLENDINDICES",.SemanticIndex = 1,.Format = DXGI_FORMAT_R32_UINT,.InputSlot = 0,
    .AlignedByteOffset = 60,.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA,.InstanceDataStepRate = 0},
    {.SemanticName = "BLENDINDICES",.SemanticIndex = 2,.Format = DXGI_FORMAT_R32_UINT,.InputSlot = 0,
    .AlignedByteOffset = 64,.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA,.InstanceDataStepRate = 0},
    {.SemanticName = "BLENDINDICES",.SemanticIndex = 3,.Format = DXGI_FORMAT_R32_UINT,.InputSlot = 0,
    .AlignedByteOffset = 68,.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA,.InstanceDataStepRate = 0},
};


#pragma endregion

#pragma region InputLayouts

ID3D11InputLayout* InputLayouts::PosColor = 0;
ID3D11InputLayout* InputLayouts::PosNormal = 0;
ID3D11InputLayout* InputLayouts::Basic32 = 0;
ID3D11InputLayout* InputLayouts::StaticMeshGBufferInputLayout = 0;
ID3D11InputLayout* InputLayouts::SkinnedMeshGBufferInputLayout = 0;
ID3D11InputLayout* InputLayouts::YunuSkinnedMeshGBufferInputLayout = 0;
ID3D11InputLayout* InputLayouts::DeferredInputLayout = 0;
ID3D11InputLayout* InputLayouts::TexInputLayout = 0;


void InputLayouts::InitAll(ID3D11Device* device)
{
    D3DX11_PASS_DESC passDesc;

    //
    // PosColor
    //
    Effects::ColorFX->ColorTech->GetPassByIndex(0)->GetDesc(&passDesc);
    HR(device->CreateInputLayout(InputLayoutDesc::PosColor, 2, passDesc.pIAInputSignature,
        passDesc.IAInputSignatureSize, &PosColor));

    //
    // PosNormal
    //
    Effects::BasicFX->Light1Tech->GetPassByIndex(0)->GetDesc(&passDesc);
    HR(device->CreateInputLayout(InputLayoutDesc::PosNormal, 2, passDesc.pIAInputSignature,
        passDesc.IAInputSignatureSize, &PosNormal));

    //
    // Basic32
    //
    Effects::BasicTexFX->Light1Tech->GetPassByIndex(0)->GetDesc(&passDesc);
    HR(device->CreateInputLayout(InputLayoutDesc::Basic32, 3, passDesc.pIAInputSignature,
        passDesc.IAInputSignatureSize, &Basic32));

    Effects::StaticMeshFX->GetGBufferTech->GetPassByIndex(0)->GetDesc(&passDesc);
    HR(device->CreateInputLayout(InputLayoutDesc::PosNorTexTan, 4, passDesc.pIAInputSignature,
        passDesc.IAInputSignatureSize, &StaticMeshGBufferInputLayout));

    Effects::YunuSkinnedMeshFX->deferredGeometryTech->GetPassByIndex(0)->GetDesc(&passDesc);
    HR(device->CreateInputLayout(InputLayoutDesc::PosNormalTexTanSkinned, 11, passDesc.pIAInputSignature,
        passDesc.IAInputSignatureSize, &YunuSkinnedMeshGBufferInputLayout));

    Effects::SkinnedMeshFX->GetGBufferTech->GetPassByIndex(0)->GetDesc(&passDesc);
    HR(device->CreateInputLayout(InputLayoutDesc::PosNormalTexTanSkinned, 11, passDesc.pIAInputSignature,
        passDesc.IAInputSignatureSize, &SkinnedMeshGBufferInputLayout));

    Effects::ComputeLightFX->ComputeLightingTech->GetPassByIndex(0)->GetDesc(&passDesc);
    HR(device->CreateInputLayout(InputLayoutDesc::PosTex, 2, passDesc.pIAInputSignature,
        passDesc.IAInputSignatureSize, &DeferredInputLayout));

    Effects::UIObjectFX->DrawUITech->GetPassByIndex(0)->GetDesc(&passDesc);
    HR(device->CreateInputLayout(InputLayoutDesc::PosTex, 2, passDesc.pIAInputSignature,
        passDesc.IAInputSignatureSize, &TexInputLayout));

}

void InputLayouts::DestroyAll()
{
    SafeRelease(TexInputLayout);
    SafeRelease(DeferredInputLayout);
    SafeRelease(SkinnedMeshGBufferInputLayout);
    SafeRelease(StaticMeshGBufferInputLayout);
    SafeRelease(Basic32);
    SafeRelease(PosNormal);
    SafeRelease(PosColor);
}

#pragma endregion