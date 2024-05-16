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

	API::LoadScene(_scene);

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

	HDData::GameObject* mainmenuCanvas = API::CreateImageBox(_scene, "mainmenuCanvas");
	mainmenuCanvas->GetComponent<HDData::ImageUI>()->SetImage("_blur_background_image.png");
	mainmenuCanvas->GetComponent<HDData::ImageUI>()->SetSortOrder(0.0f);
	mainmenuCanvas->GetTransform()->SetPosition(960.f, 540.f, 0.f);
	mainmenuCanvas->GetComponent < HDData::ImageUI>()->SetActive(true);
	_menuManager.Instance().SetMainMenuCanvas(mainmenuCanvas);

	// fade option canvas
	// 페이드 아웃용 검은색 캔버스는 0.5f 1에 가까울수록 UI 낮을수록 배경
	HDData::GameObject* fadeCanvas = API::CreateImageBox(_scene, "fadeCanvas");
	fadeCanvas->GetComponent<HDData::ImageUI>()->SetImage("black.png");
	fadeCanvas->GetComponent<HDData::ImageUI>()->SetSortOrder(0.9f);
	fadeCanvas->GetTransform()->SetPosition(960.0f, 540.f, 0.f);
	fadeCanvas->GetComponent<HDData::ImageUI>()->FadeOut();
	fadeCanvas->GetComponent<HDData::ImageUI>()->SetIsIgnoreFocus(true);

	HDData::GameObject* mainControlObject = API::CreateImageBox(_scene, "controlObject");
	mainControlObject->SetSelfActive(true);
	mainControlObject->GetTransform()->SetPosition(-500.0f, -500.0f, 0.0f);

	// play->RoomEnter & make sequence
	HDData::GameObject* playBtn = API::CreateButton(_scene, "playBtn", mainControlObject);
	playBtn->GetComponent<HDData::Button>()->SetImage("AlphaBtn.png");
	playBtn->GetTransform()->SetPosition(130.f, 240.f, 0.6f);
	playBtn->GetComponent<HDData::Button>()->SetSortOrder(0.6f);
	playBtn->AddComponent<BtnScript>();
	HDData::GameObject* playText = API::CreateTextbox(_scene, "playText", playBtn);
	playText->GetTransform()->SetPosition(playBtn->GetTransform()->GetPosition());
	playText->GetComponent<HDData::TextUI>()->SetFont("Resources/Font/KRAFTON_55.spriteFont");
	playText->GetComponent<HDData::TextUI>()->SetDefaultColor(DirectX::Colors::OrangeRed);
	playText->GetComponent<HDData::TextUI>()->SetText("PLAY");

	// RoomEnter Btn
	HDData::GameObject* roomEnterBtn = API::CreateButton(_scene, "roomEnter", playBtn);
	roomEnterBtn->GetTransform()->SetPosition(365.f, 190.f, 0.55f);
	roomEnterBtn->GetComponent<HDData::Button>()->SetImage("AlphaBtn.png");
	roomEnterBtn->GetComponent<HDData::Button>()->SetSortOrder(0.8f);
	roomEnterBtn->SetSelfActive(false);
	roomEnterBtn->AddComponent<BtnScript>();
	HDData::GameObject* roomEnterText = API::CreateTextbox(_scene, "roomEnterText", roomEnterBtn);
	roomEnterText->GetTransform()->SetPosition(roomEnterBtn->GetTransform()->GetPosition());
	roomEnterText->GetComponent<HDData::TextUI>()->SetFont("Resources/Font/KRAFTON_40.spriteFont");
	roomEnterText->GetComponent<HDData::TextUI>()->SetText("Enter");

	// room List Canvas
	HDData::GameObject* roomListCanvas = API::CreateImageBox(_scene, "roomListCanvas", roomEnterBtn);
	roomListCanvas->GetComponent<HDData::ImageUI>()->SetImage("joinCanvas.png");
	roomListCanvas->GetTransform()->SetPosition(960.0f, 540.0f, 0.0f);
	roomListCanvas->GetComponent<HDData::ImageUI>()->SetSortOrder(0.6f);
	roomListCanvas->SetSelfActive(false);
	MenuManager::Instance().SetRoomListCanvas(roomListCanvas);

	// page move button
	HDData::GameObject* pageLeftButton = API::CreateButton(_scene, "pageLeft", roomListCanvas);
	pageLeftButton->GetTransform()->SetLocalPosition(-500, 0, 0);
	auto lBtn = pageLeftButton->GetComponent<HDData::Button>();
	lBtn->SetImage("left.png");
	lBtn->SetSortOrder(0.6f);
	lBtn->SetOnClickEvent([]() {
		if (MenuManager::Instance().currentPage > 1)	MenuManager::Instance().currentPage--;
		});

	HDData::GameObject* pageRightButton = API::CreateButton(_scene, "pageRight", roomListCanvas);
	pageRightButton->GetTransform()->SetLocalPosition(500, 0, 0);
	auto rBtn = pageRightButton->GetComponent<HDData::Button>();
	rBtn->SetImage("right.png");
	rBtn->SetSortOrder(0.6f);
	rBtn->SetOnClickEvent([]() {
		if (MenuManager::Instance().currentPage < MenuManager::Instance().pageCount) MenuManager::Instance().currentPage++;
		});

	// room enter check button
	HDData::GameObject* enterCheckCanvas = API::CreateImageBox(_scene, "enterCheck", roomListCanvas);
	enterCheckCanvas->GetTransform()->SetPosition(960, 540, 0);
	auto enterCheckImg = enterCheckCanvas->GetComponent<HDData::ImageUI>();
	enterCheckImg->SetImage("enterCheckCanvas.png");
	enterCheckImg->SetSortOrder(0.9);
	enterCheckCanvas->SetSelfActive(false);

	HDData::GameObject* enterCheckText = API::CreateTextbox(_scene, "enterCheckText", enterCheckCanvas);
	enterCheckText->GetTransform()->SetLocalPosition(0, -30, 0);
	auto enterText = enterCheckText->GetComponent<HDData::TextUI>();
	enterText->SetText("Join?");
	enterText->SetSortOrder(0.91);

	HDData::GameObject* enterCheckExitButton = API::CreateButton(_scene, "enterCheckExit", enterCheckCanvas);
	enterCheckExitButton->GetTransform()->SetLocalPosition(-100, 80, 0);
	auto enterCheckexitImg = enterCheckExitButton->GetComponent<HDData::Button>();
	enterCheckexitImg->SetImage("cancelButton.png");
	enterCheckexitImg->SetSortOrder(0.91);

	HDData::GameObject* enterButton = API::CreateButton(_scene, "enterButton", enterCheckCanvas);
	enterButton->GetTransform()->SetLocalPosition(100, 80, 0);
	auto enterImg = enterButton->GetComponent<HDData::Button>();
	enterImg->SetImage("enterButton.png");
	enterImg->SetSortOrder(0.91);

	// room
	float posY = 230.f;

	for (int i = 0; i < 5; ++i)
	{
		HDData::GameObject* room = API::CreateButton(_scene, "room", roomListCanvas);
		room->GetComponent<HDData::Button>()->SetImage("subCanvas_alpha_long.png");
		room->GetTransform()->SetPosition(960, posY, 0);
		room->GetComponent<HDData::Button>()->SetSortOrder(0.8);

		MenuManager::Instance()._roomObject[i].btn = room->GetComponent<HDData::Button>();

		room->GetComponent<HDData::Button>()->SetOnClickEvent([=]()
			{
				enterCheckCanvas->SetSelfActive(true);
			});

		HDData::GameObject* roomTitle = API::CreateTextbox(_scene, "title", room);
		roomTitle->GetTransform()->SetLocalPosition(-80, 0, 0);
		auto roomT = roomTitle->GetComponent<HDData::TextUI>();
		roomT->SetText("Title");
		roomT->SetSortOrder(0.81);
		roomT->SetColor(DirectX::Colors::OrangeRed);

		roomTitle->SetSelfActive(false);

		MenuManager::Instance()._roomObject[i].title = roomT;

		HDData::GameObject* roomID = API::CreateTextbox(_scene, "roomID", room);
		roomID->GetTransform()->SetLocalPosition(-200, 0, 0);
		auto roomid = roomID->GetComponent<HDData::TextUI>();
		roomid->SetText("ID");
		roomid->SetColor(DirectX::Colors::OrangeRed);
		roomid->SetSortOrder(0.81);

		roomID->SetSelfActive(false);

		MenuManager::Instance()._roomObject[i].id = roomid;

		HDData::GameObject* roomMaxCount = API::CreateImageBox(_scene, "maxCount", room);
		roomMaxCount->GetTransform()->SetLocalPosition(280, 0, 0);
		auto max = roomMaxCount->GetComponent<HDData::ImageUI>();
		max->SetImage("flair_number_6_outline.png");
		max->SetSortOrder(0.81);

		roomMaxCount->SetSelfActive(false);

		MenuManager::Instance()._roomObject[i].maxCount = max;

		HDData::GameObject* roomCurrentCount = API::CreateImageBox(_scene, "currentCount", room);
		roomCurrentCount->GetTransform()->SetLocalPosition(320, 0, 0);
		auto current = roomCurrentCount->GetComponent<HDData::ImageUI>();
		current->SetImage("flair_number_2_outline.png");
		current->SetSortOrder(0.81);

		roomCurrentCount->SetSelfActive(false);

		MenuManager::Instance()._roomObject[i].currentCount = current;

		HDData::GameObject* isPrivateImage = API::CreateImageBox(_scene, "isLock", room);
		isPrivateImage->GetTransform()->SetLocalPosition(100, 0, 0);
		auto isP = isPrivateImage->GetComponent<HDData::ImageUI>();
		isP->SetImage("icon_user_filled.png");
		isP->SetSortOrder(0.82);

		isPrivateImage->SetSelfActive(false);

		MenuManager::Instance()._roomObject[i].isPrivate = isP;

		HDData::GameObject* isTeamImage = API::CreateImageBox(_scene, "isTeam", room);
		isTeamImage->GetTransform()->SetLocalPosition(150, 0, 0);
		auto isT = isTeamImage->GetComponent<HDData::ImageUI>();
		isT->SetImage("icon_user_filled.png");
		isT->SetSortOrder(0.82);

		isTeamImage->SetSelfActive(false);

		MenuManager::Instance()._roomObject[i].isTeam = isT;

		posY += 150;
	}

	HDData::GameObject* roomMakeBtn = API::CreateButton(_scene, "roomMake", playBtn);
	roomMakeBtn->GetTransform()->SetPosition(365.f, 290.f, 0.6f);
	roomMakeBtn->GetComponent<HDData::Button>()->SetImage("AlphaBtn.png");
	roomMakeBtn->GetComponent<HDData::Button>()->SetSortOrder(0.9f);
	roomMakeBtn->SetSelfActive(false);
	roomMakeBtn->AddComponent<BtnScript>();
	HDData::GameObject* makeRoomText = API::CreateTextbox(_scene, "setRoomText", roomMakeBtn);
	makeRoomText->GetTransform()->SetPosition(roomMakeBtn->GetTransform()->GetPosition());
	makeRoomText->GetComponent<HDData::TextUI>()->SetFont("Resources/Font/KRAFTON_40.spriteFont");
	makeRoomText->GetComponent<HDData::TextUI>()->SetText("MAKE");

	// room setting
	HDData::GameObject* setRoomCanvas = API::CreateImageBox(_scene, "lobbyCanvas", roomMakeBtn);
	setRoomCanvas->GetComponent<HDData::ImageUI>()->SetImage("joinCanvas.png");
	setRoomCanvas->GetTransform()->SetPosition(960.0f, 540.0f, 0.f);
	setRoomCanvas->GetComponent<HDData::ImageUI>()->SetSortOrder(0.7f);
	setRoomCanvas->SetSelfActive(false);

	// room name Set
	HDData::GameObject* roomNameTextLabel = API::CreateTextbox(_scene, "reoomNameTextLabel", setRoomCanvas);
	roomNameTextLabel->GetTransform()->SetPosition(710.0f, 240.0f, 0.f);
	roomNameTextLabel->GetComponent<HDData::TextUI>()->SetText("RoomName");
	roomNameTextLabel->GetComponent<HDData::TextUI>()->SetColor(DirectX::XMVectorSet(239.0f / 255.0f, 96.0f / 255.0f, 0.0f, 1.0f));
	roomNameTextLabel->GetComponent<HDData::TextUI>()->SetSortOrder(0.75f);

	HDData::GameObject* roomNameTextbox = API::CreateTextInputBox(_scene, "roomNameTextBox", setRoomCanvas);
	roomNameTextbox->GetTransform()->SetPosition(960.0f, 240.0f, 0.0f);
	auto newRoomName = roomNameTextbox->GetComponent<HDData::TextInputBoxUI>();
	roomNameTextbox->GetComponent<HDData::TextInputBoxUI>()->GetBackgroundImage()->SetSortOrder(0.75f);
	roomNameTextbox->GetComponent<HDData::TextInputBoxUI>()->GetCursorImage()->SetSortOrder(0.75f);
	roomNameTextbox->GetComponent<HDData::TextInputBoxUI>()->GetTextUI()->SetSortOrder(0.77f);

	// privateRoom Set
	HDData::GameObject* privateRoomTextLabel = API::CreateTextbox(_scene, "privateRoomTextLabel", setRoomCanvas);
	privateRoomTextLabel->GetTransform()->SetPosition(710.0f, 320.0f, 0.f);
	privateRoomTextLabel->GetComponent<HDData::TextUI>()->SetText("privateRoom");
	privateRoomTextLabel->GetComponent<HDData::TextUI>()->SetColor(DirectX::XMVectorSet(239.0f / 255.0f, 96.0f / 255.0f, 0.0f, 1.0f));
	privateRoomTextLabel->GetComponent<HDData::TextUI>()->SetSortOrder(0.75f);

	// privateCheckBox
	HDData::GameObject* privateCheckBox = API::CreateToggle(_scene, "privateCheckBox", setRoomCanvas);
	privateCheckBox->GetComponent<HDData::ToggleUI>()->GetOnComp()->SetImage("checkbox_background.png");
	privateCheckBox->GetComponent<HDData::ToggleUI>()->GetOffComp()->SetImage("checkbox_cross.png");
	privateCheckBox->GetTransform()->SetPosition(1200.0f, 240.0f, 0.0f);
	privateCheckBox->GetComponent<HDData::ToggleUI>()->SetSortOrder(0.75f);

	HDData::GameObject* roomPassWordTextBox = API::CreateTextInputBox(_scene, "roomPassWord", setRoomCanvas);
	roomPassWordTextBox->GetTransform()->SetPosition(960.0f, 320.0f, 0.0f);
	auto newRoomPassWord = roomPassWordTextBox->GetComponent<HDData::TextInputBoxUI>();
	roomPassWordTextBox->GetComponent<HDData::TextInputBoxUI>()->GetBackgroundImage()->SetSortOrder(0.75f);
	roomPassWordTextBox->GetComponent<HDData::TextInputBoxUI>()->GetCursorImage()->SetSortOrder(0.75f);
	roomPassWordTextBox->GetComponent<HDData::TextInputBoxUI>()->GetTextUI()->SetSortOrder(0.75f);

	// room setting
	HDData::GameObject* roomSetBtn = API::CreateButton(_scene, "roomSet", setRoomCanvas);
	roomSetBtn->GetTransform()->SetPosition(960.0f, 840.0f, 0.f);
	roomSetBtn->GetComponent<HDData::Button>()->SetImage("AlphaBtn.png");
	roomSetBtn->GetComponent<HDData::Button>()->SetSortOrder(0.8f);
	roomSetBtn->AddComponent<BtnScript>();

	HDData::GameObject* setRoomText = API::CreateTextbox(_scene, "setRoomText", roomSetBtn);
	setRoomText->GetTransform()->SetPosition(roomSetBtn->GetTransform()->GetPosition());
	setRoomText->GetComponent<HDData::TextUI>()->SetFont("Resources/Font/KRAFTON_40.spriteFont");
	setRoomText->GetComponent<HDData::TextUI>()->SetText("MAKE ROOM");
	setRoomText->GetComponent<HDData::TextUI>()->SetSortOrder(0.8f);

	// Training Btn
	HDData::GameObject* trainingBtn = API::CreateButton(_scene, "TestingBtn", mainControlObject);
	trainingBtn->GetTransform()->SetPosition(130.0f, 400.0f, 0.f);
	trainingBtn->GetComponent<HDData::Button>()->SetImage("AlphaBtn.png");
	trainingBtn->GetComponent<HDData::Button>()->SetSortOrder(0.6f);
	trainingBtn->AddComponent<BtnScript>();
	HDData::GameObject* trainingText = API::CreateTextbox(_scene, "trainingRoom", trainingBtn);
	trainingText->GetTransform()->SetPosition(trainingBtn->GetTransform()->GetPosition());
	trainingText->GetComponent<HDData::TextUI>()->SetFont("Resources/Font/KRAFTON_40.spriteFont");
	trainingText->GetComponent<HDData::TextUI>()->SetText("TRAINING");

	//temp Btn
	HDData::GameObject* tempBtn = API::CreateButton(_scene, "TestingBtn", mainControlObject);
	tempBtn->GetTransform()->SetPosition(150.0f, 355.0f, 0.f);
	tempBtn->GetComponent<HDData::Button>()->SetImage("AlphaBtn.png");
	tempBtn->GetComponent<HDData::Button>()->SetSortOrder(0.6f);
	tempBtn->AddComponent<BtnScript>();
	HDData::GameObject* tempText = API::CreateTextbox(_scene, "tempText", tempBtn);
	tempText->GetTransform()->SetPosition(tempBtn->GetTransform()->GetPosition());
	tempText->GetComponent<HDData::TextUI>()->SetFont("Resources/Font/KRAFTON_40.spriteFont");
	tempText->GetComponent<HDData::TextUI>()->SetText("FADE");

	// Exit button
	//HDData::GameObject* exitBtn = API::CreateButton(_scene, "extiBtn", mainControlObject);
	//exitBtn->GetTransform()->SetPosition(100.f, 370.0f, 0.6f); // y += 160
	//exitBtn->GetComponent<HDData::Button>()->SetImage("AlphaBtn.png");
	//exitBtn->AddComponent<BtnScript>();
	//HDData::GameObject* exitText = API::CreateTextbox(_scene, "exitText", exitBtn);
	//exitText->GetTransform()->SetPosition(exitBtn->GetTransform()->GetPosition());
	//exitText->GetComponent<HDData::TextUI>()->SetFont("Resources/Font/KRAFTON_40.spriteFont");
	//exitText->GetComponent<HDData::TextUI>()->SetDefaultColor(DirectX::Colors::DarkRed);
	//exitText->GetComponent<HDData::TextUI>()->SetText("EXIT");

	// setting & option
	HDData::GameObject* preferencesBtn = API::CreateButton(_scene, "preferencesBtn");
	preferencesBtn->GetTransform()->SetPosition(1875.f, 30.f, 0.f);
	preferencesBtn->AddComponent<BtnHoveringScript>();
	preferencesBtn->GetComponent<HDData::Button>()->SetImage("icon_cog.png");
	preferencesBtn->GetComponent<HDData::Button>()->SetSortOrder(0.6f);
	// ex)해상도, BGM 볼륨, 환경볼륨, 마우스 감도

	HDData::GameObject* settingControlObject = API::CreateImageBox(_scene, "settingControlObject");
	settingControlObject->GetTransform()->SetPosition(-500.0f, -500.0f, 0.0f);
	settingControlObject->SetSelfActive(false);

	HDData::GameObject* preferencesCanvas = API::CreateImageBox(_scene, "displaySetting", settingControlObject);
	preferencesCanvas->GetTransform()->SetPosition(960.f, 540.f, 0.f);
	preferencesCanvas->GetComponent<HDData::ImageUI>()->SetImage("alphaRefCanvas2.png");
	preferencesCanvas->GetComponent<HDData::ImageUI>()->SetSortOrder(0.6f);
	preferencesCanvas->GetComponent<HDData::ImageUI>()->SetIsIgnoreFocus(true);

	// event
	// game play btn
	playBtn->GetComponent<HDData::Button>()->SetOnClickEvent(
		[=]()
		{
			if (!roomMakeBtn->GetSelfActive() && !roomEnterBtn->GetSelfActive())
			{
				roomMakeBtn->SetSelfActive(true);
				roomEnterBtn->SetSelfActive(true);
			}
			else
			{
				roomMakeBtn->SetSelfActive(false);
				roomEnterBtn->SetSelfActive(false);
				if (setRoomCanvas->GetSelfActive() && roomListCanvas->GetSelfActive())
				{
					setRoomCanvas->SetSelfActive(false);
					roomListCanvas->SetSelfActive(false);
				}
			}
		}
	);

	roomEnterBtn->GetComponent<HDData::Button>()->SetOnClickEvent
	(
		[=]()
		{
			MenuManager::Instance().GetRoomList();

			if (!roomListCanvas->GetSelfActive())
			{
				MenuManager::Instance().ShowRoomListCanvas(true);
			}
			else
			{
				MenuManager::Instance().ShowRoomListCanvas(false);
			}

			if (setRoomCanvas->GetSelfActive())
			{
				setRoomCanvas->SetSelfActive(false);
			}
		}
	);

	privateCheckBox->GetComponent<HDData::ToggleUI>()->SetToggleOnEvent(
		[=]()
		{
			roomPassWordTextBox->GetComponent<HDData::TextInputBoxUI>()->GetBackgroundImage()->SetImage("back.png");
			roomPassWordTextBox->GetComponent<HDData::TextInputBoxUI>()->GetBackgroundImage()->SetIsIgnoreFocus(false);
		});

	privateCheckBox->GetComponent<HDData::ToggleUI>()->SetToggleOffEvent(
		[=]()
		{
			roomPassWordTextBox->GetComponent<HDData::TextInputBoxUI>()->GetBackgroundImage()->SetImage("back_NotActive.png");
			roomPassWordTextBox->GetComponent<HDData::TextInputBoxUI>()->GetBackgroundImage()->SetIsIgnoreFocus(true);
		});

	roomMakeBtn->GetComponent<HDData::Button>()->SetOnClickEvent(
		[=]()
		{
			if (!setRoomCanvas->GetSelfActive())
			{
				setRoomCanvas->SetSelfActive(true);
			}
			else
			{
				setRoomCanvas->SetSelfActive(false);
			}

			if (roomListCanvas->GetSelfActive())
			{
				roomListCanvas->SetSelfActive(false);
			}
		}
	);

	roomSetBtn->GetComponent<HDData::Button>()->SetOnClickEvent
	(
		[=]()
		{
			//Protocol::RoomInfo info;
			//info.set_roomname(roomNameTextbox->GetComponent<HDData::TextInputBoxUI>()->GetCurrentText());
			//info.set_isprivate(privateCheckBox->GetComponent<HDData::ToggleUI>()->GetIsOn());

			//if (info.isprivate())
			//{
			//	info.set_password(roomPassWordTextBox->GetComponent<HDData::TextInputBoxUI>()->GetCurrentText());
			//}

			//NetworkManager::Instance().SetRoom(info);
		}
	);

	// game setting
	preferencesBtn->GetComponent<HDData::Button>()->SetOnClickEvent(
		[=]()
		{
			if (mainControlObject->GetSelfActive())
			{
				mainControlObject->SetSelfActive(false);
				settingControlObject->SetSelfActive(true);
			}
			else
			{
				mainControlObject->SetSelfActive(true);
				settingControlObject->SetSelfActive(false);
			}
		}
	);

	trainingBtn->GetComponent<HDData::Button>()->SetOnClickEvent(
		[=]()
		{
			trainingText->GetComponent<HDData::TextUI>()->SetColor(DirectX::Colors::Aqua);
		}
	);

	tempBtn->GetComponent<HDData::Button>()->SetOnClickEvent(
		[=]()
		{
			if (FadeInOut::Instance().GetComplete())
			{
				FadeInOut::Instance().FadeOut();
			}
			else
			{
				FadeInOut::Instance().FadeIn();
			}
		}
	);

}
