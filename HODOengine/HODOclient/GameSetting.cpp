#include "GameSetting.h"
#include "RoundManager.h"
#include "GameManager.h"
#include "PlayerMove.h"
#include "BtnTextScript.h"

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


	///////////////////////////// 상시로 뜨는 아이콘 (0.9) //////////////////////////////
	_settingText = API::CreateStaticObject("OnSet");
	_settingText->GetTransform()->SetPosition((API::GetScreenWidth() / 100 * 97) + 30, 60, 0);
	auto icontxt = _settingText->AddComponent<HDData::ImageUI>();
	icontxt->SetImage("escText.png");
	icontxt->ChangeScale(1.5, 1.5);
	icontxt->SetSortOrder(0.95);
	/////////////////////////////////////////////////////////////////////////////////

	//////////////////////////// 배경 캔버스 (0.8) /////////////////////////////////////////
	_settingBackground = API::CreateStaticObject("blackCanvas");
	_settingBackground->GetTransform()->SetPosition(width / 2, height / 2, 0);
	auto _settingBackgroundkimg = _settingBackground->AddComponent<HDData::ImageUI>();
	_settingBackgroundkimg->SetSortOrder(0.8f);
	_settingBackgroundkimg->SetIsIgnoreFocus(true);
	_settingBackgroundkimg->ChangeScale(1.1f, 1.1f);
	_settingBackgroundkimg->SetImage("settingBackground.png");
	//////////////////////////////////////////////////////////////////////////////////

	///////////////////////////// 반투명 캔버스 (0.81) ////////////////////////////////////////
	_settingCanvas = API::CreateStaticObject("setting");
	_settingCanvas->GetTransform()->SetPosition(width / 2, height / 2, 0);
	auto img = _settingCanvas->AddComponent<HDData::ImageUI>();
	img->SetSortOrder(0.81f);
	img->SetIsIgnoreFocus(true);
	img->SetImage("alphaRefCanvas2.png");

	_infoController = API::CreateStaticObject("myinfo", _settingCanvas);
	auto info = _infoController->AddComponent<HDData::ImageUI>();
	_infoController->GetTransform()->SetPosition(960.f * width / 1920, 500, 0.f);
	info->SetSortOrder(0.82f);
	info->SetIsIgnoreFocus(true);
	info->SetImage("PrefBtntemp.png");

	_bgmSoundController = API::CreateStaticObject("bgmSound", _settingCanvas);
	auto bgming = _bgmSoundController->AddComponent<HDData::ImageUI>();
	bgming->SetSortOrder(0.82f);
	bgming->SetIsIgnoreFocus(true);
	_bgmSoundController->GetTransform()->SetPosition(960.f * width / 1920, 650, 0.f);
	bgming->SetImage("PrefBtntemp.png");

	_sfxSoundController = API::CreateStaticObject("sfxSound", _settingCanvas);
	auto sfximg = _sfxSoundController->AddComponent<HDData::ImageUI>();
	sfximg->SetSortOrder(0.82f);
	sfximg->SetIsIgnoreFocus(true);
	_sfxSoundController->GetTransform()->SetPosition(960.f * width / 1920, 770, 0.f);
	sfximg->SetImage("PrefBtntemp.png");
	/////////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////// 게임 종료 버튼 ///////////////////////////////////////////////
	_gameExitButton = API::CreateStaticButton("exit", _settingCanvas);
	_gameExitButton->GetTransform()->SetPosition(width / 2, (440.f * height / 1080) + 400, 0);
	_gameExitButton->AddComponent<BtnTextScript>();
	_exitButtonIndex = API::CreateStaticObject("exitIndex", _gameExitButton);
	_exitButtonIndex->GetTransform()->SetPosition(900.f * width / 1920, (440.f * height / 1080) + 400, 0.f);
	auto exittxt = _exitButtonIndex->AddComponent<HDData::TextUI>();
	exittxt->SetSortOrder(0.9f);
	exittxt->SetText("GAME EXIT");
	exittxt->SetFont("Resources/Font/KRAFTON_40.spriteFont");
	exittxt->SetColor(DirectX::Colors::Red);
	/////////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////// 설정창 인덱스들 (0.82) //////////////////////////////////////
	_infoIndex = API::CreateStaticObject("infoindex", _settingCanvas);
	_infoIndex->GetTransform()->SetPosition(600, 500, 0.f);
	auto infoindexcomp = _infoIndex->AddComponent<HDData::TextUI>();
	infoindexcomp->SetSortOrder(0.83f);
	infoindexcomp->SetFont("Resources/Font/KRAFTON_30.spritefont");
	infoindexcomp->SetText("NICKNAME");

	_bgmSoundIndex = API::CreateStaticObject("bgmindex", _settingCanvas);
	_bgmSoundIndex->GetTransform()->SetPosition(600.f, 650, 0);
	auto bgmindex = _bgmSoundIndex->AddComponent<HDData::TextUI>();
	bgmindex->SetSortOrder(0.83f);
	bgmindex->SetFont("Resources/Font/KRAFTON_30.spritefont");
	bgmindex->SetText("BGM");

	_sfxSoundIndex = API::CreateStaticObject("sfxindex", _settingCanvas);
	_sfxSoundIndex->GetTransform()->SetPosition(600, 770, 0);
	auto sfxindex = _sfxSoundIndex->AddComponent<HDData::TextUI>();
	sfxindex->SetSortOrder(0.83f);
	sfxindex->SetFont("Resources/Font/KRAFTON_30.spritefont");
	sfxindex->SetText("EFFECT");
	///////////////////////////////////////////////////////////////////////////////////////////


	////////////////////////////////// 설정창 값들 (0.95) ///////////////////////////////////////
	_nicknameText = API::CreateStaticObject("nick", _settingCanvas);
	_nicknameText->GetTransform()->SetPosition(1000, 500, 0.f);
	auto nickcomp = _nicknameText->AddComponent<HDData::TextUI>();
	nickcomp->SetIsIgnoreFocus(true);
	nickcomp->SetSortOrder(0.95f);
	nickcomp->SetFont("Resources/Font/KRAFTON_30.spritefont");

	_bgmSoundSlider = API::CreateStaticSlider("bgmslider", _settingCanvas);
	_bgmSoundSlider->GetTransform()->SetPosition(1000, 650, 0);

	_sfxSoundSlider = API::CreateStaticSlider("sfxslider", _settingCanvas);
	_sfxSoundSlider->GetTransform()->SetPosition(1000, 770, 0);
	///////////////////////////////////////////////////////////////////////////////////////////////

}

GameSetting::~GameSetting()
{

}

void GameSetting::Start()
{
	_bgmSoundSlider->GetComponent<HDData::SliderUI>()->SetSortOrder(0.95f);
	_sfxSoundSlider->GetComponent<HDData::SliderUI>()->SetSortOrder(0.95f);
	_gameExitButton->GetComponent<HDData::Button>()->SetSortOrder(0.95f);
	_gameExitButton->GetComponent<HDData::Button>()->SetImage("AlphaBtn.png");
	_gameExitButton->GetComponent<HDData::Button>()->SetOnClickEvent(
		[=]()
		{
			GameManager::Instance()->QuitGame();
		});

	SetSettingCanvasActive(false);
}

void GameSetting::Update()
{
	if (API::GetKeyDown(DIK_ESCAPE))
	{
		SetSettingCanvasActive(!_settingCanvas->GetSelfActive());
		API::ShowWindowCursor(_settingCanvas->GetSelfActive());

		if (RoundManager::Instance()->GetIsRoundStart())
		{
			GameManager::Instance()->GetMyObject()->GetComponent<PlayerMove>()->_isIngamePlaying = !_settingCanvas->GetSelfActive();
			GameManager::Instance()->GetMyObject()->GetComponent<PlayerMove>()->SetMovable(!_settingCanvas->GetSelfActive());
		}
	}

	HideESCButton(!RoundManager::Instance()->GetIsRoundStart());

}

void GameSetting::SetSettingCanvasActive(bool _isActive)
{
	_exitButtonIndex->SetSelfActive(_isActive);
	_gameExitButton->SetSelfActive(_isActive);
	_settingCanvas->SetSelfActive(_isActive);
	_infoController->SetSelfActive(_isActive);
	_bgmSoundController->SetSelfActive(_isActive);
	_sfxSoundController->SetSelfActive(_isActive);
	_infoIndex->SetSelfActive(_isActive);
	_nicknameText->SetSelfActive(_isActive);
	_bgmSoundIndex->SetSelfActive(_isActive);
	_bgmSoundSlider->SetSelfActive(_isActive);
	_bgmSoundSlider->GetComponent<HDData::SliderUI>()->SetActive(_isActive);
	_sfxSoundIndex->SetSelfActive(_isActive);
	_sfxSoundSlider->SetSelfActive(_isActive);
	_sfxSoundSlider->GetComponent<HDData::SliderUI>()->SetActive(_isActive);
	_settingBackground->SetSelfActive(_isActive);
}

void GameSetting::SetMyNickname(std::string nick)
{
	_nicknameText->GetComponent<HDData::TextUI>()->SetText(nick);
}

void GameSetting::HideESCButton(bool isHide)
{
	_settingText->SetSelfActive(isHide);
}

bool GameSetting::GetSettingCanvasActive()
{
	return _settingCanvas->GetSelfActive();
}
