#include "BtnHoveringScript.h"
#include "SoundManager.h"

BtnHoveringScript::BtnHoveringScript(std::string Name)
{

}

BtnHoveringScript::~BtnHoveringScript()
{

}

void BtnHoveringScript::Start()
{
	_btnObject = this->GetGameObject();
}

void BtnHoveringScript::Update()
{
	HCURSOR newCursor = LoadCursor(NULL, IDC_ARROW);
	if (GetGameObject()->GetComponent<HDData::Button>()->GetButtonComp()->GetIsHovering())
	{
		newCursor = SetCursor(LoadCursor(NULL, IDC_HAND));
		_btnObject->GetComponent<HDData::Button>()->SetColor(DirectX::Colors::Gold);

	}
	else
	{
		_btnObject->GetComponent<HDData::Button>()->ReturnDefaultColor();
	}

	if (_currentCursor != newCursor)
	{
		_currentCursor = newCursor;
		SetCursor(newCursor);
	}
}
