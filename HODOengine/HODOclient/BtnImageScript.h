#pragma once

#include "..\HODOengine\HODO_API.h"
namespace HDData
{
	class GameObject;
}

class BtnImageScript : public HDData::Script
{
public:
	BtnImageScript() = default;
	BtnImageScript(std::string Name);
	virtual ~BtnImageScript();

	virtual void Start() override;
	virtual void Update() override;

private:
	std::string imgName;

	HDData::GameObject* _childImageObject;

	HCURSOR _currentCursor;
};

