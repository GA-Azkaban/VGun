#pragma once

#include <DirectXMath.h>
#include <vector>
#include <string>
#include <unordered_map>
// Skeleton은 읽어온 뼈대의 계층구조와 뼈대의 초기 트랜스폼을 저장한다.
using namespace std;
namespace fbxsdk
{
    class FbxNode;
}
class ResourceManager;
class Skeleton
{
public:
    struct Bone
    {
        std::string name;
        DirectX::XMMATRIX initialTM;
        DirectX::XMMATRIX initialTM_inverse;
        vector<int> childIndices;
    };
    vector<Bone>& GetBones() { return bones; }
    unordered_map<string, unsigned int> boneIndexMap;
private:
    vector<Bone> bones;
    vector<fbxsdk::FbxNode*> fbxNodes;
    friend ResourceManager;
};