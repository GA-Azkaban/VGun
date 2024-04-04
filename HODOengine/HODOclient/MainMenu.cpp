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


	HDData::GameObject* temp2 = API::CreateButton(_scene, "tempBtn",mainmenuCanvas);
	temp2->GetTransform()->SetPosition(200.f, 240.f, 0.6f);
	//temp2->GetComponent<HDData::Button>()->SetImage("test.jpg");

	//HDData::GameObject* playBtn = API::CreateButton(_scene, "playBtn", mainmenuCanvas);

	HDData::GameObject* preferencesBtn = API::CreateButton(_scene, "preferencesBtn", mainmenuCanvas);
	preferencesBtn->GetTransform()->SetPosition(0.f, 500.f, 0.f);
	//preferencesBtn->GetComponent<HDData::Button>()->SetImage("abcd.jpg");
	preferencesBtn->GetComponent<HDData::Button>()->SetSortOrder(0.6f);
	preferencesBtn->GetComponent<HDData::Button>()->SetOnClickEvent(
		[preferencesBtn]()
		{
			preferencesBtn->GetComponent<HDData::Button>()->FadeOut();
		}
	);

}

