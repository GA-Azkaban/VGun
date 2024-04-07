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

const std::string MODEL_PATH = "Resources/Models/";
const std::string SCENEDATA_PATH = "Resources/SceneData/";

namespace HDEngine
{
	SceneLoader::SceneLoader()
	{

	}

	void SceneLoader::LoadUnityScene(std::string fileName)
	{
		LoadFromJson(SCENEDATA_PATH + fileName);
		CreateObject();
		LinkHierachy();
		SetTransform();
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
			info.position.x = obj["Position"]["x"].GetFloat();
			info.position.y = obj["Position"]["y"].GetFloat();
			info.position.z = obj["Position"]["z"].GetFloat();
			info.rotation.x = obj["Rotation"]["x"].GetFloat();
			info.rotation.y = obj["Rotation"]["y"].GetFloat();
			info.rotation.z = obj["Rotation"]["z"].GetFloat();
			info.rotation.w = obj["Rotation"]["w"].GetFloat();
			info.scale.x = obj["Scale"]["x"].GetFloat();
			info.scale.y = obj["Scale"]["y"].GetFloat();
			info.scale.z = obj["Scale"]["z"].GetFloat();
			info.colliderType = obj["ColliderType"].GetInt();
			info.colliderCenter.x = obj["ColliderCenter"]["x"].GetFloat();
			info.colliderCenter.y = obj["ColliderCenter"]["y"].GetFloat();
			info.colliderCenter.z = obj["ColliderCenter"]["z"].GetFloat();
			info.boxColliderSize.x = obj["BoxColliderSize"]["x"].GetFloat();
			info.boxColliderSize.y = obj["BoxColliderSize"]["y"].GetFloat();
			info.boxColliderSize.z = obj["BoxColliderSize"]["z"].GetFloat();
			info.sphereColliderRadius = obj["SphereColliderRadius"].GetFloat();
			info.texture = obj["textureName"].GetString();

			_infoList.emplace_back(info);
		}
	}

	void SceneLoader::CreateObject()
	{
		HDData::Scene* currentScene = SceneSystem::Instance().GetCurrentScene();
		for (auto& info : _infoList)
		{
			HDData::GameObject* object = currentScene->CreateObject(info.name);

			std::string meshName = info.meshName;

			if (meshName == "Stair")
			{
				info.meshName = "Plane";
			}

			if (meshName == "Plane" ||
				meshName == "Cube")
			{
				HDData::MeshRenderer* meshRenderer = object->AddComponent<HDData::MeshRenderer>();
				meshRenderer->LoadMesh(meshName + ".fbx");
				meshRenderer->LoadAlbedoMap(info.texture + ".png");
			}
			else if (info.meshName != "")
			{
				HDData::MeshRenderer* meshRenderer = object->AddComponent<HDData::MeshRenderer>();
				meshRenderer->LoadMesh(info.meshName + ".fbx");
				meshRenderer->LoadAlbedoMap(info.texture + ".png");
			}

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
			object->SetParentObject(parent);
		}
	}

	void SceneLoader::SetTransform()
	{
		for (auto& info : _infoList)
		{
			HDData::GameObject* object = _gameObjectMap[info.id];
			object->GetTransform()->SetLocalPosition(info.position);
			object->GetTransform()->SetLocalRotation(info.rotation);
			object->GetTransform()->SetLocalScale(info.scale);
		}
	}
}
