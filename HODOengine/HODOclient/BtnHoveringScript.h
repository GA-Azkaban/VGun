#pragma once

#include "..\HODOengine\HODO_API.h"
namespace HDData
{
	class GameObject;
}

class BtnHoveringScript : public HDData::Script
{
public:
	BtnHoveringScript() = default;
	BtnHoveringScript(std::string Name);
	virtual ~BtnHoveringScript();

	virtual void Start() override;
	virtual void Update() override;

	HDData::GameObject* _btnObject = nullptr;
	HCURSOR _currentCursor = nullptr;
};
