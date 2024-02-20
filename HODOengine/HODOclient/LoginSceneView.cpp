#include "LoginSceneView.h"
#include "LobbyManager.h"
#include "NetworkManager.h"

LoginSceneView::LoginSceneView()
	: _lobbyManager(LobbyManager::Instance()),
	_networkManager(NetworkManager::Instance())
{

}

LoginSceneView::~LoginSceneView()
{

}

void LoginSceneView::Initialize()
{
	// Create Scene
	_scene = API::CreateScene("MainLobby");


	LoginView();

	API::LoadScene(_scene);
}

void LoginSceneView::LoginView()
{
	auto skybox = API::CreateObject(_scene);
	auto skyboxComp = skybox->AddComponent<HDData::CubeMapRenderer>();
	skyboxComp->LoadCubeMapTexture("sunsetcube1024.dds");

	/// main canvas
	
	// 임시 동작을 위한 큐브맵 나중에 지워야 함
	//auto skybox = API::CreateObject(_scene);
	//auto skyboxComp = skybox->AddComponent<HDData::CubeMapRenderer>();
	//skyboxComp->LoadCubeMapTexture("sunsetcube1024.dds");

	// 여기서부터 로비 UI
	HDData::GameObject* mainCanvas = API::CreateImageBox(_scene, "mainCanvas");
	mainCanvas->GetComponent<HDData::ImageUI>()->SetImage("white.png");
	mainCanvas->GetTransform()->SetPosition(960.f, 540.f, 0.f);

	_lobbyManager.SetLobbyMainCanvas(mainCanvas);

	// id input box
	HDData::GameObject* idTextbox = API::CreateTextInputBox(_scene, "idTextBox", mainCanvas);
	auto id = idTextbox->GetComponent<HDData::TextInputBoxUI>();
	idTextbox->GetTransform()->SetLocalPosition(0.f, -100.f, 0.f);
	HDData::GameObject* idTextLabel = API::CreateTextbox(_scene, "idTextLabel", idTextbox);
	idTextLabel->GetTransform()->SetLocalPosition(-250.f, 0.f, 0.f);
	idTextLabel->GetComponent<HDData::TextUI>()->SetText("ID");
	idTextLabel->GetComponent<HDData::TextUI>()->SetColor(DirectX::Colors::Black);

	// password input box
	HDData::GameObject* passwordTextbox = API::CreateTextInputBox(_scene, "passwordTextBox", mainCanvas);
	auto pw = passwordTextbox->GetComponent<HDData::TextInputBoxUI>();
	HDData::GameObject* passwordTextboxLabel = API::CreateTextbox(_scene, "passwordTextboxLabel", passwordTextbox);
	passwordTextboxLabel->GetTransform()->SetLocalPosition(-250.f, 0.f, 0.f);
	passwordTextboxLabel->GetComponent<HDData::TextUI>()->SetText("PASSWORD");
	passwordTextboxLabel->GetComponent<HDData::TextUI>()->SetColor(DirectX::Colors::Black);

	// login button
	HDData::GameObject* loginBtn = API::CreateButton(_scene, "loginBtn", mainCanvas);
	loginBtn->GetTransform()->SetPosition(870.f, 650.f, 0.f);
	loginBtn->GetComponent<HDData::Button>()->SetImage("login.png");
	loginBtn->GetComponent<HDData::Button>()->SetOnClickEvent(
		[id, pw]()
		{
			std::string ID = id->GetCurrentText();
			std::string password = pw->GetCurrentText();
			LobbyManager::Instance().Login(ID, password);
		}
	);

	// join Button
	HDData::GameObject* joinBtn = API::CreateButton(_scene, "joinBtn", mainCanvas);
	joinBtn->GetTransform()->SetPosition(1050.f, 650.f, 0.f);
	joinBtn->GetComponent<HDData::Button>()->SetImage("join.png");
	joinBtn->GetComponent<HDData::Button>()->SetOnClickEvent(
		[]()
		{
			LobbyManager::Instance().Join();
		}
	);

	/// join canvas

	HDData::GameObject* joinCanvas = API::CreateImageBox(_scene, "joinCanvas");
	joinCanvas->GetComponent<HDData::ImageUI>()->SetImage("green.png");
	joinCanvas->GetTransform()->SetPosition(960.f, 540.f, 0.f);

	_lobbyManager.SetJoinCanvas(joinCanvas);

	HDData::GameObject* newIDtextbox = API::CreateTextInputBox(_scene, "newIDtextbox", joinCanvas);
	newIDtextbox->GetTransform()->SetLocalPosition(0.f, -200.f, 0.f);
	auto newIDtext = newIDtextbox->GetComponent<HDData::TextInputBoxUI>();
	HDData::GameObject* newIDLabel = API::CreateTextbox(_scene, "newIDLabel", newIDtextbox);
	newIDLabel->GetTransform()->SetLocalPosition(-200.f, 0.f, 0.f);
	newIDLabel->GetComponent<HDData::TextUI>()->SetText("ID");
	newIDLabel->GetComponent<HDData::TextUI>()->SetColor(DirectX::Colors::White);

	HDData::GameObject* newPasswordTextbox = API::CreateTextInputBox(_scene, "newPasswordTextbox", joinCanvas);
	newPasswordTextbox->GetTransform()->SetLocalPosition(0.f, -100.f, 0.f);
	auto newPWtext = newPasswordTextbox->GetComponent<HDData::TextInputBoxUI>();
	HDData::GameObject* newPasswordLabel = API::CreateTextbox(_scene, "newPasswordLabel", newPasswordTextbox);
	newPasswordLabel->GetTransform()->SetLocalPosition(-200.f, 0.f, 0.f);
	newPasswordLabel->GetComponent<HDData::TextUI>()->SetText("Password");
	newPasswordLabel->GetComponent<HDData::TextUI>()->SetColor(DirectX::Colors::White);

	HDData::GameObject* newNicknameTextbox = API::CreateTextInputBox(_scene, "newNicknameTextbox", joinCanvas);
	newNicknameTextbox->GetTransform()->SetLocalPosition(0.f, 0.f, 0.f);
	auto newNNtext = newNicknameTextbox->GetComponent<HDData::TextInputBoxUI>();
	HDData::GameObject* newNicknameLabel = API::CreateTextbox(_scene, "newNicknameLabel", newNicknameTextbox);
	newNicknameLabel->GetTransform()->SetLocalPosition(-200.f, 0.f, 0.f);
	newNicknameLabel->GetComponent<HDData::TextUI>()->SetText("Nickname");
	newNicknameLabel->GetComponent<HDData::TextUI>()->SetColor(DirectX::Colors::White);

	HDData::GameObject* makeAccountBtn = API::CreateButton(_scene, "makeAccountBtn", joinCanvas);
	
	makeAccountBtn->GetComponent<HDData::Button>()->SetImage("addNewAccount.png");
	makeAccountBtn->GetTransform()->SetLocalPosition(-150.f, 100.f, 0.f);
	makeAccountBtn->GetComponent<HDData::Button>()->SetOnClickEvent(
		[=]()
		{
			if (!newIDtext->GetCurrentText().empty() && !newPWtext->GetCurrentText().empty() && !newNNtext->GetCurrentText().empty())
			{
				LobbyManager::Instance().MakeNewAccount(newIDtext->GetCurrentText(), newPWtext->GetCurrentText(), newNNtext->GetCurrentText());
			}
		}
	);

	HDData::GameObject* exitJoinBtn = API::CreateButton(_scene, "exitJoinBtn", joinCanvas);
	exitJoinBtn->GetComponent<HDData::Button>()->SetSortOrder(1);
	exitJoinBtn->GetComponent<HDData::Button>()->SetImage("exitJoin.png");
	exitJoinBtn->GetTransform()->SetLocalPosition(150.f, 100.f, 0.f);
	exitJoinBtn->GetComponent<HDData::Button>()->SetOnClickEvent(
		[=]()
		{
			LobbyManager::Instance().ExitJoin();
		}
	);

	joinCanvas->OnDisable();
}

void LoginSceneView::MainMenu()
{

}

void LoginSceneView::Lobby()
{

}
