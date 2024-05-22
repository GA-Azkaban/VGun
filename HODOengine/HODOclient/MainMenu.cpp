#include "MainMenu.h"
#include "MenuManager.h"
#include "NetworkManager.h"
#include "FadeInOut.h"

#include "../HODOengine/AudioSource.h"
#include "BtnScript.h"
#include "BtnHoveringScript.h"

MainMenuScene::MainMenuScene()
	:_menuManager(MenuManager::Instance())
{

}

MainMenuScene::~MainMenuScene()
{

}

void MainMenuScene::Initalize()
{
	_scene = API::CreateScene("MainMenu");

	MainMenu();

	//API::LoadScene(_scene);

	// need	24.03.28
	// 페이드인 페이드 아웃,
	// 포커스시 크기 또는 색상변화
	// 스크롤링,세로 슬라이드 바
	// 총 맞았을때 화면 외곽 붉은 이펙트
	// 화염 연기이펙트(후순위)
	// 격발시 총구 이펙트
	// 블룸효과(포스트프로세싱)
	// setting
}

void MainMenuScene::MainMenu()
{
	//HDData::GameObject* bgmPlayer;
	//auto audio = bgmPlayer->AddComponent<HDData::AudioSource>();
	//audio->AddAudio();

	HDData::GameObject* main_Canvas = API::CreateImageBox(_scene, "mainmenuCanvas");
	main_Canvas->GetComponent<HDData::ImageUI>()->SetImage("_blur_background_image.png");
	main_Canvas->GetComponent<HDData::ImageUI>()->SetSortOrder(0.0f);
	main_Canvas->GetTransform()->SetPosition(960.f, 540.f, 0.f);
	main_Canvas->GetComponent < HDData::ImageUI>()->SetActive(true);
	_menuManager.Instance().SetMainMenuCanvas(main_Canvas);

	// fade option canvas
	// 페이드 아웃용 검은색 캔버스는 0.5f 1에 가까울수록 UI 낮을수록 배경
	HDData::GameObject* fadeCanvas = API::CreateImageBox(_scene, "fadeCanvas");
	fadeCanvas->GetComponent<HDData::ImageUI>()->SetImage("black.png");
	fadeCanvas->GetComponent<HDData::ImageUI>()->SetSortOrder(0.9f);
	fadeCanvas->GetTransform()->SetPosition(960.0f, 540.f, 0.f);
	fadeCanvas->GetComponent<HDData::ImageUI>()->FadeOut();
	fadeCanvas->GetComponent<HDData::ImageUI>()->SetIsIgnoreFocus(true);

	HDData::GameObject* main_controlCanvas = API::CreateImageBox(_scene, "controlObject", main_Canvas);
	main_controlCanvas->SetSelfActive(true);
	main_controlCanvas->GetTransform()->SetPosition(-500.0f, -500.0f, 0.0f);

	// play->RoomEnter & make sequence
	HDData::GameObject* main_playBtn = API::CreateButton(_scene, "playBtn", main_controlCanvas);
	main_playBtn->GetComponent<HDData::Button>()->SetImage("AlphaBtn.png");
	main_playBtn->GetTransform()->SetPosition(130.f, 240.f, 0.6f);
	main_playBtn->GetComponent<HDData::Button>()->SetSortOrder(0.6f);
	main_playBtn->AddComponent<BtnScript>();
	HDData::GameObject* main_playText = API::CreateTextbox(_scene, "playText", main_playBtn);
	main_playText->GetTransform()->SetPosition(main_playBtn->GetTransform()->GetPosition());
	main_playText->GetComponent<HDData::TextUI>()->SetFont("Resources/Font/KRAFTON_55.spriteFont");
	main_playText->GetComponent<HDData::TextUI>()->SetDefaultColor(DirectX::Colors::OrangeRed);
	main_playText->GetComponent<HDData::TextUI>()->SetText("PLAY");

	// RoomEnter Btn
	HDData::GameObject* main_enterBtn = API::CreateButton(_scene, "roomEnter", main_playBtn);
	main_enterBtn->GetTransform()->SetPosition(365.f, 190.f, 0.55f);
	main_enterBtn->GetComponent<HDData::Button>()->SetImage("AlphaBtn.png");
	main_enterBtn->GetComponent<HDData::Button>()->SetSortOrder(0.8f);
	main_enterBtn->SetSelfActive(false);
	main_enterBtn->AddComponent<BtnScript>();
	HDData::GameObject* main_enterText = API::CreateTextbox(_scene, "roomEnterText", main_enterBtn);
	main_enterText->GetTransform()->SetPosition(main_enterBtn->GetTransform()->GetPosition());
	main_enterText->GetComponent<HDData::TextUI>()->SetFont("Resources/Font/KRAFTON_40.spriteFont");
	main_enterText->GetComponent<HDData::TextUI>()->SetText("Enter");

	// room List Canvas
	HDData::GameObject* enter_roomLstCanvas = API::CreateImageBox(_scene, "roomListCanvas",main_enterBtn);
	enter_roomLstCanvas->GetComponent<HDData::ImageUI>()->SetImage("joinCanvas.png");
	enter_roomLstCanvas->GetTransform()->SetPosition(1000.0f, 540.0f, 0.0f);
	enter_roomLstCanvas->GetComponent<HDData::ImageUI>()->SetSortOrder(0.6f);
	enter_roomLstCanvas->SetSelfActive(false);
	MenuManager::Instance().SetRoomListCanvas(enter_roomLstCanvas);

	// Refresh btn
	HDData::GameObject* refreshButton = API::CreateButton(_scene, "refreshBtn", enter_roomLstCanvas);
	refreshButton->GetTransform()->SetLocalPosition(330, -330, 0);
	refreshButton->AddComponent<BtnHoveringScript>();
	auto freshBtn = refreshButton->GetComponent<HDData::Button>();
	freshBtn->SetDefaultColor(DirectX::Colors::White);
	freshBtn->SetImage("flair_arrow_3.png");
	freshBtn->SetSortOrder(0.6f);
	freshBtn->SetOnClickEvent(
		[freshBtn]()
		{
			NetworkManager::Instance().SendRoomListRequest();
		}

	);

	// page move button
	HDData::GameObject* pageLeftButton = API::CreateButton(_scene, "pageLeft", enter_roomLstCanvas);
	pageLeftButton->GetTransform()->SetLocalPosition(-300, 0, 0);
	pageLeftButton->AddComponent<BtnHoveringScript>();
	auto lBtn = pageLeftButton->GetComponent<HDData::Button>();
	lBtn->SetImage("leftArrow.png");
	lBtn->SetSortOrder(0.6f);
	lBtn->SetOnClickEvent([]() {
		if (MenuManager::Instance().currentPage > 1)
		{
			MenuManager::Instance().currentPage--;
			MenuManager::Instance().RenderRoomList();
		}
		});

	HDData::GameObject* pageRightButton = API::CreateButton(_scene, "pageRight", enter_roomLstCanvas);
	pageRightButton->GetTransform()->SetLocalPosition(300, 0, 0);
	pageRightButton->AddComponent<BtnHoveringScript>();
	auto rBtn = pageRightButton->GetComponent<HDData::Button>();
	rBtn->SetImage("rightArrow.png");
	rBtn->SetSortOrder(0.6f);
	rBtn->SetOnClickEvent([]() {
		if (MenuManager::Instance().currentPage < MenuManager::Instance().pageCount)
		{
			MenuManager::Instance().currentPage++;
			MenuManager::Instance().RenderRoomList();
		}
		});

	/// input room password canvas
	HDData::GameObject* enter_inputPasswordCanvas = API::CreateImageBox(_scene, "enter_inputPasswordCanvas", enter_roomLstCanvas);
	enter_inputPasswordCanvas->GetTransform()->SetPosition(960, 540, 0);
	auto enter_psswordCanvasImg = enter_inputPasswordCanvas->GetComponent<HDData::ImageUI>();
	enter_psswordCanvasImg->SetImage("enterCheckCanvas.png");
	enter_psswordCanvasImg->SetSortOrder(0.9);
	enter_inputPasswordCanvas->SetSelfActive(false);

	HDData::GameObject* inputPW_Text = API::CreateTextbox(_scene, "inputPW_Text", enter_inputPasswordCanvas);
	inputPW_Text->GetTransform()->SetLocalPosition(0, -80, 0);
	auto inputPW_Texttxt = inputPW_Text->GetComponent<HDData::TextUI>();
	inputPW_Texttxt->SetSortOrder(0.91);
	inputPW_Texttxt->SetText("Enter your password");

	HDData::GameObject* inputPW_inputBox = API::CreateTextInputBox(_scene, "inputPW_inputBox", enter_inputPasswordCanvas);
	inputPW_inputBox->GetTransform()->SetLocalPosition(0, 0, 0);
	auto inputPW_inputBoxComp = inputPW_inputBox->GetComponent<HDData::TextInputBoxUI>();
	inputPW_inputBoxComp->SetSortOrder(0.92);

	HDData::GameObject* inputPW_enter = API::CreateButton(_scene, "inputPW_enter", enter_inputPasswordCanvas);
	inputPW_enter->GetTransform()->SetLocalPosition(-100, 110, 0);
	auto inputPW_enterbtn = inputPW_enter->GetComponent<HDData::Button>();
	inputPW_enterbtn->SetImage("enterButton.png");
	inputPW_enterbtn->SetSortOrder(0.93);
	inputPW_enterbtn->SetOnClickEvent([=]()
		{
			std::string input = inputPW_inputBox->GetComponent<HDData::TextInputBoxUI>()->GetCurrentText();
			NetworkManager::Instance().SendRoomEnter(std::to_string(MenuManager::Instance().selectedRoomInfo->id), input);
		});

	HDData::GameObject* inputPW_exit = API::CreateButton(_scene, "inputPW_exit", enter_inputPasswordCanvas);
	inputPW_exit->GetTransform()->SetLocalPosition(100, 110, 0);
	auto inputPW_exitbtn = inputPW_exit->GetComponent<HDData::Button>();
	inputPW_exitbtn->SetImage("cancelButton.png");
	inputPW_exitbtn->SetSortOrder(0.93);
	inputPW_exitbtn->SetOnClickEvent([=]()
		{
			enter_inputPasswordCanvas->SetSelfActive(false);
		});


	/// room enter check button
	HDData::GameObject* enter_enterCheckCanvas = API::CreateImageBox(_scene, "enterCheck", enter_roomLstCanvas);
	enter_enterCheckCanvas->GetTransform()->SetPosition(960, 540, 0);
	auto enterCheckImg = enter_enterCheckCanvas->GetComponent<HDData::ImageUI>();
	enterCheckImg->SetImage("enterCheckCanvas.png");
	enterCheckImg->SetSortOrder(0.9);
	enter_enterCheckCanvas->SetSelfActive(false);

	HDData::GameObject* enter_enterCheckText = API::CreateTextbox(_scene, "enterCheckText", enter_enterCheckCanvas);
	enter_enterCheckText->GetTransform()->SetLocalPosition(0, -30, 0);
	auto enterText = enter_enterCheckText->GetComponent<HDData::TextUI>();
	enterText->SetText("Join?");
	enterText->SetIsIgnoreFocus(true);
	enterText->SetSortOrder(0.91);

	HDData::GameObject* enter_enterCheckExitBtn = API::CreateButton(_scene, "enterCheckExit", enter_enterCheckCanvas);
	enter_enterCheckExitBtn->GetTransform()->SetLocalPosition(-100, 80, 0);
	auto enter_exitImg = enter_enterCheckExitBtn->GetComponent<HDData::Button>();
	enter_exitImg->SetImage("cancelButton.png");
	enter_exitImg->SetSortOrder(0.91);
	enter_exitImg->SetOnClickEvent([=]()
		{
			MenuManager::Instance().ShowCheckEnterCanvas(false);
		});
	MenuManager::Instance().SetCheckEnterCanvas(enter_enterCheckCanvas);
	
	HDData::GameObject* enter_enterBtn = API::CreateButton(_scene, "enterButton", enter_enterCheckCanvas);
	enter_enterBtn->GetTransform()->SetLocalPosition(100, 80, 0);
	auto enter_enterImg = enter_enterBtn->GetComponent<HDData::Button>();
	enter_enterImg->SetImage("enterButton.png");
	enter_enterImg->SetSortOrder(0.91);
	enter_enterImg->SetOnClickEvent([=]()
		{
			NetworkManager::Instance().SendRoomEnter(std::to_string(MenuManager::Instance().selectedRoomInfo->id));
		});

	// room
	float posY = 230.f;

	for (int i = 0; i < 5; ++i)
	{
		HDData::GameObject* enter_roomCanvas = API::CreateButton(_scene, "room", enter_roomLstCanvas);
		enter_roomCanvas->GetComponent<HDData::Button>()->SetImage("subCanvas_alpha_long.png");
		enter_roomCanvas->GetTransform()->SetPosition(960, posY, 0);
		enter_roomCanvas->GetComponent<HDData::Button>()->SetSortOrder(0.8);

		MenuManager::Instance()._roomObject[i].btn = enter_roomCanvas->GetComponent<HDData::Button>();

		HDData::GameObject* enter_roomTitle = API::CreateTextbox(_scene, "title", enter_roomCanvas);
		enter_roomTitle->GetTransform()->SetLocalPosition(-80, 0, 0);
		auto roomT = enter_roomTitle->GetComponent<HDData::TextUI>();
		roomT->SetText("Title");
		roomT->SetSortOrder(0.81);
		roomT->SetColor(DirectX::Colors::BlanchedAlmond);

		enter_roomTitle->SetSelfActive(false);

		MenuManager::Instance()._roomObject[i].title = roomT;

		HDData::GameObject* enter_roomID = API::CreateTextbox(_scene, "roomID", enter_roomCanvas);
		enter_roomID->GetTransform()->SetLocalPosition(-200, 0, 0);
		auto roomid = enter_roomID->GetComponent<HDData::TextUI>();
		roomid->SetText("ID");
		roomid->SetColor(DirectX::Colors::BlanchedAlmond);
		roomid->SetSortOrder(0.81);

		enter_roomID->SetSelfActive(false);

		MenuManager::Instance()._roomObject[i].id = roomid;

		HDData::GameObject* enter_maxCount = API::CreateImageBox(_scene, "maxCount", enter_roomCanvas);
		enter_maxCount->GetTransform()->SetLocalPosition(280, 0, 0);
		auto max = enter_maxCount->GetComponent<HDData::ImageUI>();
		max->SetImage("flair_number_6_outline.png");
		max->SetSortOrder(0.81);

		enter_maxCount->SetSelfActive(false);

		MenuManager::Instance()._roomObject[i].maxCount = max;

		HDData::GameObject* enter_currentCount = API::CreateImageBox(_scene, "currentCount", enter_roomCanvas);
		enter_currentCount->GetTransform()->SetLocalPosition(320, 0, 0);
		auto current = enter_currentCount->GetComponent<HDData::ImageUI>();
		current->SetImage("flair_number_2_outline.png");
		current->SetSortOrder(0.81);

		enter_currentCount->SetSelfActive(false);

		MenuManager::Instance()._roomObject[i].currentCount = current;

		HDData::GameObject* enter_isPrivate = API::CreateImageBox(_scene, "isLock", enter_roomCanvas);
		enter_isPrivate->GetTransform()->SetLocalPosition(100, 0, 0);
		auto isP = enter_isPrivate->GetComponent<HDData::ImageUI>();
		isP->SetImage("icon_user_filled.png");
		isP->SetSortOrder(0.82);

		enter_isPrivate->SetSelfActive(false);

		MenuManager::Instance()._roomObject[i].isPrivate = isP;

		HDData::GameObject* enter_isTeam = API::CreateImageBox(_scene, "isTeam", enter_roomCanvas);
		enter_isTeam->GetTransform()->SetLocalPosition(150, 0, 0);
		auto isT = enter_isTeam->GetComponent<HDData::ImageUI>();
		isT->SetImage("icon_user_filled.png");
		isT->SetSortOrder(0.82);

		enter_isTeam->SetSelfActive(false);

		MenuManager::Instance()._roomObject[i].isTeam = isT;

		posY += 150;


		enter_roomCanvas->GetComponent<HDData::Button>()->SetOnClickEvent([=]()
			{
				MenuManager::Instance().selectedRoomInfo =
					MenuManager::Instance()._roomList[MenuManager::Instance().currentPage * i];

				if (MenuManager::Instance().selectedRoomInfo->isPrivate)
				{
					enter_inputPasswordCanvas->SetSelfActive(true);
				}
				else
				{
					MenuManager::Instance().ShowCheckEnterCanvas(true);
				}
			});
	}

	HDData::GameObject* main_makeBtn = API::CreateButton(_scene, "roomMake", main_playBtn);
	main_makeBtn->GetTransform()->SetPosition(365.f, 290.f, 0.6f);
	main_makeBtn->GetComponent<HDData::Button>()->SetImage("AlphaBtn.png");
	main_makeBtn->GetComponent<HDData::Button>()->SetSortOrder(0.9f);
	main_makeBtn->SetSelfActive(false);
	main_makeBtn->AddComponent<BtnScript>();
	HDData::GameObject* main_makeText = API::CreateTextbox(_scene, "setRoomText", main_makeBtn);
	main_makeText->GetTransform()->SetPosition(main_makeBtn->GetTransform()->GetPosition());
	main_makeText->GetComponent<HDData::TextUI>()->SetFont("Resources/Font/KRAFTON_40.spriteFont");
	main_makeText->GetComponent<HDData::TextUI>()->SetText("MAKE");

	/// make room ui

	// room setting
	HDData::GameObject* make_canvas = API::CreateImageBox(_scene, "lobbyCanvas", main_makeBtn);
	make_canvas->GetComponent<HDData::ImageUI>()->SetImage("joinCanvas.png");
	make_canvas->GetTransform()->SetPosition(960.0f, 540.0f, 0.f);
	make_canvas->GetComponent<HDData::ImageUI>()->SetSortOrder(0.7f);
	make_canvas->SetSelfActive(false);

	// room name Set
	HDData::GameObject* make_roomNameLabel = API::CreateTextbox(_scene, "reoomNameTextLabel", make_canvas);
	make_roomNameLabel->GetTransform()->SetPosition(710.0f, 240.0f, 0.f);
	make_roomNameLabel->GetComponent<HDData::TextUI>()->SetText("RoomName");
	make_roomNameLabel->GetComponent<HDData::TextUI>()->SetColor(DirectX::XMVectorSet(239.0f / 255.0f, 96.0f / 255.0f, 0.0f, 1.0f));
	make_roomNameLabel->GetComponent<HDData::TextUI>()->SetSortOrder(0.75f);

	HDData::GameObject* make_roomNameInput = API::CreateTextInputBox(_scene, "roomNameTextBox", make_canvas);
	make_roomNameInput->GetTransform()->SetPosition(960.0f, 240.0f, 0.0f);
	auto newRoomName = make_roomNameInput->GetComponent<HDData::TextInputBoxUI>();
	make_roomNameInput->GetComponent<HDData::TextInputBoxUI>()->GetBackgroundImage()->SetSortOrder(0.75f);
	make_roomNameInput->GetComponent<HDData::TextInputBoxUI>()->GetCursorImage()->SetSortOrder(0.75f);
	make_roomNameInput->GetComponent<HDData::TextInputBoxUI>()->GetTextUI()->SetSortOrder(0.77f);

	// privateRoom Set
	HDData::GameObject* make_passwordLabel = API::CreateTextbox(_scene, "privateRoomTextLabel", make_canvas);
	make_passwordLabel->GetTransform()->SetPosition(710.0f, 320.0f, 0.f);
	make_passwordLabel->GetComponent<HDData::TextUI>()->SetText("privateRoom");
	make_passwordLabel->GetComponent<HDData::TextUI>()->SetColor(DirectX::XMVectorSet(239.0f / 255.0f, 96.0f / 255.0f, 0.0f, 1.0f));
	make_passwordLabel->GetComponent<HDData::TextUI>()->SetSortOrder(0.75f);

	// privateCheckBox
	HDData::GameObject* make_isPrivateCheck = API::CreateToggle(_scene, "privateCheckBox", make_canvas);
	make_isPrivateCheck->GetComponent<HDData::ToggleUI>()->GetOnComp()->SetImage("checkbox_background.png");
	make_isPrivateCheck->GetComponent<HDData::ToggleUI>()->GetOffComp()->SetImage("checkbox_cross.png");
	make_isPrivateCheck->GetTransform()->SetPosition(1200.0f, 240.0f, 0.0f);
	make_isPrivateCheck->GetComponent<HDData::ToggleUI>()->SetSortOrder(0.75f);

	HDData::GameObject* roomPassWordTextBox = API::CreateTextInputBox(_scene, "roomPassWord", make_canvas);
	roomPassWordTextBox->GetTransform()->SetPosition(960.0f, 320.0f, 0.0f);
	auto newRoomPassWord = roomPassWordTextBox->GetComponent<HDData::TextInputBoxUI>();
	roomPassWordTextBox->GetComponent<HDData::TextInputBoxUI>()->GetBackgroundImage()->SetSortOrder(0.75f);
	roomPassWordTextBox->GetComponent<HDData::TextInputBoxUI>()->GetCursorImage()->SetSortOrder(0.75f);
	roomPassWordTextBox->GetComponent<HDData::TextInputBoxUI>()->GetTextUI()->SetSortOrder(0.75f);

	// room setting
	HDData::GameObject* roomSetBtn = API::CreateButton(_scene, "roomSet", make_canvas);
	roomSetBtn->GetTransform()->SetPosition(960.0f, 840.0f, 0.f);
	roomSetBtn->GetComponent<HDData::Button>()->SetSortOrder(0.8f);
	roomSetBtn->GetComponent<HDData::Button>()->SetImage("AlphaBtn.png");
	roomSetBtn->GetComponent<HDData::Button>()->SetSortOrder(0.8f);
	roomSetBtn->AddComponent<BtnScript>();

	HDData::GameObject* setRoomText = API::CreateTextbox(_scene, "setRoomText", roomSetBtn);
	setRoomText->GetTransform()->SetPosition(roomSetBtn->GetTransform()->GetPosition());
	setRoomText->GetComponent<HDData::TextUI>()->SetFont("Resources/Font/KRAFTON_40.spriteFont");
	setRoomText->GetComponent<HDData::TextUI>()->SetText("MAKE ROOM");
	setRoomText->GetComponent<HDData::TextUI>()->SetSortOrder(0.8f);

	// Training Btn
	HDData::GameObject* main_trainingBtn = API::CreateButton(_scene, "TestingBtn", main_controlCanvas);
	main_trainingBtn->GetTransform()->SetPosition(150.0f, 330.0f, 0.f);
	main_trainingBtn->GetComponent<HDData::Button>()->SetImage("AlphaBtn.png");
	main_trainingBtn->GetComponent<HDData::Button>()->SetSortOrder(0.6f);
	main_trainingBtn->AddComponent<BtnScript>();
	HDData::GameObject* trainingText = API::CreateTextbox(_scene, "trainingRoom", main_trainingBtn);
	trainingText->GetTransform()->SetPosition(main_trainingBtn->GetTransform()->GetPosition());
	trainingText->GetComponent<HDData::TextUI>()->SetFont("Resources/Font/KRAFTON_40.spriteFont");
	trainingText->GetComponent<HDData::TextUI>()->SetText("TRAINING");

	////EXIT Btn
	HDData::GameObject* exit_Btn = API::CreateButton(_scene, "TestingBtn", main_controlCanvas);
	exit_Btn->GetTransform()->SetPosition(100.0f, 430.0f, 0.f);
	exit_Btn->GetComponent<HDData::Button>()->SetImage("AlphaBtn.png");
	exit_Btn->GetComponent<HDData::Button>()->SetSortOrder(0.6f);
	exit_Btn->AddComponent<BtnScript>();
	HDData::GameObject* tempText = API::CreateTextbox(_scene, "tempText", exit_Btn);
	tempText->GetTransform()->SetPosition(exit_Btn->GetTransform()->GetPosition());
	tempText->GetComponent<HDData::TextUI>()->SetFont("Resources/Font/KRAFTON_40.spriteFont");
	tempText->GetComponent<HDData::TextUI>()->SetText("EXIT");

	// setting & option
	HDData::GameObject* preferencesBtn = API::CreateButton(_scene, "preferencesBtn");
	preferencesBtn->GetTransform()->SetPosition(1875.f, 30.f, 0.f);
	preferencesBtn->AddComponent<BtnHoveringScript>();
	auto prefBtn = preferencesBtn->GetComponent<HDData::Button>();
	prefBtn->SetImage("icon_cog.png");
	prefBtn->SetSortOrder(0.6f);

	// ex)해상도, BGM 볼륨, 환경볼륨, 마우스 감도
	HDData::GameObject* settingControlObject = API::CreateImageBox(_scene, "settingControlObject");
	settingControlObject->GetTransform()->SetPosition(-500.0f, -500.0f, 0.0f);
	settingControlObject->SetSelfActive(false);

	HDData::GameObject* preferencesCanvas = API::CreateImageBox(_scene, "Setting", settingControlObject);
	preferencesCanvas->GetTransform()->SetPosition(960.f, 540.f, 0.f);
	preferencesCanvas->GetComponent<HDData::ImageUI>()->SetImage("alphaRefCanvas2.png");
	preferencesCanvas->GetComponent<HDData::ImageUI>()->SetSortOrder(0.6f);
	preferencesCanvas->GetComponent<HDData::ImageUI>()->SetIsIgnoreFocus(true);

	// 해상도
	// default 1920x1080
	HDData::GameObject* resolutionText = API::CreateTextbox(_scene, "resolutionText", preferencesCanvas);
	resolutionText->GetTransform()->SetPosition(300.0f,250.0f,0.f);
	resolutionText->GetComponent<HDData::TextUI>()->SetFont("Resources/Font/KRAFTON_30.spritefont");
	resolutionText->GetComponent<HDData::TextUI>()->SetText("Resolution");
	resolutionText->GetComponent<HDData::TextUI>()->SetSortOrder(0.61f);
	HDData::GameObject* resolutionBtn1 = API::CreateButton(_scene, "resolutionBtn", preferencesCanvas);
	resolutionBtn1->GetTransform()->SetPosition(1350.0f, 240.0f, 0.f);
	resolutionBtn1->AddComponent<BtnScript>();
	resolutionBtn1->AddComponent<BtnHoveringScript>();
	resolutionBtn1->GetComponent<HDData::Button>()->SetImage("PrefLineBtn.png");
	resolutionBtn1->GetComponent<HDData::Button>()->SetSortOrder(0.62f);
	HDData::GameObject* screenSizeStat = API::CreateTextbox(_scene, "screenStat", resolutionBtn1);
	screenSizeStat->GetTransform()->SetPosition(1350.0f, 245.0f, 0.f);
	screenSizeStat->GetComponent<HDData::TextUI>()->SetFont("Resources/Font/KRAFTON_30.spriteFont");
	screenSizeStat->GetComponent<HDData::TextUI>()->SetSortOrder(0.61f);
	screenSizeStat->GetComponent<HDData::TextUI>()->SetText(MenuManager::Instance().GetScreenSize());
	HDData::GameObject* resolutionCanvas = API::CreateImageBox(_scene, "resolutionCanvas", preferencesCanvas);
	resolutionCanvas->GetTransform()->SetPosition(960.f, 240.0f, 0.f);
	resolutionCanvas->GetComponent<HDData::ImageUI>()->SetImage("PrefBtntemp.png");
	resolutionCanvas->GetComponent<HDData::ImageUI>()->SetSortOrder(0.61f);
	HDData::GameObject* resolutionToggleArrow = API::CreateImageBox(_scene, "resolutionToggleArrow", resolutionBtn1);
	resolutionToggleArrow->GetTransform()->SetPosition(1650.f, 240.f, 0.f);
	resolutionToggleArrow->GetComponent<HDData::ImageUI>()->SetImage("downArrow.png");
	resolutionToggleArrow->GetComponent<HDData::ImageUI>()->SetSortOrder(0.61f);

	HDData::GameObject* resolutionBtn2 = API::CreateButton(_scene, "resolutionBtn2", preferencesCanvas);
	resolutionBtn2->GetTransform()->SetPosition(1350.0f, 330.0f, 0.f);
	resolutionBtn2->AddComponent<BtnScript>();
	resolutionBtn2->AddComponent<BtnHoveringScript>();
	resolutionBtn2->GetComponent<HDData::Button>()->SetImage("PrefLineBtn.png");
	resolutionBtn2->GetComponent<HDData::Button>()->SetSortOrder(0.62f);
	resolutionBtn2->SetSelfActive(false);
	HDData::GameObject* resolutionText2 = API::CreateTextbox(_scene, "screenStat", resolutionBtn2);
	resolutionText2->GetTransform()->SetPosition(1350.0f, 335.0f, 0.f);
	resolutionText2->GetComponent<HDData::TextUI>()->SetFont("Resources/Font/KRAFTON_30.spriteFont");
	resolutionText2->GetComponent<HDData::TextUI>()->SetSortOrder(0.62f);
	resolutionText2->GetComponent<HDData::TextUI>()->SetText("1600x900(60Hz)");

	HDData::GameObject* resolutionBtn3 = API::CreateButton(_scene, "resolutionBtn3", preferencesCanvas);
	resolutionBtn3->GetTransform()->SetPosition(1350.0f, 420.0f, 0.f);
	resolutionBtn3->AddComponent<BtnScript>();
	resolutionBtn3->AddComponent<BtnHoveringScript>();
	resolutionBtn3->GetComponent<HDData::Button>()->SetImage("PrefLineBtn.png");
	resolutionBtn3->GetComponent<HDData::Button>()->SetSortOrder(0.62f);
	resolutionBtn3->SetSelfActive(false);
	HDData::GameObject* resolutionText3 = API::CreateTextbox(_scene, "screenStat", resolutionBtn3);
	resolutionText3->GetTransform()->SetPosition(1350.0f, 425.0f, 0.f);
	resolutionText3->GetComponent<HDData::TextUI>()->SetFont("Resources/Font/KRAFTON_30.spriteFont");
	resolutionText3->GetComponent<HDData::TextUI>()->SetSortOrder(0.62f);
	resolutionText3->GetComponent<HDData::TextUI>()->SetText("1920x1080(60Hz)");

	HDData::GameObject* resolutionBtn4 = API::CreateButton(_scene, "resolutionBtn3", preferencesCanvas);
	resolutionBtn4->GetTransform()->SetPosition(1350.0f, 510.0f, 0.f);
	resolutionBtn4->AddComponent<BtnScript>();
	resolutionBtn4->AddComponent<BtnHoveringScript>();
	resolutionBtn4->GetComponent<HDData::Button>()->SetImage("PrefLineBtn.png");
	resolutionBtn4->GetComponent<HDData::Button>()->SetSortOrder(0.62f);
	resolutionBtn4->SetSelfActive(false);
	HDData::GameObject* resolutionText4 = API::CreateTextbox(_scene, "screenStat", resolutionBtn4);
	resolutionText4->GetTransform()->SetPosition(1350.0f, 515.0f, 0.f);
	resolutionText4->GetComponent<HDData::TextUI>()->SetFont("Resources/Font/KRAFTON_30.spriteFont");
	resolutionText4->GetComponent<HDData::TextUI>()->SetSortOrder(0.62f);
	resolutionText4->GetComponent<HDData::TextUI>()->SetText("2550x1440(60Hz)");

	// BGM Volume
	HDData::GameObject* BGMVolumeText = API::CreateTextbox(_scene, "BGMVolumeText", preferencesCanvas);
	BGMVolumeText->GetTransform()->SetPosition(305.0f, 350.0f, 0.f);
	BGMVolumeText->GetComponent<HDData::TextUI>()->SetFont("Resources/Font/KRAFTON_30.spritefont");
	BGMVolumeText->GetComponent<HDData::TextUI>()->SetText("BGM Volume");
	BGMVolumeText->GetComponent<HDData::TextUI>()->SetSortOrder(0.61f);
	HDData::GameObject* BGMVolumeCanvas = API::CreateImageBox(_scene, "BGMVolumeCanvas", preferencesCanvas);
	BGMVolumeCanvas->GetTransform()->SetPosition(960.f, 340.0f, 0.f);
	BGMVolumeCanvas->GetComponent<HDData::ImageUI>()->SetImage("PrefBtntemp.png");
	BGMVolumeCanvas->GetComponent<HDData::ImageUI>()->SetSortOrder(0.61f);

	// Environment Volume
	HDData::GameObject* EnvironmentVolumeText = API::CreateTextbox(_scene, "EnvironmentVolumeText", preferencesCanvas);
	EnvironmentVolumeText->GetTransform()->SetPosition(390.0f, 450.0f, 0.f);
	EnvironmentVolumeText->GetComponent<HDData::TextUI>()->SetFont("Resources/Font/KRAFTON_30.spritefont");
	EnvironmentVolumeText->GetComponent<HDData::TextUI>()->SetText("Environment Volume");
	EnvironmentVolumeText->GetComponent<HDData::TextUI>()->SetSortOrder(0.61f);
	HDData::GameObject* EnvironmentVolumeCanvas = API::CreateImageBox(_scene, "EnvironmentVolumeCanvas", preferencesCanvas);
	EnvironmentVolumeCanvas->GetTransform()->SetPosition(960.f, 440.0f, 0.f);
	EnvironmentVolumeCanvas->GetComponent<HDData::ImageUI>()->SetImage("PrefBtntemp.png");
	EnvironmentVolumeCanvas->GetComponent<HDData::ImageUI>()->SetSortOrder(0.61f);

	// Mouse Sensitivity
	HDData::GameObject* MouseSensitivityText = API::CreateTextbox(_scene, "MouseSensitivityText", preferencesCanvas);
	MouseSensitivityText->GetTransform()->SetPosition(365.0f, 550.0f, 0.f);
	MouseSensitivityText->GetComponent<HDData::TextUI>()->SetFont("Resources/Font/KRAFTON_30.spritefont");
	MouseSensitivityText->GetComponent<HDData::TextUI>()->SetText("Mouse Sensitivity");
	MouseSensitivityText->GetComponent<HDData::TextUI>()->SetSortOrder(0.61f);
	HDData::GameObject* MouseSensitivityCanvas = API::CreateImageBox(_scene, "MouseSensitivityCanvas", preferencesCanvas);
	MouseSensitivityCanvas->GetTransform()->SetPosition(960.f, 540.0f, 0.f);
	MouseSensitivityCanvas->GetComponent<HDData::ImageUI>()->SetImage("PrefBtntemp.png");
	MouseSensitivityCanvas->GetComponent<HDData::ImageUI>()->SetSortOrder(0.61f);

	// 가로줄
	//HDData::GameObject* tempHorizonLine = API::CreateImageBox(_scene, "tempHorizonLine", preferencesCanvas);
	//tempHorizonLine->GetTransform()->SetPosition(960.f, 590.f, 0.f);
	//tempHorizonLine->GetComponent<HDData::ImageUI>()->SetImage("white_line_temp.png");
	//tempHorizonLine->GetComponent<HDData::ImageUI>()->SetSortOrder(0.61f);
	// 세로줄
	//HDData::GameObject* tempVerticalLine = API::CreateImageBox(_scene, "tempVerticalLine", preferencesCanvas);
	//tempVerticalLine->GetTransform()->SetPosition(650.f, 540, 0.f);
	//tempVerticalLine->GetComponent<HDData::ImageUI>()->SetImage("white_line_vertical.png");
	//tempVerticalLine->GetComponent<HDData::ImageUI>()->SetSortOrder(0.6f);

	// event
	// game play btn
	main_playBtn->GetComponent<HDData::Button>()->SetOnClickEvent(
		[=]()
		{
			if (!main_makeBtn->GetSelfActive() && !main_enterBtn->GetSelfActive())
			{
				main_makeBtn->SetSelfActive(true);
				main_enterBtn->SetSelfActive(true);
			}
			else
			{
				main_makeBtn->SetSelfActive(false);
				main_enterBtn->SetSelfActive(false);
				if (make_canvas->GetSelfActive() && enter_roomLstCanvas->GetSelfActive())
				{
					make_canvas->SetSelfActive(false);
					enter_roomLstCanvas->SetSelfActive(false);
				}
			}
		}
	);

	main_enterBtn->GetComponent<HDData::Button>()->SetOnClickEvent
	(
		[=]()
		{
			if (!enter_roomLstCanvas->GetSelfActive())
			{
				MenuManager::Instance().ShowRoomListCanvas(true);
			}
			else
			{
				MenuManager::Instance().ShowRoomListCanvas(false);
			}

			if (make_canvas->GetSelfActive())
			{
				make_canvas->SetSelfActive(false);
			}
		}
	);

	make_isPrivateCheck->GetComponent<HDData::ToggleUI>()->SetToggleOnEvent(
		[=]()
		{
			roomPassWordTextBox->GetComponent<HDData::TextInputBoxUI>()->GetBackgroundImage()->SetImage("back.png");
			roomPassWordTextBox->GetComponent<HDData::TextInputBoxUI>()->GetBackgroundImage()->SetIsIgnoreFocus(false);
		});

	make_isPrivateCheck->GetComponent<HDData::ToggleUI>()->SetToggleOffEvent(
		[=]()
		{
			roomPassWordTextBox->GetComponent<HDData::TextInputBoxUI>()->GetBackgroundImage()->SetImage("back_NotActive.png");
			roomPassWordTextBox->GetComponent<HDData::TextInputBoxUI>()->GetBackgroundImage()->SetIsIgnoreFocus(true);
		});

	main_makeBtn->GetComponent<HDData::Button>()->SetOnClickEvent(
		[=]()
		{
			if (!make_canvas->GetSelfActive())
			{
				make_canvas->SetSelfActive(true);
			}
			else
			{
				make_canvas->SetSelfActive(false);
			}

			if (enter_roomLstCanvas->GetSelfActive())
			{
				enter_roomLstCanvas->SetSelfActive(false);
			}
		}
	);

	roomSetBtn->GetComponent<HDData::Button>()->SetOnClickEvent
	(
		[=]()
		{
			NetworkManager::Instance().SendRoomCreate(newRoomName->GetCurrentText(), "", 6, false);
		}
	);

	// game setting
	preferencesBtn->GetComponent<HDData::Button>()->SetOnClickEvent(
		[=]()
		{
			if (main_controlCanvas->GetSelfActive())
			{
				main_controlCanvas->SetSelfActive(false);
				settingControlObject->SetSelfActive(true);
			}
			else
			{
				main_controlCanvas->SetSelfActive(true);
				settingControlObject->SetSelfActive(false);
			}
		}
	);

	main_trainingBtn->GetComponent<HDData::Button>()->SetOnClickEvent(
		[=]()
		{
			trainingText->GetComponent<HDData::TextUI>()->SetColor(DirectX::Colors::Aqua);
		}
	);

	//tempBtn->GetComponent<HDData::Button>()->SetOnClickEvent(
	//	[=]()
	//	{
	//		if (FadeInOut::Instance().GetComplete())
	//		{
	//			FadeInOut::Instance().FadeOut();
	//		}
	//		else
	//		{
	//			FadeInOut::Instance().FadeIn();
	//		}
	//	}
	//);	
	
	resolutionBtn1->GetComponent<HDData::Button>()->SetOnClickEvent(
		[=]()
		{
			if (!resolutionBtn2->GetSelfActive())
			{
				resolutionBtn2->SetSelfActive(true);
				resolutionBtn3->SetSelfActive(true);
				resolutionBtn4->SetSelfActive(true);
			}
			else if (resolutionBtn2->GetSelfActive())
			{
				resolutionBtn2->SetSelfActive(false);
				resolutionBtn3->SetSelfActive(false);
				resolutionBtn4->SetSelfActive(false);
			}
		}
	);

	resolutionBtn2->GetComponent<HDData::Button>()->SetOnClickEvent(
		[=]()
		{
			MenuManager::Instance().SetScreenSize(1);
			screenSizeStat->GetComponent<HDData::TextUI>()->SetText(MenuManager::Instance().GetScreenSize());
		}
	);

	resolutionBtn3->GetComponent<HDData::Button>()->SetOnClickEvent(
		[=]()
		{
			MenuManager::Instance().SetScreenSize(2);
			screenSizeStat->GetComponent<HDData::TextUI>()->SetText(MenuManager::Instance().GetScreenSize());
		}
	);
	resolutionBtn4->GetComponent<HDData::Button>()->SetOnClickEvent(
		[=]()
		{
			MenuManager::Instance().SetScreenSize(3);
			screenSizeStat->GetComponent<HDData::TextUI>()->SetText(MenuManager::Instance().GetScreenSize());
		}
	);


}
