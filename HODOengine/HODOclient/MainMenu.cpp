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
	mainmenuCanvas->GetComponent<HDData::ImageUI>()->SetImage("white.png");
	mainmenuCanvas->GetComponent<HDData::ImageUI>()->SetSortOrder(0.0f);
	mainmenuCanvas->GetTransform()->SetPosition(960.f, 540.f, 0.f);
	mainmenuCanvas->GetComponent < HDData::ImageUI>()->SetActive(false);
	_menuManager.Instance().SetMainMenuCanvas(mainmenuCanvas);

	// fade option canvas
	// 페이드 아웃용 검은색 캔버스는 0.5f 1에 가까울수록 UI 낮을수록 배경
	HDData::GameObject* fadeCanvas = API::CreateImageBox(_scene, "fadeCanvas");
	fadeCanvas->GetComponent<HDData::ImageUI>()->SetImage("black.png");
	fadeCanvas->GetComponent<HDData::ImageUI>()->SetSortOrder(0.9f);
	fadeCanvas->GetTransform()->SetPosition(960.0f, 540.f, 0.f);
	fadeCanvas->GetComponent<HDData::ImageUI>()->FadeOut();
	fadeCanvas->GetComponent<HDData::ImageUI>()->SetIsIgnoreFocus(true);

	HDData::GameObject* temp = API::CreateImageBox(_scene, "tempCanvas");
	temp->GetComponent<HDData::ImageUI>()->SetImage("test.jpg");
	temp->GetComponent<HDData::ImageUI>()->SetSortOrder(0.0f);
	temp->GetTransform()->SetPosition(960.f, 540.f, 0.f);

	// play->RoomEnter sequence
	HDData::GameObject* playBtn = API::CreateButton(_scene, "playBtn",mainmenuCanvas);
	playBtn->GetComponent<HDData::Button>()->SetImage("play_btn.png");
	playBtn->GetTransform()->SetPosition(115.f, 240.f, 0.6f);
	playBtn->GetComponent<HDData::Button>()->SetSortOrder(0.6f);

	HDData::GameObject* roomEnterBtn = API::CreateButton(_scene, "roomEnter", playBtn);
	roomEnterBtn->GetTransform()->SetPosition(350.f, 190.f,0.6f);
	roomEnterBtn->GetComponent<HDData::Button>()->SetImage("arrowLeft.png");
	roomEnterBtn->GetComponent<HDData::Button>()->FadeOut();

	HDData::GameObject* roomMakeBtn = API::CreateButton(_scene, "roomEnter", playBtn);
	roomMakeBtn->GetTransform()->SetPosition(350.f, 290.f, 0.6f);
	roomMakeBtn->GetComponent<HDData::Button>()->SetImage("arrowRight.png");
	roomMakeBtn->GetComponent<HDData::Button>()->FadeOut();

	playBtn->GetComponent<HDData::Button>()->SetOnClickEvent(
		[=]()
		{
			roomEnterBtn->GetComponent<HDData::Button>()->FadeIn();
			roomMakeBtn->GetComponent<HDData::Button>()->FadeIn(); 
		}
	);

	playBtn->GetComponent<HDData::Button>()->SetOnClickEvent(
		[=]()
		{
			roomEnterBtn->GetComponent<HDData::Button>()->FadeOut();
			roomMakeBtn->GetComponent<HDData::Button>()->FadeOut();
		}
	);

	// setting & option
	HDData::GameObject* preferencesBtn = API::CreateButton(_scene, "preferencesBtn", mainmenuCanvas);
	preferencesBtn->GetTransform()->SetPosition(115.f, 500.f, 0.f);
	preferencesBtn->GetComponent<HDData::Button>()->SetImage("setting_btn.png");
	preferencesBtn->GetComponent<HDData::Button>()->SetSortOrder(0.6f);
	preferencesBtn->GetComponent<HDData::Button>()->SetOnClickEvent(
		[=]()
		{
			preferencesBtn->GetComponent<HDData::Button>()->FadeOut();
		}
	);

}

