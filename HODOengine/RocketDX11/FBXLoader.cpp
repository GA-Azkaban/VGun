#include "FBXLoader.h"

namespace RocketCore::Graphics
{
	FBXLoader::FBXLoader()
	{

	}

	void FBXLoader::Initialize()
	{

	}

	bool FBXLoader::DoTheImportThing(const std::string& path, Entity* parentEntity)
	{
        Assimp::Importer importer;
 
        unsigned int flag;
        flag = aiProcess_Triangulate |
            aiProcess_JoinIdenticalVertices |
            aiProcess_CalcTangentSpace |
            aiProcess_GenNormals |
            aiProcess_MakeLeftHanded |
            aiProcess_FlipWindingOrder |
            aiProcess_FlipUVs;

// 		if (RenderAPI::GetAPI() == RenderAPI::API::DirectX11)
// 			flag |= aiProcess_FlipUVs;
 
        const aiScene* scene = importer.ReadFile(path, flag);
 
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            std::string error = "Error:Assimp:";
            QCAT_ASSERT(false, error + importer.GetErrorString());
        }
	}
}
