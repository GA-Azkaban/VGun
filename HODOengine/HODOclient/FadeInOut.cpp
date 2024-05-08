#include "FadeInOut.h"

FadeInOut& FadeInOut::Instance()
{
	if (_instance == nullptr)
	{
		_instance = new FadeInOut;
	}

	return *_instance;
}

FadeInOut* FadeInOut::_instance = nullptr;

FadeInOut::FadeInOut()
{
	API::CreateStaticComponent(this);
	_fadeCanvas = API::CreateStaticObject();
	_fadeCanvas->AddComponent<HDData::ImageUI>();
	_fadeCanvas->GetComponent<HDData::ImageUI>()->SetImage("black.png");
	_fadeCanvas->GetComponent<HDData::ImageUI>()->SetSortOrder(0.9f);
	_fadeCanvas->GetTransform()->SetPosition(960.0f,540.0f,0.f);
	_fadeCanvas->GetComponent<HDData::ImageUI>()->SetIsIgnoreFocus(true);
}

void FadeInOut::Start()
{

}

void FadeInOut::Update()
{

}

void FadeInOut::FadeIn()
{
	_fadeCanvas->GetComponent<HDData::ImageUI>()->FadeIn();
}

void FadeInOut::FadeOut()
{
	_fadeCanvas->GetComponent<HDData::ImageUI>()->FadeOut();
}

bool FadeInOut::GetComplete()
{
	return _fadeCanvas->GetComponent<HDData::ImageUI>()->GetComplete();
}

