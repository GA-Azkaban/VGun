#pragma once
#include "LazyObjects.h"
#include "Skeleton.h"
#include "YunuAnimation.h"
//#include "FbxLoader.h"
#include "fbxsdk.h"
#include <map>
#include <unordered_map>
#include <string>
#include <windows.h>
#include <memory>
#include <d3d11.h>
#include <wrl.h> 

using namespace Microsoft::WRL;
namespace MZMeshData
{
    struct MeshData;
}
class ResourceManager
{
public:

public:
    ~ResourceManager();
    MZMeshData::MeshData* LoadMeshData(LPCSTR fileName);
    void LoadFile(LPCSTR filename);
    MZMeshData::MeshData* GetMeshData(LPCSTR key);
    Skeleton* GetSkeletonData(LPCSTR key);
    ID3D11ShaderResourceView* GetTextureSRV(LPCSTR key);
    YunuAnimation* GetYunuAnimation(LPCSTR key);

    vector<string> GetMeshKeys() { return GetResourceKeys(m_meshObjectList); }
    vector<string> GetSkeletonKeys() { return GetResourceKeys(m_skeletonMap); }
    vector<string> GetTextureKeys() { return GetResourceKeys(m_textureSRVMap); }
    vector<string> GetAnimationKeys() { return GetResourceKeys(m_animationMap); }
public:
    static MZRenderer::LazyObjects<ResourceManager> Instance;
    friend MZRenderer::LazyObjects<ResourceManager>;

private:
    template<typename ContainerType>
    vector<string> GetResourceKeys(const ContainerType& container)
    {
        vector<string> ret;
        //for (auto each : container)
        for (auto itr = container.begin(); itr != container.end(); itr++)
            ret.push_back(itr->first);
        return ret;
    }
    void LoadASE(LPCSTR fileName);
    void LoadFBX(LPCSTR fileName);
    void LoadDDS(LPCSTR fileName);

    void RegisterMeshData(LPCSTR key, MZMeshData::MeshData* mesh) { m_meshObjectList[key] = mesh; }
    void RegisterSkeletonData(LPCSTR key, Skeleton* skeleton) { m_skeletonMap[key] = unique_ptr<Skeleton>(skeleton); };
    void RegisterYunuAnimationData(LPCSTR key, YunuAnimation* animation) { m_animationMap[key] = unique_ptr<YunuAnimation>(animation); }
    vector<YunuAnimation*> GetYunuAnimations(FbxScene* rootScene, Skeleton* targetSkeleton);
    Skeleton* GetSkeletonData(FbxNode* skeletalRoot);
    MZMeshData::MeshData* GetMeshData(LPCSTR fileName, FbxNode* meshRoot);

    ResourceManager();
    // 사실 unorderedMap이 더 효율이 좋음. 조회 시간복잡도가 map은 O(log n), unordered는 O(1)임
    std::map<std::string, MZMeshData::MeshData*> m_meshObjectList;
    std::map<std::string, std::unique_ptr<Skeleton>> m_skeletonMap;
    std::unordered_map<std::string, ComPtr<ID3D11ShaderResourceView>> m_textureSRVMap;
    std::unordered_map<std::string, unique_ptr<YunuAnimation>> m_animationMap;
    FbxManager* mFbxManager;
};

