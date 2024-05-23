#pragma once
#include "../HODOengine/HODO_API.h"

class SettingCanvas : public HDData::Script
{
public:
	static SettingCanvas& Instance();

private:
	SettingCanvas();
	~SettingCanvas();

	static SettingCanvas* _instance;

public:
	void Start() override;
	void Update() override;

private:
	HDData::GameObject* _settingCanvas; 
	HDData::GameObject* _resolutionText;
};

