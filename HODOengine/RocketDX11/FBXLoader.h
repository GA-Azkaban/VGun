#pragma once

#include <string>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

namespace RocketCore::Graphics
{
	class FBXLoader
	{
	public:
		FBXLoader();

		void Initialize();

		bool DoTheImportThing(const std::string& path, Entity* parentEntity);

	};
}
