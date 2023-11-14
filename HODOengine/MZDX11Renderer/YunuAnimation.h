#pragma once
#include <DirectXMath.h>
#include <vector>
#include <string>
#include "Skeleton.h"
// Skeleton은 읽어온 뼈대의 계층구조와 뼈대의 초기 트랜스폼을 저장한다.
using namespace std;
using namespace DirectX;
class Skeleton;
class ResourceManager;
class YunuAnimation
{
public:
    YunuAnimation(Skeleton* targetSkeleton)
    {
        this->targetSkeleton = targetSkeleton;
        this->bonesKeyFrames.resize(targetSkeleton->GetBones().size());
    }
    struct BoneKeyFrame
    {
        float time;
        FXMVECTOR localPosition;
        FXMVECTOR localRotation;
        FXMVECTOR localScale;
    };
    const Skeleton* GetTargetSkeleton() { return targetSkeleton; }
    const vector<vector<BoneKeyFrame>>& GetBonesKeyFrames() { return bonesKeyFrames; }
    float GetDuration() { return duration; }
private:
    string animationName;
    float duration;
    Skeleton* targetSkeleton;
    vector<vector<BoneKeyFrame>> bonesKeyFrames;
    friend ResourceManager;
};