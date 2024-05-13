#include "BtnHoveringScript.h"

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
	if (GetGameObject()->GetComponent<HDData::Button>()->GetButtonComp()->GetIsHovering())
	{
		_btnObject->GetComponent<HDData::Button>()->SetColor(DirectX::Colors::Aqua);
	}
	else
	{
		_btnObject->GetComponent<HDData::Button>()->SetColor(DirectX::Colors::White);
	}
}
