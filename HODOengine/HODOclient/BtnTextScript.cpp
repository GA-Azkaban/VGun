#include "BtnTextScript.h"

BtnTextScript::BtnTextScript(std::string Name)
	:imgName(Name),
	_childTextObject(nullptr),
	_currentCursor(nullptr)

{

}

BtnTextScript::~BtnTextScript()
{

}

void BtnTextScript::Start()
{
	auto vec = this->GetGameObject()->GetChildGameObjects();
	for (auto& obj : vec)
	{
		if (obj->GetComponent<HDData::TextUI>())
		{
			_childTextObject = obj;
		}
	}
}

void BtnTextScript::Update()
{
	auto button = GetGameObject()->GetComponent<HDData::Button>();
	HCURSOR newCursor = LoadCursor(NULL, IDC_ARROW);
	if (button && button->GetButtonComp()->GetIsHovering())
	{
		newCursor = SetCursor(LoadCursor(NULL, IDC_HAND));
		_childTextObject->GetComponent<HDData::TextUI>()->SetColor(DirectX::Colors::Gold);
	}
	else
	{
		_childTextObject->GetComponent<HDData::TextUI>()->ReturnDefaultColor();
	}

	if (_currentCursor != newCursor)
	{
		_currentCursor = newCursor;
		SetCursor(newCursor);
	}
}
