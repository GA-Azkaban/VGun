#pragma once
#include <vector>
#include <DirectXMath.h>

#ifdef MZPARSER_EXPORTS 
#define MZ_API __declspec(dllexport)
#else
#define MZ_API __declspec(dllimport)
#endif

namespace fbxsdk
{
    class FbxManager;
    class FbxScene;
}
#include "fbxsdk.h"
struct MyVertex
{
    float pos[3];
    float normal[3];
    float uv[2];
};


class MZ_API FbxLoader
{
public:
    FbxLoader();
    ~FbxLoader();

public:
    void Load(const char* fileName);

public:
    std::vector<MyVertex> GetVertexList();
    std::vector<unsigned int> GetIndexList();
    bool isTexCoordExist = false;

private:
    void Initialize();
    void SetGeometryBuffers();
    void Finalize();

private:
    FbxManager* m_pManager;
    FbxScene* m_pScene;

    std::vector<MyVertex> m_VertextList;
    std::vector<unsigned int> m_IndexList;

};

extern "C" __declspec(dllexport) FbxLoader * CreateFBXLoader();
extern "C" __declspec(dllexport) void ReleaseFBXLoader(FbxLoader * loader);

