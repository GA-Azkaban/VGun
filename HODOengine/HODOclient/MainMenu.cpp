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
	_menuManager.Instance().SetMainMenuCanvas(mainmenuCanvas);

	HDData::GameObject* temp = API::CreateImageBox(_scene, "tempCanvas");
	temp->GetComponent<HDData::ImageUI>()->SetImage("test.jpg");
	temp->GetComponent<HDData::ImageUI>()->SetSortOrder(0.0f);
	temp->GetTransform()->SetPosition(960.f, 540.f, 0.f);

	HDData::GameObject* temp2 = API::CreateButton(_scene, "tempBtn",mainmenuCanvas);
	temp2->GetTransform()->SetPosition(200.f, 240.f, 0.f);
	//temp2->GetComponent<HDData::Button>()->SetImage("test.jpg");

	//HDData::GameObject* playBtn = API::CreateButton(_scene, "playBtn", mainmenuCanvas);

	HDData::GameObject* preferencesBtn = API::CreateButton(_scene, "preferencesBtn", mainmenuCanvas);
	preferencesBtn->GetTransform()->SetPosition(0.f, 500.f, 0.f);
	//preferencesBtn->GetComponent<HDData::Button>()->SetImage("abcd.jpg");

}

