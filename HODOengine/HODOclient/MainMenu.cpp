#include "MainMenu.h"
#include "MenuManager.h"
#include "SoundSystem.h"

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

}

void MainMenuScene::MainMenu()
{

	HDData::GameObject* mainmenuCanvas = API::CreateImageBox(_scene, "mainmenuCanvas");
	mainmenuCanvas->GetComponent<HDData::ImageUI>()->SetImage("_blur_background_image.png");
	//mainmenuCanvas->GetComponent<HDData::ImageUI>()->SetImage("white.png");
	mainmenuCanvas->GetComponent<HDData::ImageUI>()->SetSortOrder(0.0f);
	mainmenuCanvas->GetTransform()->SetPosition(960.f, 540.f, 0.f);
	mainmenuCanvas->GetComponent < HDData::ImageUI>()->SetActive(true);
	_menuManager.Instance().SetMainMenuCanvas(mainmenuCanvas);

	HDData::GameObject* mainControlObject = API::CreateImageBox(_scene,"controlObject");
	mainControlObject->GetTransform()->SetPosition(-500.0f, -500.0f, 0.0f);
	mainControlObject->SetSelfActive(true);

	// fade option canvas
	// 페이드 아웃용 검은색 캔버스는 0.5f 1에 가까울수록 UI 낮을수록 배경
	HDData::GameObject* fadeCanvas = API::CreateImageBox(_scene, "fadeCanvas");
	fadeCanvas->GetComponent<HDData::ImageUI>()->SetImage("black.png");
	fadeCanvas->GetComponent<HDData::ImageUI>()->SetSortOrder(0.9f);
	fadeCanvas->GetTransform()->SetPosition(960.0f, 540.f, 0.f);
	fadeCanvas->GetComponent<HDData::ImageUI>()->FadeOut();
	fadeCanvas->GetComponent<HDData::ImageUI>()->SetIsIgnoreFocus(true);

	// play->RoomEnter & make sequence
	HDData::GameObject* playBtn = API::CreateButton(_scene, "playBtn", mainControlObject);
	playBtn->GetComponent<HDData::Button>()->SetImage("play_btn.png");
	playBtn->GetTransform()->SetPosition(130.f, 240.f, 0.6f);
	playBtn->GetComponent<HDData::Button>()->SetSortOrder(0.6f);

	HDData::GameObject* roomEnterBtn = API::CreateButton(_scene, "roomEnter", playBtn);
	roomEnterBtn->GetTransform()->SetPosition(365.f, 190.f, 0.55f);
	roomEnterBtn->GetComponent<HDData::Button>()->SetImage("login.png");
	roomEnterBtn->SetSelfActive(false);

	HDData::GameObject* roomMakeBtn = API::CreateButton(_scene, "roomMake", playBtn);
	roomMakeBtn->GetTransform()->SetPosition(365.f, 290.f, 0.6f);
	roomMakeBtn->GetComponent<HDData::Button>()->SetImage("addNewAccount.png");
	roomMakeBtn->GetComponent<HDData::Button>()->SetSortOrder(0.56f);
	roomMakeBtn->SetSelfActive(false);

	// room setting
	HDData::GameObject* setRoomCanvas = API::CreateImageBox(_scene, "lobbyCanvas", roomMakeBtn);
	setRoomCanvas->GetComponent<HDData::ImageUI>()->SetImage("green.png");
	setRoomCanvas->GetTransform()->SetPosition(960.0f, 540.0f, 0.f);
	setRoomCanvas->GetComponent<HDData::ImageUI>()->SetSortOrder(0.57f);
	setRoomCanvas->SetSelfActive(false);

	// room name Set
	HDData::GameObject* roomNameTextbox = API::CreateTextInputBox(_scene, "roomNameTextBox", setRoomCanvas);
	roomNameTextbox->GetTransform()->SetPosition(960.0f, 240.0f,0.0f);
	auto newRoomName = roomNameTextbox->GetComponent<HDData::TextInputBoxUI>()->GetTextUI();
	roomNameTextbox->GetComponent<HDData::TextInputBoxUI>()->GetBackgroundImage()->SetSortOrder(0.6f);
	roomNameTextbox->GetComponent<HDData::TextInputBoxUI>()->GetCursorImage()->SetSortOrder(0.61f);
	roomNameTextbox->GetComponent<HDData::TextInputBoxUI>()->GetTextUI()->SetSortOrder(0.61f);

	// privateRoom Set
	HDData::GameObject* privateCheckBox = API::CreateButton(_scene, "privateCheckBox", setRoomCanvas);
	privateCheckBox->GetComponent<HDData::Button>()->SetImage("checkbox_background.png");
	privateCheckBox->GetTransform()->SetPosition(750.0f, 320.0f, 0.0f);
	privateCheckBox->GetComponent<HDData::Button>()->SetSortOrder(0.61f);

	HDData::GameObject* privateCheck = API::CreateImageBox(_scene,"privateCheck",setRoomCanvas);
	privateCheck->GetComponent<HDData::ImageUI>()->SetImage("checkbox_cross.png");
	privateCheck->GetTransform()->SetPosition(privateCheckBox->GetTransform()->GetPosition());
	privateCheck->GetComponent<HDData::ImageUI>()->SetSortOrder(0.62f);
	privateCheck->GetComponent<HDData::ImageUI>()->SetIsIgnoreFocus(true);
	privateCheck->SetSelfActive(false);

	HDData::GameObject* roomSetBtn = API::CreateButton(_scene, "roomSet", setRoomCanvas);
	roomSetBtn->GetTransform()->SetPosition(960.0f, 840.0f, 0.f);
	roomSetBtn->GetComponent<HDData::Button>()->SetImage("addNewAccount.png");
	roomSetBtn->GetComponent<HDData::Button>()->SetSortOrder(0.6f);

	HDData::GameObject* roomPassWord = API::CreateTextInputBox(_scene, "roomPassWord", setRoomCanvas);
	roomPassWord->GetTransform()->SetPosition(960.0f, 240.0f, 0.0f);
	auto newRoomPassWord = roomPassWord->GetComponent<HDData::TextInputBoxUI>()->GetTextUI();
	roomPassWord->GetComponent<HDData::TextInputBoxUI>()->GetBackgroundImage()->SetSortOrder(0.6f);
	roomPassWord->GetComponent<HDData::TextInputBoxUI>()->GetCursorImage()->SetSortOrder(0.61f);
	roomPassWord->GetComponent<HDData::TextInputBoxUI>()->GetTextUI()->SetSortOrder(0.61f);

	// find room
	HDData::GameObject* roomListCanvas = API::CreateImageBox(_scene, "roomListCanvas", roomEnterBtn);
	roomListCanvas->GetTransform()->SetPosition(960.0f, 900.0f, 0.0f);
	roomListCanvas->GetComponent<HDData::ImageUI>()->SetImage("_shadow_gradient.png");

	// Training room -> scene change
	HDData::GameObject* trainingRoomBtn = API::CreateButton(_scene,"trainingRoomBtn", mainControlObject);
	trainingRoomBtn->GetTransform()->SetPosition(130.f, 400.f, 0.f);
	trainingRoomBtn->GetComponent<HDData::Button>()->SetImage("training_room.png");

	// Exit button
	HDData::GameObject* exitBtn = API::CreateButton(_scene, "trainingRoomBtn", mainControlObject);
	exitBtn->GetTransform()->SetPosition(130.f, 560, 0.6f); // y += 160
	exitBtn->GetComponent<HDData::Button>()->SetImage("exit_btn.png");

	// setting & option
	HDData::GameObject* preferencesBtn = API::CreateButton(_scene, "preferencesBtn");
	preferencesBtn->GetTransform()->SetPosition(1875.f, 30.f, 0.f);
	preferencesBtn->GetComponent<HDData::Button>()->SetImage("icon_cog.png");
	preferencesBtn->GetComponent<HDData::Button>()->SetSortOrder(0.6f);
	
	HDData::GameObject* settingControlObject = API::CreateImageBox(_scene, "settingControlObject");
	settingControlObject->GetTransform()->SetPosition(-500.0f, -500.0f, 0.0f);
	settingControlObject->SetSelfActive(false);

	HDData::GameObject* displaySetting = API::CreateImageBox(_scene, "displaySetting", settingControlObject);
	displaySetting->GetTransform()->SetPosition(363.f, 354.f, 0.f);
	displaySetting->GetComponent<HDData::ImageUI>()->SetImage("_card_holder.png");
	displaySetting->GetComponent<HDData::ImageUI>()->SetSortOrder(0.6f);
	displaySetting->GetComponent<HDData::ImageUI>()->SetIsIgnoreFocus(true);

	HDData::GameObject* audioSetting = API::CreateImageBox(_scene, "audioSetting", settingControlObject);
	audioSetting->GetTransform()->SetPosition(763.f, 354.f, 0.f);
	audioSetting->GetComponent<HDData::ImageUI>()->SetImage("_card_holder.png");
	audioSetting->GetComponent<HDData::ImageUI>()->SetSortOrder(0.6f);
	audioSetting->GetComponent<HDData::ImageUI>()->SetIsIgnoreFocus(true);

	HDData::GameObject* gameSetting = API::CreateImageBox(_scene, "gameSetting", settingControlObject);
	gameSetting->GetTransform()->SetPosition(1163.f, 354.f, 0.f);
	gameSetting->GetComponent<HDData::ImageUI>()->SetImage("_card_holder.png");
	gameSetting->GetComponent<HDData::ImageUI>()->SetSortOrder(0.6f);
	gameSetting->GetComponent<HDData::ImageUI>()->SetIsIgnoreFocus(true);

	HDData::GameObject* operationSetting = API::CreateImageBox(_scene, "operationSetting", settingControlObject);
	operationSetting->GetTransform()->SetPosition(1563.f, 354.f, 0.f);
	operationSetting->GetComponent<HDData::ImageUI>()->SetImage("_card_holder.png");
	operationSetting->GetComponent<HDData::ImageUI>()->SetSortOrder(0.6f);
	operationSetting->GetComponent<HDData::ImageUI>()->SetIsIgnoreFocus(true);

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
				if (setRoomCanvas->GetSelfActive())
				{
					setRoomCanvas->SetSelfActive(false);
				}
			}
		}
	);

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
		}
	);

	privateCheckBox->GetComponent<HDData::Button>()->SetOnClickEvent(
		[=]()
		{
			if (!privateCheck->GetSelfActive())
			{
				privateCheck->SetSelfActive(true);
			}
			else
			{
				privateCheck->SetSelfActive(false);
			}
		}
	);

	roomSetBtn->GetComponent<HDData::Button>()->SetOnClickEvent
	(
		[=]()
		{
			MenuManager::Instance().SetRoom();
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


}

