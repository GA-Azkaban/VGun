#pragma once

#include "..\HODOengine\HODO_API.h"
namespace HDData
{
	class GameObject;
}

class BtnTextScript : public HDData::Script
{
public:
	BtnTextScript() = default;
	BtnTextScript(std::string Name);
	virtual ~BtnTextScript();

	virtual void Start() override;
	virtual void Update() override;

private:
	std::string imgName;

	HDData::GameObject* _childTextObject;

	HCURSOR _currentCursor;
};

