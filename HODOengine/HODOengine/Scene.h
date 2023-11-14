#pragma once
#include <string>
#include <vector>

#include "dllExporter.h"

namespace hodoEngine
{
	using ID = std::string;

	class GameObject;

	class HODO_API Scene
	{
	public:
		Scene() = default;
		Scene(std::string sceneName);
		~Scene();

	public:
		void Start();
		void Update();

	public:
		GameObject* FindObjectByID(ID uuid);
		bool IsScenePlaying();

		std::vector<ID>& GetObjList();

	private:
		bool _isPlaying;
		std::vector<ID> _ObjList;

	public:
		std::string GetSceneName();
		void SetSceneName(std::string sceneName);

	private:
		std::string _scneneName = "new scene";
		
	};

}


