#include <fstream>
#include <sstream>
#include <string>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

#include "MaterialLoader.h"
#include "../HODO3DGraphicsInterface/IMaterial.h"
#include "MaterialManager.h"

using rapidjson::Document;
using rapidjson::SizeType;
using rapidjson::Value;

const std::string MATERIALDATA_PATH = "Resources/MaterialData/";

namespace HDEngine
{
	MaterialLoader::MaterialLoader()
	{

	}

	void MaterialLoader::LoadMaterialData(std::string filename)
	{
		LoadFromJson(MATERIALDATA_PATH + filename);
		CreateMaterial();
	}

	void MaterialLoader::LoadFromJson(std::string filePath)
	{
		// 모든 값을 string으로 묶어서 '한 번' 가져온다
		std::string serializeDataOnce;
		std::ifstream inputFile(filePath);

		if (inputFile.is_open())
		{
			std::stringstream buffer;
			buffer << inputFile.rdbuf();
			serializeDataOnce = buffer.str();
			inputFile.close();
		}

		Document doc;
		doc.Parse(serializeDataOnce.c_str());

		for (SizeType i = 0; i < doc.Size(); ++i)
		{
			const Value& obj = doc[i];
			MaterialInfo info;

			info.name = obj["name"].GetString();
			info.mainTexture = obj["mainTex"].GetString();
			info.metalicTexture = obj["metalicTex"].GetString();

			const Value& color = obj["color"].GetArray();
			
			for (SizeType c = 0; c < color.Size(); c++) 
			{
				info.color[c] = color[c].GetInt();
			}

			info.roughness = obj["roughness"].GetFloat();

			_materialList.emplace_back(info);
		}
	}

	void MaterialLoader::CreateMaterial()
	{
		for (auto& mat : _materialList)
		{
			HDEngine::MaterialDesc desc;

			desc.materialName = mat.name;

			if (mat.mainTexture != "") { desc.albedo = mat.mainTexture; }
			if (mat.metalicTexture != "") { desc.metallic = mat.metalicTexture; }
			
			desc.color.x = mat.color[0];
			desc.color.y = mat.color[1];
			desc.color.z = mat.color[2];
			desc.color.w = mat.color[3];

			desc.roughness = mat.roughness;

			HDEngine::MaterialManager::Instance().CreateMaterial(desc);
		}
	}

}
