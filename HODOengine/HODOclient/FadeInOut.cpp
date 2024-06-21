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
	UINT width = API::GetScreenWidth();
	UINT height = API::GetScreenHeight();

	API::CreateStaticComponent(this);
	_fadeCanvas = API::CreateStaticObject();
	_fadeCanvas->AddComponent<HDData::ImageUI>();
	_fadeCanvas->GetComponent<HDData::ImageUI>()->SetImage("black.png");
	_fadeCanvas->GetComponent<HDData::ImageUI>()->ChangeScale(static_cast<float>(width) / 1920, static_cast<float>(height) / 1080);
	//_fadeCanvas->GetComponent<HDData::ImageUI>()->ChangeScale(API::GetScreenWidth(), API::GetScreenHeight());
	_fadeCanvas->GetComponent<HDData::ImageUI>()->SetSortOrder(0.9f);
	_fadeCanvas->GetTransform()->SetPosition(API::GetScreenWidth() / 2.0f, API::GetScreenHeight() / 2.0f, 0);
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

