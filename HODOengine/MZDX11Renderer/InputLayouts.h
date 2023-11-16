#pragma once
#include "d3dx11effect.h"


class InputLayouts
{
public:
    static void InitAll(ID3D11Device* device);
    static void DestroyAll();

    static ID3D11InputLayout* PosColor;
    static ID3D11InputLayout* PosNormal;
    static ID3D11InputLayout* Basic32;
    static ID3D11InputLayout* StaticMeshGBufferInputLayout;
    static ID3D11InputLayout* SkinnedMeshGBufferInputLayout;
    static ID3D11InputLayout* YunuSkinnedMeshGBufferInputLayout;
    static ID3D11InputLayout* DeferredInputLayout;
    static ID3D11InputLayout* TexInputLayout;
};

class InputLayoutDesc
{
public:
    static const D3D11_INPUT_ELEMENT_DESC PosColor[2];
    // Init like const int A::a[4] = {0, 1, 2, 3}; in .cpp file.
    static const D3D11_INPUT_ELEMENT_DESC PosNormal[2];
    static const D3D11_INPUT_ELEMENT_DESC PosTex[2];

    /// 텍스쳐 예제로 추가. 이름은 용책과의 연관성을 위해서 그냥 유지함
    static const D3D11_INPUT_ELEMENT_DESC Basic32[3];
    static const D3D11_INPUT_ELEMENT_DESC PosNorTexTan[4];
    static const D3D11_INPUT_ELEMENT_DESC PosNormalTexTanSkinned[11];

};

