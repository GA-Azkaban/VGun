#pragma once
#include "../HODOengine/HODO_API.h"



class B
{
public:
	B()
	{

	}
	~B()
	{

	}

	void Initialize()
	{
		_scene = API::CreateScene("B");

		auto a = API::CreateButton(_scene);
		a->GetTransform()->SetPosition(300.f, 150.f, 0.f);
	}

private:
	HDData::Scene* _scene;


};
