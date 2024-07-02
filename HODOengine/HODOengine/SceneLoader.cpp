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
		for (auto& pos : _spawnPoint)
		{
			pos = { 1, 2, 1 };
		}
	}

	void SceneLoader::LoadUnityScene(std::string fileName, HDData::Scene* scene)
	{
		now = scene;

		HDEngine::MaterialDesc mat;
		mat.materialName = "PolygonWestern_Texture_01_A";
		mat.albedo = "PolygonWestern_Texture_01_A.png";
		mat.metallic = "PolygonWestern_Texture_Metallic.png";

		_material = MaterialManager::Instance().CreateMaterial(mat);

		HDEngine::MaterialDesc plane;
		plane.color = { 142, 118, 89, 255 };

		_plane = MaterialManager::Instance().CreateMaterial(plane);

		LoadFromJson(SCENEDATA_PATH + fileName);
		CreateObject(scene);
		LinkHierachy();
		SetTransform();
	}

	Vector3* SceneLoader::GetRespawnPoint()
	{
		return _spawnPoint;
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

			const Value& matArray = obj["materials"];
			if (matArray.IsArray())
			{
				for (SizeType j = 0; j < matArray.Size(); ++j)
				{
					info.materials.push_back(matArray[j].GetString());
				}
			}

			_infoList.emplace_back(info);
		}
	}

	void SceneLoader::CreateObject(HDData::Scene* scene)
	{
		_gameObjectMap.clear();

		for (auto& info : _infoList)
		{
			HDData::GameObject* object = scene->CreateObject(info.name);

			std::string meshName = info.meshName;
			
			// 스폰위치를 위한 변수
			std::string objName = info.name;
			Vector3 tempPosition = info.position;

			HDData::MeshRenderer* meshRenderer = object->AddComponent<HDData::MeshRenderer>();

			if (meshName == "Plane")
			{
				info.meshName = "Cube";
				meshRenderer->LoadMesh("primitiveCube");
			}
			else if (info.meshName != "")
			{
				meshRenderer->LoadMesh("SM_" + info.meshName + ".fbx");
			}
			
			if (objName == "SpawnPoint")
			{
				// 스폰 위치 받아와서 배열에 추가
				_spawnPoint[_spawnIndex] = tempPosition;
				_spawnIndex++;
			}

			switch (info.colliderType)
			{
				case 1:	// box static
				{
					auto col = object->AddComponent<HDData::StaticBoxCollider>();
					col->SetPositionOffset({ -info.colliderCenter.x, info.colliderCenter.y, -info.colliderCenter.z });
					col->SetScaleOffset(info.boxColliderSize);
				}
				break;
				case 2:	// sphere dynamic
				{
					auto col = object->AddComponent<HDData::DynamicSphereCollider>();
					col->SetPositionOffset({ -info.colliderCenter.x, info.colliderCenter.y, -info.colliderCenter.z });
					col->SetScaleOffset({ info.sphereColliderRadius, info.sphereColliderRadius, info.sphereColliderRadius });
				}
				break;
				default:
					break;
			}

			int m_size = meshRenderer->GetMeshCount();

			for (int m = 0; m < m_size; m++)
			{
				if (info.meshName == "Cube")
				{
					meshRenderer->LoadMaterial(_plane, m);
				}
				else
				{
					meshRenderer->LoadMaterial(_material, m);
				}
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
			if (info.meshName == "Cube")
			{
				object->GetTransform()->SetLocalScale(info.scale.x, 0.1, info.scale.z);
				object->GetTransform()->Translate(info.position.x, -0.1, info.position.z);
			}
			else if (info.meshName == "2F")
			{
				object->GetTransform()->SetLocalScale(info.scale.x, 0.1, info.scale.z);
			}

			object->GetTransform()->Rotate(0, -180, 0);

		}
	}
}
