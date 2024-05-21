#include "BtnScript.h"

BtnScript::BtnScript(std::string Name)
	:imgName(Name),
	_childTextObject(nullptr),
	_childImageObject(nullptr)
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
		if (obj->GetComponent<HDData::TextUI>())
		{
			textFlag = true;
			_childTextObject = obj;
		}
		else if (obj->GetComponent<HDData::ImageUI>())
		{
			imageFlag = true;
			_childImageObject = obj;
		}
	}
}

void BtnScript::Update()
{
	auto button = GetGameObject()->GetComponent<HDData::Button>();
	if (button && button->GetButtonComp()->GetIsHovering())
	{
		if (textFlag)
		{
			_childTextObject->GetComponent<HDData::TextUI>()->SetColor(DirectX::Colors::Aqua);
		}
		if (imageFlag)
		{
			_childImageObject->GetComponent<HDData::ImageUI>()->SetColor(DirectX::Colors::Aqua);
		}
	}
	else
	{
		if (textFlag)
		{
			_childTextObject->GetComponent<HDData::TextUI>()->ReturnDefaultColor();
		}
		if (imageFlag)
		{
			_childImageObject->GetComponent<HDData::ImageUI>()->SetColor(DirectX::Colors::White);
		}
	}
}
