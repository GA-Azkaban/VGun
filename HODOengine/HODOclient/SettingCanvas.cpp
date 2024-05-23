#include "SettingCanvas.h"

SettingCanvas& SettingCanvas::Instance()
{
	if (_instance == nullptr)
	{
		_instance = new SettingCanvas;
	}

	return *_instance;
}

SettingCanvas::SettingCanvas()
{
	API::CreateStaticComponent(this);
}

SettingCanvas::~SettingCanvas()
{

}

SettingCanvas* SettingCanvas::_instance = nullptr;

void SettingCanvas::Start()
{
	
}

void SettingCanvas::Update()
{

}
