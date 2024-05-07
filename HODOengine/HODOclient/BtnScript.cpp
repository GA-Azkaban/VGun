#include "BtnScript.h"

BtnScript::BtnScript(std::string Name)
	:imgName(Name),
	_childObject(nullptr)
{

}

BtnScript::~BtnScript()
{

}

void BtnScript::Start()
{
	auto vec = this->GetGameObject()->GetChildGameObjects();
	for (auto& obj : vec)
	{
		if(obj->GetComponent<HDData::TextUI>())
		{
			_childObject = obj;
		}
	}
}

void BtnScript::Update()
{
	if (GetGameObject()->GetComponent<HDData::Button>()->GetButtonComp()->GetIsHovering())
	{
		_childObject->GetComponent<HDData::TextUI>()->SetColor(DirectX::Colors::Aqua);
	}
	else
	{
		_childObject->GetComponent<HDData::TextUI>()->ReturnDefaultColor();
	}
}
