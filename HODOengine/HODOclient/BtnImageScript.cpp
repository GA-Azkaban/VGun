#include "BtnImageScript.h"

BtnImageScript::BtnImageScript(std::string Name)
	:imgName(Name),
	_childImageObject(nullptr),
	_currentCursor(nullptr)
{

}

BtnImageScript::~BtnImageScript()
{

}

void BtnImageScript::Start()
{
	auto vec = this->GetGameObject()->GetChildGameObjects();
	for (auto& obj : vec)
	{
		if (obj->GetComponent<HDData::ImageUI>())
		{
			_childImageObject = obj;
		}
	}
}

void BtnImageScript::Update()
{
	auto button = GetGameObject()->GetComponent<HDData::Button>();
	HCURSOR newCursor = LoadCursor(NULL, IDC_ARROW);
	if (button && button->GetButtonComp()->GetIsHovering())
	{
		newCursor = SetCursor(LoadCursor(NULL, IDC_HAND));
		_childImageObject->GetComponent<HDData::ImageUI>()->SetColor(DirectX::Colors::Aqua);
	}
	else
	{
		_childImageObject->GetComponent<HDData::ImageUI>()->SetColor(DirectX::Colors::White);
	}

	if (_currentCursor != newCursor)
	{
		_currentCursor = newCursor;
		SetCursor(newCursor);
	}
}
