#include "MainMenu.h"
#include "MenuManager.h"
#include "NetworkManager.h"

#include "../HODOengine/AudioSource.h"
#include "BtnTextScript.h"
#include "BtnImageScript.h"
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
	int width = static_cast<int>(API::GetScreenWidth());
	int height = static_cast<int>(API::GetScreenHeight());

	int originalwidth = 4000;
	int originalHeight = 2248;

	int targetWidth = 2560;
	int targetHeight = 1440;

	float widthScale = static_cast<float>(targetWidth) / originalwidth;
	float heightScale = static_cast<float>(targetHeight) / originalHeight;
	float scale = (widthScale < heightScale) ? widthScale : heightScale;

	HDData::GameObject* mainCanvas = API::CreateImageBox(_scene, "mainmenuCanvas");
	auto mainCanvasImage = mainCanvas->GetComponent<HDData::ImageUI>();
	mainCanvasImage->SetImage("_blur_background_image.png");
	mainCanvasImage->SetSortOrder(0.1f);
	//mainCanvasImage->ChangeScale(static_cast<float>(width) / 1920, static_cast<float>(height) / 1080);
	mainCanvasImage->ChangeScale(scale, scale);
	mainCanvasImage->SetIsIgnoreFocus(true);
	mainCanvas->GetTransform()->SetPosition(960.f * width / 1920, 540.f * height / 1080, 0.f);
	mainCanvasImage->SetActive(true);
	_menuManager.Instance().SetMainMenuCanvas(mainCanvas);

	// fade option canvas
	// 페이드 아웃용 검은색 캔버스는 0.5f 1에 가까울수록 UI 낮을수록 배경
	HDData::GameObject* fadeCanvas = API::CreateImageBox(_scene, "fadeCanvas");
	fadeCanvas->GetComponent<HDData::ImageUI>()->SetImage("black.png");
	fadeCanvas->GetComponent<HDData::ImageUI>()->SetSortOrder(0.1f);
	fadeCanvas->GetComponent<HDData::ImageUI>()->ChangeScale(static_cast<float>(width) / 1920, static_cast<float>(height) / 1080);
	fadeCanvas->GetTransform()->SetPosition(960.0f * width / 1920, 540.f * height / 1080, 0.f);
	fadeCanvas->GetComponent<HDData::ImageUI>()->FadeOut(1.0f);
	fadeCanvas->GetComponent<HDData::ImageUI>()->SetIsIgnoreFocus(true);

	HDData::GameObject* main_controlCanvas = API::CreateImageBox(_scene, "controlObject", mainCanvas);
	main_controlCanvas->SetSelfActive(true);
	main_controlCanvas->GetTransform()->SetPosition(-500.0f * width / 1920, -500.0f * height / 1080, 0.0f);

	// gameLogo
	auto gameLogo = API::CreateImageBox(_scene, "gameLogo", main_controlCanvas);
	gameLogo->GetTransform()->SetPosition(200.0f, 950.0f, 0.0f);
	auto gameLogoComp = gameLogo->GetComponent<HDData::ImageUI>();
	gameLogoComp->SetSortOrder(0.11);
	gameLogoComp->SetImage("gameLogo.png");
	gameLogoComp->ChangeScale(0.4f, 0.4f);

	// TeamLogo
	auto teamLogo = API::CreateButton(_scene, "teamLogo", main_controlCanvas);
	teamLogo->GetTransform()->SetPosition(200.0f, 1250.0f, 0.0f);
	teamLogo->AddComponent<BtnHoveringScript>();
	auto teamLogoComp = teamLogo->GetComponent<HDData::Button>();
	teamLogoComp->SetImage("teamLogo.png");
	teamLogoComp->SetSortOrder(0.2f);
	teamLogoComp->ChangeScale(0.3f, 0.3f);

	// credit
	auto credit = API::CreateImageBox(_scene, "credit", teamLogo);
	credit->GetTransform()->SetPosition(API::GetScreenWidth() / 2, API::GetScreenHeight() / 2, 0.0f);
	credit->SetSelfActive(false);
	auto creditImg = credit->GetComponent<HDData::ImageUI>();
	creditImg->SetImage("joinCanvas.png");
	creditImg->ChangeScale(1.5f, 0.91875f);
	creditImg->SetSortOrder(0.6f);

	auto creditText = API::CreateImageBox(_scene, "creditPeople", credit);
	creditText->GetTransform()->SetPosition((API::GetScreenWidth() / 2)+115.0f, (API::GetScreenHeight() / 2)-25.0f, 0.0f);
	auto creditTextImg = creditText->GetComponent<HDData::ImageUI>();
	creditTextImg->SetImage("creditnew.png");
	creditTextImg->SetSortOrder(0.61f);

	// play->RoomEnter & make sequence
	HDData::GameObject* main_playBtn = API::CreateButton(_scene, "playBtn", main_controlCanvas);
	main_playBtn->GetComponent<HDData::Button>()->SetImage("AlphaBtn.png");
	main_playBtn->GetTransform()->SetPosition(130.f * width / 1920, 240.f * height / 1080, 0.6f);
	main_playBtn->GetComponent<HDData::Button>()->SetSortOrder(0.7f);
	main_playBtn->GetComponent<HDData::Button>()->ChangeScale(static_cast<float>(width) / 1920, static_cast<float>(height) / 1080);
	main_playBtn->AddComponent<BtnTextScript>();
	HDData::GameObject* main_playText = API::CreateTextbox(_scene, "playText", main_playBtn);
	main_playText->GetTransform()->SetPosition(main_playBtn->GetTransform()->GetPosition());
	main_playText->GetComponent<HDData::TextUI>()->SetFont("Resources/Font/KRAFTON_55.spriteFont");
	main_playText->GetComponent<HDData::TextUI>()->SetDefaultColor(DirectX::Colors::OrangeRed);
	main_playText->GetComponent<HDData::TextUI>()->SetSortOrder(0.6f);
	main_playText->GetComponent<HDData::TextUI>()->SetText("PLAY");

	// RoomEnter Btn
	HDData::GameObject* main_enterBtn = API::CreateButton(_scene, "roomEnter", main_playBtn);
	main_enterBtn->GetTransform()->SetPosition(365.f * width / 1920, 190.f * height / 1080, 0.55f);
	main_enterBtn->GetComponent<HDData::Button>()->SetImage("AlphaBtn.png");
	main_enterBtn->GetComponent<HDData::Button>()->SetSortOrder(0.62f);
	main_enterBtn->GetComponent<HDData::Button>()->ChangeScale(static_cast<float>(width) / 1920, static_cast<float>(height) / 1080);
	main_enterBtn->SetSelfActive(false);
	main_enterBtn->AddComponent<BtnTextScript>();
	HDData::GameObject* main_enterText = API::CreateTextbox(_scene, "roomEnterText", main_enterBtn);
	main_enterText->GetTransform()->SetPosition(main_enterBtn->GetTransform()->GetPosition());
	auto entertxt = main_enterText->GetComponent<HDData::TextUI>();
	entertxt->SetSortOrder(0.6f);
	entertxt->SetFont("Resources/Font/KRAFTON_40.spriteFont");
	entertxt->SetText("ENTER");

	// room List Canvas
	HDData::GameObject* enter_roomLstCanvas = API::CreateImageBox(_scene, "roomListCanvas", main_enterBtn);
	enter_roomLstCanvas->GetComponent<HDData::ImageUI>()->SetImage("joinCanvas.png");
	enter_roomLstCanvas->GetTransform()->SetPosition(960.0f * width / 1920, 540.0f * height / 1080, 0.0f);
	enter_roomLstCanvas->GetComponent<HDData::ImageUI>()->SetSortOrder(0.6f);
	enter_roomLstCanvas->GetComponent<HDData::ImageUI>()->ChangeScale(static_cast<float>(width) / 1920, static_cast<float>(height) / 1080);
	enter_roomLstCanvas->SetSelfActive(false);
	MenuManager::Instance().SetRoomListCanvas(enter_roomLstCanvas);

	// Refresh btn
	HDData::GameObject* refreshButton = API::CreateButton(_scene, "refreshBtn", enter_roomLstCanvas);
	refreshButton->GetTransform()->SetLocalPosition(330.0f * width / 1920, -330.0f * height / 1080, 0);
	refreshButton->AddComponent<BtnHoveringScript>();
	auto freshBtn = refreshButton->GetComponent<HDData::Button>();
	freshBtn->SetDefaultColor(DirectX::Colors::White);
	freshBtn->SetImage("flair_arrow_3.png");
	freshBtn->SetSortOrder(0.65f);
	freshBtn->ChangeScale(static_cast<float>(width) / 1920, static_cast<float>(height) / 1080);
	freshBtn->SetOnClickEvent(
		[]()
		{
			NetworkManager::Instance().SendRoomListRequest();
		}
	);

	// page move button
	HDData::GameObject* pageLeftButton = API::CreateButton(_scene, "pageLeft", enter_roomLstCanvas);
	pageLeftButton->GetTransform()->SetLocalPosition(-325.0f + (width - 1920) * (-0.17f), 0, 0);
	pageLeftButton->AddComponent<BtnHoveringScript>();
	auto lBtn = pageLeftButton->GetComponent<HDData::Button>();
	lBtn->SetImage("Left.png");
	lBtn->SetSortOrder(0.65f);
	lBtn->ChangeScale(static_cast<float>(width) / 1920, static_cast<float>(height) / 1080);
	lBtn->SetOnClickEvent([]() {
		if (MenuManager::Instance().currentPage > 1)
		{
			MenuManager::Instance().currentPage--;
			MenuManager::Instance().RenderRoomList();
		}
		});

	HDData::GameObject* pageRightButton = API::CreateButton(_scene, "pageRight", enter_roomLstCanvas);
	pageRightButton->GetTransform()->SetLocalPosition(325.0f + (width - 1920) * 0.17f, 0, 0);
	pageRightButton->AddComponent<BtnHoveringScript>();
	auto rBtn = pageRightButton->GetComponent<HDData::Button>();
	rBtn->SetImage("Right.png");
	rBtn->SetSortOrder(0.65f);
	rBtn->ChangeScale(static_cast<float>(width) / 1920, static_cast<float>(height) / 1080);
	rBtn->SetOnClickEvent([]() {
		if (MenuManager::Instance().currentPage < MenuManager::Instance().pageCount)
		{
			MenuManager::Instance().currentPage++;
			MenuManager::Instance().RenderRoomList();
		}
		});

	/// input room password canvas
	HDData::GameObject* enter_inputPasswordCanvas = API::CreateImageBox(_scene, "enter_inputPasswordCanvas", enter_roomLstCanvas);
	enter_inputPasswordCanvas->GetTransform()->SetPosition(960.0f * width / 1920, 540.0f * height / 1080, 0.0f);
	auto enter_psswordCanvasImg = enter_inputPasswordCanvas->GetComponent<HDData::ImageUI>();
	enter_psswordCanvasImg->SetImage("enterCheckCanvas.png");
	enter_psswordCanvasImg->SetSortOrder(0.7f);
	enter_psswordCanvasImg->ChangeScale(static_cast<float>(width) / 1920, static_cast<float>(height) / 1080);
	enter_inputPasswordCanvas->SetSelfActive(false);

	HDData::GameObject* inputPW_Text = API::CreateTextbox(_scene, "inputPW_Text", enter_inputPasswordCanvas);
	inputPW_Text->GetTransform()->SetLocalPosition(0.0f, -80.0f, 0.0f);
	auto inputPW_Texttxt = inputPW_Text->GetComponent<HDData::TextUI>();
	inputPW_Texttxt->SetSortOrder(0.71f);
	inputPW_Texttxt->SetText("Enter your password");

	HDData::GameObject* inputPW_inputBox = API::CreateTextInputBox(_scene, "inputPW_inputBox", enter_inputPasswordCanvas);
	inputPW_inputBox->GetTransform()->SetLocalPosition(0.0f, 0.0f, 0.0f);
	auto inputPW_inputBoxComp = inputPW_inputBox->GetComponent<HDData::TextInputBoxUI>();
	inputPW_inputBoxComp->SetSortOrder(0.73f);

	/// 이부분 이미지 수정
	HDData::GameObject* inputPW_enter = API::CreateButton(_scene, "inputPW_enter", enter_inputPasswordCanvas);
	inputPW_enter->GetTransform()->SetLocalPosition(-100.0f, 110.0f, 0.0f);
	auto inputPW_enterbtn = inputPW_enter->GetComponent<HDData::Button>();
	inputPW_enterbtn->SetImage("Button_02.png");
	inputPW_enterbtn->ChangeScale(0.45f, 0.6154f);
	inputPW_enterbtn->SetSortOrder(0.73f);
	//inputPW_enterbtn->ChangeScale(static_cast<float>(width) / 1920, static_cast<float>(height) / 1080);
	inputPW_enterbtn->SetOnClickEvent([=]()
		{
			std::string input = inputPW_inputBox->GetComponent<HDData::TextInputBoxUI>()->GetCurrentText();
			NetworkManager::Instance().SendRoomEnter(std::to_string(MenuManager::Instance().selectedRoomInfo->id), input);
		});

	HDData::GameObject* inputPW_exit = API::CreateButton(_scene, "inputPW_exit", enter_inputPasswordCanvas);
	inputPW_exit->GetTransform()->SetLocalPosition(100.0f, 110.0f, 0.0f);
	auto inputPW_exitbtn = inputPW_exit->GetComponent<HDData::Button>();
	inputPW_exitbtn->SetImage("Button_02.png");
	inputPW_exitbtn->ChangeScale(0.45f, 0.6154f);
	inputPW_exitbtn->SetSortOrder(0.73f);
	//inputPW_exitbtn->ChangeScale(static_cast<float>(width) / 1920, static_cast<float>(height) / 1080);
	inputPW_exitbtn->SetOnClickEvent([=]()
		{
			enter_inputPasswordCanvas->SetSelfActive(false);
		});

	/// room enter check button
	/// 좀 더 진한 이미지를 사용해야함
	HDData::GameObject* enter_enterCheckCanvas = API::CreateImageBox(_scene, "enterCheck", enter_roomLstCanvas);
	enter_enterCheckCanvas->GetTransform()->SetPosition(1600.0f * width / 1920, 540.0f * height / 1080, 0);
	auto enterCheckImg = enter_enterCheckCanvas->GetComponent<HDData::ImageUI>();
	enterCheckImg->SetImage("Button_vertical_02.png");
	enterCheckImg->SetSortOrder(0.7f);
	enter_enterCheckCanvas->SetSelfActive(false);

	MenuManager::Instance().SetCheckEnterCanvas(enter_enterCheckCanvas);

	HDData::GameObject* enter_enterBtn = API::CreateButton(_scene, "enterButton", enter_enterCheckCanvas);
	enter_enterBtn->GetTransform()->SetLocalPosition(0.0f, 300.0f, 0);
	enter_enterBtn->AddComponent<BtnTextScript>();
	auto enter_enterImg = enter_enterBtn->GetComponent<HDData::Button>();
	enter_enterImg->SetImage("Button_01.png");
	enter_enterImg->SetSortOrder(0.75);
	enter_enterImg->ChangeScale(0.45f, 0.6145f);
	enter_enterImg->SetOnClickEvent([=]()
		{
			MenuManager::Instance().ShowCheckEnterCanvas(false);
			NetworkManager::Instance().SendRoomEnter(std::to_string(MenuManager::Instance().selectedRoomInfo->id));
			enter_roomLstCanvas->SetSelfActive(false);
		});

	HDData::GameObject* enter_enterCheckText = API::CreateTextbox(_scene, "enterCheckText", enter_enterBtn);
	enter_enterCheckText->GetTransform()->SetLocalPosition(-25.0f, -5.0f, 0);
	auto enterText = enter_enterCheckText->GetComponent<HDData::TextUI>();
	enterText->SetText("Join");
	enterText->SetFont("Resources/Font/KRAFTON_25.spriteFont");
	enterText->SetColor(DirectX::Colors::OrangeRed);
	enterText->SetIsIgnoreFocus(true);
	enterText->SetSortOrder(0.71);
	enterText->ChangeScale(width / 1920.0f);

	// room
	float posY = 235.0f + (height - 1080) * 0.0138f;

	for (int i = 0; i < 5; ++i)
	{
		HDData::GameObject* enter_roomCanvas = API::CreateButton(_scene, "room", enter_roomLstCanvas);
		enter_roomCanvas->GetComponent<HDData::Button>()->SetImage("subCanvas_alpha_long.png");
		//enter_roomCanvas->GetTransform()->SetPosition((960.0f + 130.0f) * width / 1920, posY * height / 1080, 0);
		enter_roomCanvas->GetTransform()->SetPosition(960.0f * width / 1920, posY * height / 1080, 0);
		enter_roomCanvas->GetComponent<HDData::Button>()->SetSortOrder(0.65f);
		enter_roomCanvas->GetComponent<HDData::Button>()->ChangeScale(static_cast<float>(width) / 1920, static_cast<float>(height) / 1080);

		MenuManager::Instance()._roomObject[i].btn = enter_roomCanvas->GetComponent<HDData::Button>();

		HDData::GameObject* enter_roomTitle = API::CreateTextbox(_scene, "title", enter_roomCanvas);
		enter_roomTitle->GetTransform()->SetLocalPosition(-30, 10, 0);
		auto roomT = enter_roomTitle->GetComponent<HDData::TextUI>();
		roomT->SetFont("Resources/Font/KRAFTON_30.spritefont");
		roomT->SetText("Title");
		roomT->SetSortOrder(0.71f);
		roomT->SetColor(DirectX::Colors::BlanchedAlmond);

		enter_roomTitle->SetSelfActive(false);

		MenuManager::Instance()._roomObject[i].title = roomT;

		HDData::GameObject* enter_roomID = API::CreateTextbox(_scene, "roomID", enter_roomCanvas);
		enter_roomID->GetTransform()->SetLocalPosition(-230, 10, 0);
		auto roomid = enter_roomID->GetComponent<HDData::TextUI>();
		roomid->SetFont("Resources/Font/KRAFTON_25.spritefont");
		roomid->SetText("ID");
		roomid->SetColor(DirectX::Colors::BlanchedAlmond);
		roomid->SetSortOrder(0.71f);

		enter_roomID->SetSelfActive(false);

		MenuManager::Instance()._roomObject[i].id = roomid;

		HDData::GameObject* enter_maxCount = API::CreateImageBox(_scene, "maxCount", enter_roomCanvas);
		enter_maxCount->GetTransform()->SetLocalPosition(260.0f, -25.0f, 0);
		auto max = enter_maxCount->GetComponent<HDData::ImageUI>();
		max->SetImage("flair_number_6_outline.png");
		max->SetSortOrder(0.81);
		max->ChangeScale(static_cast<float>(width) / 1920, static_cast<float>(height) / 1080);

		enter_maxCount->SetSelfActive(false);

		MenuManager::Instance()._roomObject[i].maxCount = max;

		HDData::GameObject* enter_currentCount = API::CreateImageBox(_scene, "currentCount", enter_roomCanvas);
		enter_currentCount->GetTransform()->SetLocalPosition(210.0f, -25.0f, 0);
		auto current = enter_currentCount->GetComponent<HDData::ImageUI>();
		current->SetImage("flair_number_2_outline.png");
		current->SetSortOrder(0.71);
		current->ChangeScale(static_cast<float>(width) / 1920, static_cast<float>(height) / 1080);

		enter_currentCount->SetSelfActive(false);

		MenuManager::Instance()._roomObject[i].currentCount = current;

		posY += 150;

		enter_roomCanvas->GetComponent<HDData::Button>()->SetOnClickEvent([=]()
			{
				if (MenuManager::Instance()._roomList.size() <= MenuManager::Instance().currentPage * i)  return;

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
	main_makeBtn->GetTransform()->SetPosition(365.f * width / 1920, 290.f * height / 1080, 0.6f);
	main_makeBtn->GetComponent<HDData::Button>()->SetImage("AlphaBtn.png");
	main_makeBtn->GetComponent<HDData::Button>()->SetSortOrder(0.65f);
	main_makeBtn->GetComponent<HDData::Button>()->ChangeScale(static_cast<float>(width) / 1920, static_cast<float>(height) / 1080);
	main_makeBtn->SetSelfActive(false);
	main_makeBtn->AddComponent<BtnTextScript>();
	HDData::GameObject* main_makeText = API::CreateTextbox(_scene, "setRoomText", main_makeBtn);
	main_makeText->GetTransform()->SetPosition(main_makeBtn->GetTransform()->GetPosition());
	main_makeText->GetComponent<HDData::TextUI>()->SetFont("Resources/Font/KRAFTON_40.spriteFont");
	main_makeText->GetComponent<HDData::TextUI>()->SetText("MAKE");
	main_makeText->GetComponent<HDData::TextUI>()->SetSortOrder(0.6f);

	/// make room ui

	// room setting
	HDData::GameObject* make_canvas = API::CreateImageBox(_scene, "lobbyCanvas", main_makeBtn);
	make_canvas->GetComponent<HDData::ImageUI>()->SetImage("joinCanvas.png");
	make_canvas->GetTransform()->SetPosition(960.0f * width / 1920, 540.0f * height / 1080, 0.f);
	make_canvas->GetComponent<HDData::ImageUI>()->SetSortOrder(0.6f);
	make_canvas->GetComponent<HDData::ImageUI>()->ChangeScale(0.75f, 0.5f);
	make_canvas->SetSelfActive(false);

	// room name Set
	HDData::GameObject* make_roomNameLabel = API::CreateTextbox(_scene, "roomNameTextLabel", make_canvas);
	//make_roomNameLabel->GetTransform()->SetPosition((710.0f * width / 1920), 240.0f * height / 1080, 0.f);
	make_roomNameLabel->GetTransform()->SetPosition(API::GetScreenWidth() / 2, 450.0f * height / 1080, 0.0f);
	make_roomNameLabel->GetComponent<HDData::TextUI>()->SetFont("Resources/Font/KRAFTON_30.spriteFont");
	make_roomNameLabel->GetComponent<HDData::TextUI>()->SetText("RoomName");
	make_roomNameLabel->GetComponent<HDData::TextUI>()->SetColor(DirectX::XMVectorSet(239.0f / 255.0f, 96.0f / 255.0f, 0.0f, 1.0f));
	make_roomNameLabel->GetComponent<HDData::TextUI>()->SetSortOrder(0.65f);
	if (width / 1920 > 1.0f)
	{
		make_roomNameLabel->GetComponent<HDData::TextUI>()->ChangeScale(width / 1920.0f * 1.35f);
	}

	HDData::GameObject* make_roomNameInput = API::CreateTextInputBox(_scene, "roomNameTextBox", make_canvas);
	//make_roomNameInput->GetTransform()->SetPosition(960.0f * width / 1920, 240.0f * height / 1080, 0.0f);
	make_roomNameInput->GetTransform()->SetPosition(API::GetScreenWidth() / 2, 500.0f * height / 1080, 0.0f);
	auto newRoomName = make_roomNameInput->GetComponent<HDData::TextInputBoxUI>();
	make_roomNameInput->GetComponent<HDData::TextInputBoxUI>()->GetBackgroundImage()->SetSortOrder(0.65f);
	make_roomNameInput->GetComponent<HDData::TextInputBoxUI>()->GetCursorImage()->SetSortOrder(0.65f);
	make_roomNameInput->GetComponent<HDData::TextInputBoxUI>()->GetTextUI()->SetSortOrder(0.65f);

	// room setting
	HDData::GameObject* roomSetBtn = API::CreateButton(_scene, "roomSet", make_canvas);
	roomSetBtn->GetTransform()->SetPosition(API::GetScreenWidth() / 2, 620.0f * height / 1080, 0.f);
	roomSetBtn->GetComponent<HDData::Button>()->SetSortOrder(0.8f);
	roomSetBtn->GetComponent<HDData::Button>()->SetImage("AlphaBtn.png");
	roomSetBtn->GetComponent<HDData::Button>()->SetSortOrder(0.68f);
	roomSetBtn->GetComponent<HDData::Button>()->ChangeScale(static_cast<float>(width) / 1920, static_cast<float>(height) / 1080);
	roomSetBtn->AddComponent<BtnTextScript>();

	HDData::GameObject* setRoomText = API::CreateTextbox(_scene, "setRoomText", roomSetBtn);
	setRoomText->GetTransform()->SetPosition(roomSetBtn->GetTransform()->GetPosition());
	setRoomText->GetComponent<HDData::TextUI>()->SetFont("Resources/Font/KRAFTON_40.spriteFont");
	setRoomText->GetComponent<HDData::TextUI>()->SetText("MAKE ROOM");
	setRoomText->GetComponent<HDData::TextUI>()->SetSortOrder(0.67f);

	//EXIT Btn
	HDData::GameObject* exit_Btn = API::CreateButton(_scene, "TestingBtn", main_controlCanvas);
	exit_Btn->GetTransform()->SetPosition((140.0f - (width - 1920) * 0.0078f) * width / 1920, 330.0f * height / 1080, 0.f);
	exit_Btn->GetComponent<HDData::Button>()->SetImage("AlphaBtn.png");
	exit_Btn->GetComponent<HDData::Button>()->SetSortOrder(0.7f);
	exit_Btn->GetComponent<HDData::Button>()->ChangeScale(static_cast<float>(width) / 1920, static_cast<float>(height) / 1080);
	exit_Btn->AddComponent<BtnTextScript>();
	HDData::GameObject* tempText = API::CreateTextbox(_scene, "tempText", exit_Btn);
	tempText->GetTransform()->SetPosition(exit_Btn->GetTransform()->GetPosition());
	tempText->GetComponent<HDData::TextUI>()->SetFont("Resources/Font/KRAFTON_40.spriteFont");
	tempText->GetComponent<HDData::TextUI>()->SetSortOrder(0.65f);
	tempText->GetComponent<HDData::TextUI>()->SetText("LOGOUT");

	// game play btn
	main_playBtn->GetComponent<HDData::Button>()->SetOnClickEvent(
		[=]()
		{
			credit->SetSelfActive(false);

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
			credit->SetSelfActive(false);

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

	main_makeBtn->GetComponent<HDData::Button>()->SetOnClickEvent(
		[=]()
		{
			credit->SetSelfActive(false);

			if (!make_canvas->GetSelfActive())
			{
				make_canvas->SetSelfActive(true);
			}
			else
			{
				make_canvas->SetSelfActive(false);
				make_roomNameInput->GetComponent<HDData::TextInputBoxUI>()->SetOrigin();
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
			credit->SetSelfActive(false);
			make_canvas->SetSelfActive(false);
			NetworkManager::Instance().SendRoomCreate(newRoomName->GetCurrentText(), "", 6, false);
			make_roomNameInput->GetComponent<HDData::TextInputBoxUI>()->SetOrigin();
		}
	);

	// game Logo & credit event
	teamLogo->GetComponent<HDData::Button>()->SetOnClickEvent(
		[=]()
		{
			make_canvas->SetSelfActive(false);
			enter_roomLstCanvas->SetSelfActive(false);
			if (credit->GetSelfActive())
			{
				credit->SetSelfActive(false);
			}
			else
			{
				credit->SetSelfActive(true);
			}
		}
	);

	exit_Btn->GetComponent<HDData::Button>()->SetOnClickEvent(
		[=]()
		{
			NetworkManager::Instance().SendLogout();
			credit->SetSelfActive(false);
		}
	);

}
