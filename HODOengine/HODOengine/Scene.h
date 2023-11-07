#pragma once
#include <string>
#include <vector>

#include "dllExporter.h"

namespace hodoEngine
{
	class GameObject;

	class HODO_API Scene
	{
	public:
		Scene(std::string sceneName);
		~Scene();

	public:
		void Start();
		void Update();

	public:
		GameObject* CreateObject(std::string objName);
		bool DeleteObject();

		std::vector<GameObject*>& GetObjList();
		std::vector<GameObject*>& GetActiveObjList();

	private:
		std::vector<GameObject*> _objList;
		std::vector<GameObject*> _activeObjList;

	public:
		std::string GetSceneName();
		void SetSceneName(std::string sceneName);

	private:
		std::string _scneneName;
		
	};

}


