#pragma once
#include "YunuGraphicsInterface.h"
#include "MZDX11Renderer.h"
#include "DeferredRenderer.h"
#include "ResourceManager.h"

namespace yunuGIAdapter
{
    class ResourceManagerAdapter : public yunuGI::IResourceManager
    {
    public:

        virtual vector<string> GetMeshKeys() const override { return ResourceManager::Instance.Get().GetMeshKeys(); }
        virtual vector<string> GetSkeletonKeys() const override { return ResourceManager::Instance.Get().GetSkeletonKeys(); }
        virtual vector<string> GetTextureKeys() const override { return ResourceManager::Instance.Get().GetTextureKeys(); }
        virtual vector<string> AnimationKeys() const override { return ResourceManager::Instance.Get().GetAnimationKeys(); }
        virtual void LoadFile(const char* filePath) override
        {
            ResourceManager::Instance.Get().LoadFile(filePath);
        };
        virtual void UnloadResources() override
        {
        };
    };
}
