#include "MainMenu.h"
#include "MenuManager.h"

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

	//HDData::GameObject* lobby = API::CreateImageBox(_scene, "tempCanvas");
	//lobby->GetComponent<HDData::ImageUI>()->SetImage("test.jpg");
	//lobby->GetComponent<HDData::ImageUI>()->SetSortOrder(0.0f);
	//lobby->GetTransform()->SetPosition(960.f, 540.f, 0.f);

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

	HDData::GameObject* setRoomCanvas = API::CreateImageBox(_scene, "lobbyCanvas", playBtn);
	setRoomCanvas->GetComponent<HDData::ImageUI>()->SetImage("green.png");
	setRoomCanvas->GetTransform()->SetPosition(960.0f, 540.0f, 0.f);
	setRoomCanvas->GetComponent<HDData::ImageUI>()->SetSortOrder(0.57f);
	setRoomCanvas->SetSelfActive(false);

	HDData::GameObject* roomListCanvas = API::CreateImageBox(_scene, "roomListCanvas", setRoomCanvas);
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
	
	// event
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

	preferencesBtn->GetComponent<HDData::Button>()->SetOnClickEvent(
		[=]()
		{
			if (mainControlObject->GetSelfActive())
			{
				mainControlObject->SetSelfActive(false);
				//playBtn->SetSelfActive(false);
				//trainingRoomBtn->SetSelfActive(false);
				//exitBtn->SetSelfActive(false);
			}
			else
			{
				mainControlObject->SetSelfActive(true);
				//playBtn->SetSelfActive(true);
				//trainingRoomBtn->SetSelfActive(true);
				//exitBtn->SetSelfActive(true);
			}
		}
	);
}

