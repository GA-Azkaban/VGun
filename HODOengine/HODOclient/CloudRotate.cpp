#include "CloudRotate.h"

CloudRotateScript::CloudRotateScript(std::string name)
{

}

CloudRotateScript::~CloudRotateScript()
{

}

void CloudRotateScript::Start()
{
	_pivotObject = this->GetGameObject();
	//_pivotObject = this->GetGameObject()->GetChildGameObjects();
}

void CloudRotateScript::Update()
{
	// 구름들의 포인터로 받아 부모를 회전시키면 자식도 돌아감
	

}


