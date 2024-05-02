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
	auto test = GetGameObject();

	auto vec = this->GetGameObject()->GetChildGameObjects();
	for (auto& obj : vec)
	{
			_childObject = obj;
	}
}

void BtnScript::Update()
{

	auto test = GetGameObject()->GetComponent<HDData::Button>();

	if (GetGameObject()->GetComponent<HDData::Button>()->GetButtonComp()->GetIsHovering())
	{
		_childObject->GetComponent<HDData::TextUI>()->SetColor(DirectX::Colors::Aqua);
	}
	else
	{
		_childObject->GetComponent<HDData::TextUI>()->SetColor(DirectX::Colors::White);
	}

//	this->GetGameObject()->GetComponent<HDData::TextUI>()->GetIsHovering();

}
