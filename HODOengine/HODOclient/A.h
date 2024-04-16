#pragma once
#include "../HODOengine/HODO_API.h"

class A
{
public:
	A()
	{

	}
	~A()
	{

	}

	void Initialize()
	{
		_scene = API::CreateScene("A");

		auto a = API::CreateButton(_scene);
		a->GetTransform()->SetPosition(100.f, 150.f, 0.f);
		a->GetComponent<HDData::Button>()->SetOnClickEvent([]()
			{
				API::LoadSceneByName("B");
			});


		auto player = API::CreateObject(_scene, "player");
		player->LoadFBXFile("SKM_TP_X_idle.fbx");
	}

private:
	HDData::Scene* _scene;

};
