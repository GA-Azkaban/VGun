#pragma once
#include <string>
#include <vector>
#include "Singleton.h"


namespace HDEngine
{
	class MaterialLoader : public Singleton<MaterialLoader>
	{
		friend Singleton;

	private:
		MaterialLoader();

		struct MaterialInfo
		{
			std::string name;
			std::string mainTexture;
			std::string metalicTexture;
			int color[4];
			float roughness;
		};

	public:
		void LoadMaterialData(std::string filename);

	private:
		void LoadFromJson(std::string filePath);
		void CreateMaterial();

	private:
		std::vector<MaterialInfo> _materialList;
	};
}
