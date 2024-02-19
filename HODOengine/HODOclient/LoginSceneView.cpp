#include "LoginSceneView.h"
#include "LobbyManager.h"
#include "NetworkManager.h"

LoginSceneView::LoginSceneView()
	: _lobbyManager(LobbyManager::Instance())
{
	_networkManager = API::CreateStaticObject();
	_networkManager->AddComponent<NetworkManager>();
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

	HDData::GameObject* mainCanvas = API::CreateImageBox(_scene, "mainCanvas");
	mainCanvas->GetComponent<HDData::ImageUI>()->SetImage("white.png");
	mainCanvas->GetTransform()->SetPosition(960.f, 540.f, 0.f);

	_lobbyManager.SetLobbyMainCanvas(mainCanvas);

	// id input box
	HDData::GameObject* idTextbox = API::CreateTextInputBox(_scene, "idTextBox", mainCanvas);
	idTextbox->GetTransform()->SetLocalPosition(0.f, -100.f, 0.f);
	HDData::GameObject* idTextLabel = API::CreateTextbox(_scene, "idTextLabel", idTextbox);
	idTextLabel->GetTransform()->SetLocalPosition(-250.f, 0.f, 0.f);
	idTextLabel->GetComponent<HDData::TextUI>()->SetText("ID");
	idTextLabel->GetComponent<HDData::TextUI>()->SetColor(DirectX::Colors::White);

	// password input box
	HDData::GameObject* passwordTextbox = API::CreateTextInputBox(_scene, "passwordTextBox", mainCanvas);
	HDData::GameObject* passwordTextboxLabel = API::CreateTextbox(_scene, "passwordTextboxLabel", passwordTextbox);
	passwordTextboxLabel->GetTransform()->SetLocalPosition(-250.f, 0.f, 0.f);
	passwordTextboxLabel->GetComponent<HDData::TextUI>()->SetText("PASSWORD");
	passwordTextboxLabel->GetComponent<HDData::TextUI>()->SetColor(DirectX::Colors::White);

	// login button
	HDData::GameObject* loginBtn = API::CreateButton(_scene, "loginBtn", mainCanvas);
	loginBtn->GetTransform()->SetPosition(870.f, 650.f, 0.f);
	loginBtn->GetComponent<HDData::Button>()->SetImage("login.png");
	loginBtn->GetComponent<HDData::Button>()->SetOnClickEvent(
		[this, &idTextbox, &passwordTextbox]()
		{
			std::string id = idTextbox->GetComponent<HDData::TextInputBoxUI>()->GetCurrentText();
			std::string password = passwordTextbox->GetComponent<HDData::TextInputBoxUI>()->GetCurrentText();
			_lobbyManager.Login(id, password);
		}
	);

	// join Button
	HDData::GameObject* joinBtn = API::CreateButton(_scene, "joinBtn", mainCanvas);
	joinBtn->GetTransform()->SetPosition(1050.f, 650.f, 0.f);
	joinBtn->GetComponent<HDData::Button>()->SetImage("join.png");
	joinBtn->GetComponent<HDData::Button>()->SetOnClickEvent(
		[this, &idTextbox, &passwordTextbox]()
		{
			_lobbyManager.Join();
		}
	);

	/// join canvas

	HDData::GameObject* joinCanvas = API::CreateImageBox(_scene, "joinCanvas");
	joinCanvas->GetComponent<HDData::ImageUI>()->SetImage("green.png");
	joinCanvas->GetTransform()->SetPosition(960.f, 540.f, 0.f);

	//_lobbyManager.SetJoinCanvas(joinCanvas);

	HDData::GameObject* newIDtextbox = API::CreateTextInputBox(_scene, "newIDtextbox", joinCanvas);
	newIDtextbox->GetTransform()->SetPosition(0.f, -200.f, 0.f);
	auto newIDtext = newIDtextbox->GetComponent<HDData::TextUI>();
	HDData::GameObject* newIDLabel = API::CreateTextbox(_scene, "newIDLabel", newIDtextbox);
	newIDLabel->GetTransform()->SetLocalPosition(-200.f, 0.f, 0.f);
	newIDLabel->GetComponent<HDData::TextUI>()->SetText("ID");
	newIDLabel->GetComponent<HDData::TextUI>()->SetColor(DirectX::Colors::White);

	HDData::GameObject* newPasswordTextbox = API::CreateTextInputBox(_scene, "newPasswordTextbox", joinCanvas);
	newPasswordTextbox->GetTransform()->SetPosition(0.f, -100.f, 0.f);
	auto newPWtext = newPasswordTextbox->GetComponent<HDData::TextUI>();
	HDData::GameObject* newPasswordLabel = API::CreateTextbox(_scene, "newPasswordLabel", newPasswordTextbox);
	newPasswordLabel->GetTransform()->SetLocalPosition(-200.f, 0.f, 0.f);
	newPasswordLabel->GetComponent<HDData::TextUI>()->SetText("Password");
	//newPasswordLabel->GetComponent<HDData::TextUI>()->SetColor(DirectX::Colors::Black);

	HDData::GameObject* newNicknameTextbox = API::CreateTextInputBox(_scene, "newNicknameTextbox", joinCanvas);
	newNicknameTextbox->GetTransform()->SetPosition(0.f, 0.f, 0.f);
	auto newNNtext = newNicknameTextbox->GetComponent<HDData::TextUI>();
	HDData::GameObject* newNicknameLabel = API::CreateTextbox(_scene, "newNicknameLabel", newNicknameTextbox);
	newNicknameLabel->GetTransform()->SetLocalPosition(-200.f, 0.f, 0.f);
	newNicknameLabel->GetComponent<HDData::TextUI>()->SetText("Nickname");

	// error msg
	HDData::GameObject* errorMsg = API::CreateImageBox(_scene, "errorImg", joinCanvas);
	errorMsg->GetComponent<HDData::ImageUI>()->SetImage("errorMsg.png");
	HDData::GameObject* exitErrorBtn = API::CreateButton(_scene, "exitErrorBtn", errorMsg);
	exitErrorBtn->GetTransform()->SetPosition(960.f, 650.f, 0.f);

	errorMsg->OnDisable();

	exitErrorBtn->GetComponent<HDData::Button>()->SetOnClickEvent(
		[&errorMsg]()
		{
			errorMsg->OnDisable();
		}
	);

	HDData::GameObject* makeAccountBtn = API::CreateButton(_scene, "makeAccountBtn", joinCanvas);
	//makeAccountBtn->GetComponent<HDData::ImageUI>()->SetImage("addNewAccount.png");
	makeAccountBtn->GetComponent<HDData::Button>()->SetOnClickEvent(
		[&newIDtext, &newPWtext, &newNNtext, &errorMsg]()
		{
			if (newIDtext->GetText().empty() || newPWtext->GetText().empty() || newNNtext->GetText().empty())
			{
				errorMsg->OnEnable();
			}
		}
	);

	HDData::GameObject* exitJoinBtn = API::CreateButton(_scene, "exitJoinBtn", joinCanvas);
	//exitJoinBtn->GetComponent<HDData::ImageUI>()->SetImage("exitJoin.png");
	exitJoinBtn->GetComponent<HDData::Button>()->SetOnClickEvent(
		[this]()
		{
			_lobbyManager.ExitJoin();
		}
	);

	mainCanvas->OnDisable();
	joinCanvas->OnDisable();
}

void LoginSceneView::MainMenu()
{

}

void LoginSceneView::Lobby()
{

}
