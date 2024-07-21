#include "GameSetting.h"

GameSetting& GameSetting::Instance()
{
	if (_instance == nullptr)
	{
		_instance = new GameSetting;
	}

	return *_instance;
}

GameSetting* GameSetting::_instance;

GameSetting::GameSetting()
{
	API::CreateStaticComponent(this);

	auto width = API::GetScreenWidth();
	auto height = API::GetScreenHeight();

	_settingCanvas = API::CreateStaticObject("setting");
	_settingCanvas->GetTransform()->SetPosition(width / 2, height / 2, 0);
	auto img = _settingCanvas->AddComponent<HDData::ImageUI>();
	img->SetSortOrder(0.8f);
	img->SetIsIgnoreFocus(true);
	img->SetImage("alphaRefCanvas2.png");

	_bgmSoundController = API::CreateStaticObject("bgmSound", _settingCanvas);
	auto bgming = _bgmSoundController->AddComponent<HDData::ImageUI>();
	bgming->SetSortOrder(0.8f);
	_bgmSoundController->GetTransform()->SetPosition(960.f * width / 1920, 530, 0.f);
	bgming->SetImage("PrefBtntemp.png");

	_sfxSoundController = API::CreateStaticObject("sfxSound", _settingCanvas);
	auto sfximg = _sfxSoundController->AddComponent<HDData::ImageUI>();
	sfximg->SetSortOrder(0.8f);
	_sfxSoundController->GetTransform()->SetPosition(960.f * width / 1920, 650, 0.f);
	sfximg->SetImage("PrefBtntemp.png");

	_bgmSoundIndex = API::CreateStaticObject();
	_bgmSoundIndex->GetTransform()->SetPosition(600, 530, 0);
	auto bgmindex = _bgmSoundIndex->AddComponent<HDData::TextUI>();
	bgmindex->SetSortOrder(0.81f);
	bgmindex->SetFont("Resources/Font/KRAFTON_30.spritefont");
 	bgmindex->SetText("BGM");

	_bgmSoundSlider = API::CreateStaticSlider("bgmslider", _settingCanvas);
	_bgmSoundSlider->GetTransform()->SetPosition(1000, 530, 0);

	_sfxSoundIndex = API::CreateStaticObject();
	_sfxSoundIndex->GetTransform()->SetPosition(600, 650, 0);
	auto sfxindex = _sfxSoundIndex->AddComponent<HDData::TextUI>();
	sfxindex->SetSortOrder(0.81f);
	sfxindex->SetFont("Resources/Font/KRAFTON_30.spritefont");
	sfxindex->SetText("EFFECT");

	_sfxSoundSlider = API::CreateStaticSlider("sfxslider", _settingCanvas);
	_sfxSoundSlider->GetTransform()->SetPosition(1000, 650, 0);

}

GameSetting::~GameSetting()
{

}

void GameSetting::Start()
{
	_bgmSoundSlider->GetComponent<HDData::SliderUI>()->SetSortOrder(0.83f);
	_sfxSoundSlider->GetComponent<HDData::SliderUI>()->SetSortOrder(0.83f);
	SetSettingCanvasActive(false);
}

void GameSetting::Update()
{
	if (API::GetKeyDown(DIK_ESCAPE))
	{
		SetSettingCanvasActive(!_settingCanvas->GetSelfActive());
	}
}

void GameSetting::SetSettingCanvasActive(bool _isActive)
{
	_settingCanvas->SetSelfActive(_isActive);
	_bgmSoundController->SetSelfActive(_isActive);
	_sfxSoundController->SetSelfActive(_isActive);
	_bgmSoundIndex->SetSelfActive(_isActive);
	_bgmSoundSlider->SetSelfActive(_isActive);
	_bgmSoundSlider->GetComponent<HDData::SliderUI>()->SetActive(_isActive);
	_sfxSoundIndex->SetSelfActive(_isActive);
	_sfxSoundSlider->SetSelfActive(false);
	_sfxSoundSlider->GetComponent<HDData::SliderUI>()->SetActive(_isActive);
}
