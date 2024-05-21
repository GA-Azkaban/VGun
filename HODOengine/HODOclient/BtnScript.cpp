#include "BtnScript.h"

BtnScript::BtnScript(std::string Name)
	:imgName(Name),
	_childTextObject(nullptr),
	_childImageObject(nullptr),
	_currentCursor(nullptr)
	
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
			_textFlag = true;
			_childTextObject = obj;
		}
		else if (obj->GetComponent<HDData::ImageUI>())
		{
			_imageFlag = true;
			_childImageObject = obj;
		}
	}
}

void BtnScript::Update()
{
	auto button = GetGameObject()->GetComponent<HDData::Button>();
	HCURSOR newCursor = LoadCursor(NULL, IDC_ARROW);
	if (button && button->GetButtonComp()->GetIsHovering())
	{
		newCursor = SetCursor(LoadCursor(NULL, IDC_HAND));
		if (_textFlag)
		{
			_childTextObject->GetComponent<HDData::TextUI>()->SetColor(DirectX::Colors::Aqua);
		}
		if (_imageFlag)
		{
			_childImageObject->GetComponent<HDData::ImageUI>()->SetColor(DirectX::Colors::Aqua);
		}
	}
	else
	{
		if (_textFlag)
		{
			_childTextObject->GetComponent<HDData::TextUI>()->ReturnDefaultColor();
		}
		if (_imageFlag)
		{
			_childImageObject->GetComponent<HDData::ImageUI>()->SetColor(DirectX::Colors::White);
		}
	}

	if (_currentCursor != newCursor)
	{
		_currentCursor = newCursor;
		SetCursor(newCursor);
	}
}
