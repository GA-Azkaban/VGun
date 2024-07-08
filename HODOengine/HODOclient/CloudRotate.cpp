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
	_cloudPos = API::GetCloudPosition();
}

void CloudRotateScript::Update()
{
	
}


