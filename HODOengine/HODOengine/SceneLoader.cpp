#include <fstream>
#include <sstream>
#include <string>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

using rapidjson::Document;
using rapidjson::SizeType;
using rapidjson::Value;

#include "SceneLoader.h"

namespace HDEngine
{
	SceneLoader::SceneLoader()
	{

	}

	void SceneLoader::LoadUnityScene(std::string filePath)
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

// 		std::cout << doc[0]["ID"].GetInt() << std::endl;
// 		std::cout << doc[1]["ID"].GetInt() << std::endl;

		for (SizeType i = 0; i < doc.Size(); ++i)
		{
			const Value& obj = doc[i];

			//std::cout << obj["Name"].GetString() << std::endl;

			// 객체 하나를 선언
			//DataStruct data;
		}
	}

}
