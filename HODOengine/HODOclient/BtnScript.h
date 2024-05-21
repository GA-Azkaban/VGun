#pragma once

#include "..\HODOengine\HODO_API.h"
namespace HDData
{
	class GameObject;
}

class BtnScript : public HDData::Script
{
public:
	BtnScript() = default;
	BtnScript(std::string Name);
	virtual ~BtnScript();

	virtual void Start() override;
	virtual void Update() override;

private:
	std::string imgName;

	HDData::GameObject* _childTextObject;
	bool _textFlag = false;
	HDData::GameObject* _childImageObject;
	bool _imageFlag = false;

	HCURSOR _currentCursor;
};

