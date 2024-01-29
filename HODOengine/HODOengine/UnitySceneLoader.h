#pragma once
#include <string>

namespace HDEngine
{
	class UnitySceneLoader
	{
	public:
		UnitySceneLoader();

		void LoadUnityScene(std::string filePath);
	};
}
