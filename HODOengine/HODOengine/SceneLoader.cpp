#include <fstream>
#include <sstream>
#include <string>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

#include "SceneLoader.h"
#include "ObjectSystem.h"
#include "SceneSystem.h"
#include "GameObject.h"
#include "Scene.h"
#include "MeshRenderer.h"
#include "StaticBoxCollider.h"
#include "DynamicSphereCollider.h"

using rapidjson::Document;
using rapidjson::SizeType;
using rapidjson::Value;

const std::string RESOURCE_PATH = "Resources/";

namespace HDEngine
{
	SceneLoader::SceneLoader()
	{

	}

	void SceneLoader::LoadUnityScene(std::string filePath)
	{
		LoadFromJson(filePath);
		CreateObject();
		LinkHierachy();
	}

	void SceneLoader::LoadFromJson(std::string filePath)
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
			InfoStruct info;

			info.id = obj["ID"].GetInt();
			info.parentID = obj["ParentID"].GetInt();
			info.name = obj["Name"].GetString();
			info.meshName = obj["MeshName"].GetString();
			info.position.x = obj["Position"][0].GetFloat();
			info.position.y = obj["Position"][1].GetFloat();
			info.position.z = obj["Position"][2].GetFloat();
			info.rotation.x = obj["Rotation"][0].GetFloat();
			info.rotation.y = obj["Rotation"][1].GetFloat();
			info.rotation.z = obj["Rotation"][2].GetFloat();
			info.rotation.w = obj["Rotation"][3].GetFloat();
			info.scale.x = obj["Scale"][0].GetFloat();
			info.scale.y = obj["Scale"][1].GetFloat();
			info.scale.z = obj["Scale"][2].GetFloat();
			info.colliderType = obj["ColliderType"].GetInt();
			info.colliderCenter.x = obj["ColliderCenter"][0].GetFloat();
			info.colliderCenter.y = obj["ColliderCenter"][1].GetFloat();
			info.colliderCenter.z = obj["ColliderCenter"][2].GetFloat();
			info.boxColliderSize.x = obj["BoxColliderSize"][0].GetFloat();
			info.boxColliderSize.y = obj["BoxColliderSize"][1].GetFloat();
			info.boxColliderSize.z = obj["BoxColliderSize"][2].GetFloat();
			info.sphereColliderRadius = obj["SphereColliderRadius"].GetFloat();
		}
	}

	void SceneLoader::CreateObject()
	{
		HDData::Scene* currentScene = SceneSystem::Instance().GetCurrentScene();
		for (auto& info : _infoList)
		{
			HDData::GameObject* object = currentScene->CreateObject(info.name);
			HDData::MeshRenderer* meshRenderer = object->AddComponent<HDData::MeshRenderer>();
			switch (info.colliderType)
			{
				case 1:	// box static
				{
					auto col = object->AddComponent<HDData::StaticBoxCollider>();
					col->SetPositionOffset({ info.colliderCenter.x, info.colliderCenter.y, info.colliderCenter.z });
					col->SetWidth(info.boxColliderSize.x);
					col->SetHeight(info.boxColliderSize.y);
					col->SetDepth(info.boxColliderSize.z);
				}
					break;
				case 2:	// sphere dynamic
				{
					auto col = object->AddComponent<HDData::DynamicSphereCollider>();
					col->SetPositionOffset({ info.colliderCenter.x, info.colliderCenter.y, info.colliderCenter.z });
					col->SetRadius(info.sphereColliderRadius);
				}
					break;
				default:
					break;
			}
			meshRenderer->LoadMesh(RESOURCE_PATH + info.meshName);

			_gameObjectList.emplace_back(object);
			_gameObjectMap.insert(std::make_pair(info.id, object));
		}
	}

	void SceneLoader::LinkHierachy()
	{
		for (auto& info : _infoList)
		{
			if (info.parentID == 0)
			{
				continue;
			}

			HDData::GameObject* object = _gameObjectMap[info.id];
			HDData::GameObject* parent = _gameObjectMap[info.parentID];
			object->SetParent(parent);
		}
	}

}
