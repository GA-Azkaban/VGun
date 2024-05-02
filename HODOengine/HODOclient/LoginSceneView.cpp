#include "LoginSceneView.h"
#include "LobbyManager.h"

LoginSceneView::LoginSceneView()
	: _lobbyManager(LobbyManager::Instance())
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

	//API::LoadScene(_scene);
	//API::LoadScene(menu);
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
	mainCanvas->GetComponent<HDData::ImageUI>()->SetSortOrder(0.0f);
	mainCanvas->GetTransform()->SetPosition(960.f, 540.f, 0.f);

	_lobbyManager.SetLobbyMainCanvas(mainCanvas);

	// fade option canvas
	HDData::GameObject* fadeCanvas = API::CreateImageBox(_scene, "blackCanvas");
	fadeCanvas->GetComponent<HDData::ImageUI>()->SetImage("black.png");
	fadeCanvas->GetComponent<HDData::ImageUI>()->SetSortOrder(0.9f);
	fadeCanvas->GetTransform()->SetPosition(960.0f, 540.f, 0.f);
	fadeCanvas->GetComponent<HDData::ImageUI>()->FadeOut();
	fadeCanvas->GetComponent<HDData::ImageUI>()->SetIsIgnoreFocus(true);
	//_lobbyManager.SetFadeCanvas(fadeCanvas);

	// id input box
	HDData::GameObject* idTextbox = API::CreateTextInputBox(_scene, "idTextBox", mainCanvas);
	auto id = idTextbox->GetComponent<HDData::TextInputBoxUI>();
	idTextbox->GetTransform()->SetLocalPosition(0.f, -100.f, 0.f);
	idTextbox->GetComponent<HDData::TextInputBoxUI>()->GetBackgroundImage()->SetSortOrder(0.1f);
	idTextbox->GetComponent<HDData::TextInputBoxUI>()->GetCursorImage()->SetSortOrder(0.11f);
	idTextbox->GetComponent<HDData::TextInputBoxUI>()->GetTextUI()->SetSortOrder(0.11f);
	HDData::GameObject* idTextLabel = API::CreateTextbox(_scene, "idTextLabel", idTextbox);
	idTextLabel->GetTransform()->SetLocalPosition(-250.f, 0.f, 0.f);
	idTextLabel->GetComponent<HDData::TextUI>()->SetText("ID");
	idTextLabel->GetComponent<HDData::TextUI>()->SetColor(DirectX::Colors::Black);
	idTextLabel->GetComponent<HDData::TextUI>()->SetSortOrder(0.11f);

	// password input box
	HDData::GameObject* passwordTextbox = API::CreateTextInputBox(_scene, "passwordTextBox", mainCanvas);
	auto pw = passwordTextbox->GetComponent<HDData::TextInputBoxUI>();
	passwordTextbox->GetComponent<HDData::TextInputBoxUI>()->GetBackgroundImage()->SetSortOrder(0.1f);
	passwordTextbox->GetComponent<HDData::TextInputBoxUI>()->GetCursorImage()->SetSortOrder(0.11f);
	passwordTextbox->GetComponent<HDData::TextInputBoxUI>()->GetTextUI()->SetSortOrder(0.11f);
	HDData::GameObject* passwordTextboxLabel = API::CreateTextbox(_scene, "passwordTextboxLabel", passwordTextbox);
	passwordTextboxLabel->GetTransform()->SetLocalPosition(-250.f, 0.f, 0.f);
	passwordTextboxLabel->GetComponent<HDData::TextUI>()->SetText("PASSWORD");
	passwordTextboxLabel->GetComponent<HDData::TextUI>()->SetColor(DirectX::Colors::Black);
	passwordTextboxLabel->GetComponent<HDData::TextUI>()->SetSortOrder(0.11f);

	// login button
	HDData::GameObject* loginBtn = API::CreateButton(_scene, "loginBtn", mainCanvas);
	loginBtn->GetTransform()->SetPosition(870.f, 650.f, 0.f);
	loginBtn->GetComponent<HDData::Button>()->SetImage("login.png");
	loginBtn->GetComponent<HDData::Button>()->SetSortOrder(0.1f);
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
	joinBtn->GetComponent<HDData::Button>()->SetSortOrder(0.1f);
	joinBtn->GetComponent<HDData::Button>()->SetOnClickEvent(
		[]()
		{
			LobbyManager::Instance().Join();
		}
	);

	// join canvas
	HDData::GameObject* joinCanvas = API::CreateImageBox(_scene, "joinCanvas");
	joinCanvas->GetComponent<HDData::ImageUI>()->SetImage("green.png");
	joinCanvas->GetComponent<HDData::ImageUI>()->SetSortOrder(0.15f);
	joinCanvas->GetTransform()->SetPosition(960.f, 540.f, 0.f);

	_lobbyManager.SetJoinCanvas(joinCanvas);

	HDData::GameObject* newIDtextbox = API::CreateTextInputBox(_scene, "newIDtextbox", joinCanvas);
	newIDtextbox->GetTransform()->SetLocalPosition(0.f, -200.f, 0.f);
	auto newIDtext = newIDtextbox->GetComponent<HDData::TextInputBoxUI>()->GetTextUI();
	newIDtextbox->GetComponent<HDData::TextInputBoxUI>()->GetBackgroundImage()->SetSortOrder(0.2f);
	newIDtextbox->GetComponent<HDData::TextInputBoxUI>()->GetCursorImage()->SetSortOrder(0.21f);
	newIDtextbox->GetComponent<HDData::TextInputBoxUI>()->GetTextUI()->SetSortOrder(0.21f);
	
	HDData::GameObject* newIDLabel = API::CreateTextbox(_scene, "newIDLabel", newIDtextbox);
	newIDLabel->GetTransform()->SetLocalPosition(-200.f, 0.f, 0.f);
	newIDLabel->GetComponent<HDData::TextUI>()->SetText("ID");
	newIDLabel->GetComponent<HDData::TextUI>()->SetColor(DirectX::Colors::White);
	newIDLabel->GetComponent<HDData::TextUI>()->SetSortOrder(0.21f);

	HDData::GameObject* newPasswordTextbox = API::CreateTextInputBox(_scene, "newPasswordTextbox", joinCanvas);
	newPasswordTextbox->GetTransform()->SetLocalPosition(0.f, -100.f, 0.f);
	auto newPWtext = newPasswordTextbox->GetComponent<HDData::TextInputBoxUI>()->GetTextUI();
	newPasswordTextbox->GetComponent<HDData::TextInputBoxUI>()->GetBackgroundImage()->SetSortOrder(0.2f);
	newPasswordTextbox->GetComponent<HDData::TextInputBoxUI>()->GetCursorImage()->SetSortOrder(0.21f);
	newPasswordTextbox->GetComponent<HDData::TextInputBoxUI>()->GetTextUI()->SetSortOrder(0.21f);
	
	HDData::GameObject* newPasswordLabel = API::CreateTextbox(_scene, "newPasswordLabel", newPasswordTextbox);
	newPasswordLabel->GetTransform()->SetLocalPosition(-200.f, 0.f, 0.f);
	newPasswordLabel->GetComponent<HDData::TextUI>()->SetText("Password");
	newPasswordLabel->GetComponent<HDData::TextUI>()->SetColor(DirectX::Colors::White);
	newPasswordLabel->GetComponent<HDData::TextUI>()->SetSortOrder(0.21f);

	HDData::GameObject* newNicknameTextbox = API::CreateTextInputBox(_scene, "newNicknameTextbox", joinCanvas);
	newNicknameTextbox->GetTransform()->SetLocalPosition(0.f, 0.f, 0.f);
	auto newNNtext = newNicknameTextbox->GetComponent<HDData::TextInputBoxUI>()->GetTextUI();
	newNicknameTextbox->GetComponent<HDData::TextInputBoxUI>()->GetBackgroundImage()->SetSortOrder(0.2f);
	newNicknameTextbox->GetComponent<HDData::TextInputBoxUI>()->GetCursorImage()->SetSortOrder(0.21f);
	newNicknameTextbox->GetComponent<HDData::TextInputBoxUI>()->GetTextUI()->SetSortOrder(0.21f);
	
	HDData::GameObject* newNicknameLabel = API::CreateTextbox(_scene, "newNicknameLabel", newNicknameTextbox);
	newNicknameLabel->GetTransform()->SetLocalPosition(-200.f, 0.f, 0.f);
	newNicknameLabel->GetComponent<HDData::TextUI>()->SetText("Nickname");
	newNicknameLabel->GetComponent<HDData::TextUI>()->SetColor(DirectX::Colors::White);
	newNicknameLabel->GetComponent<HDData::TextUI>()->SetSortOrder(0.21f);

	HDData::GameObject* makeAccountBtn = API::CreateButton(_scene, "makeAccountBtn", joinCanvas);
	makeAccountBtn->GetComponent<HDData::Button>()->SetImage("addNewAccount.png");
	makeAccountBtn->GetComponent<HDData::Button>()->SetSortOrder(0.2f);
	makeAccountBtn->GetTransform()->SetLocalPosition(-150.f, 100.f, 0.f);
	makeAccountBtn->GetComponent<HDData::Button>()->SetOnClickEvent
	(
		[=]()
		{
			if (!newIDtext->GetText().empty() && !newPWtext->GetText().empty() && !newNNtext->GetText().empty())
			{
				LobbyManager::Instance().MakeNewAccount(newIDtext->GetText(), newPWtext->GetText(), newNNtext->GetText());
			}
		}
	);

	// 이부분을 어떻게 처리하지
	// login sucess canvas
	HDData::GameObject* loginSucess = API::CreateButton(_scene, "sucessImg");
	loginSucess->GetComponent<HDData::Button>()->SetImage("Login_Complete.png");
	loginSucess->GetComponent<HDData::Button>()->SetSortOrder(0.3f);
	loginSucess->GetTransform()->SetPosition(960.f, 540.f, 0.f);
	loginSucess->SetSelfActive(false);
	_lobbyManager.SetSucessCanvas(loginSucess);
	loginSucess->GetComponent<HDData::Button>()->SetOnClickEvent(
		[=]()
		{
			LobbyManager::Instance().showOff(loginSucess);
			// load main menu scene
		}
	);

	///login fail canvas
	HDData::GameObject* loginFail = API::CreateButton(_scene, "failImg");
	loginFail->GetComponent<HDData::Button>()->SetImage("Login_Fail.png");
	loginFail->GetComponent<HDData::Button>()->SetSortOrder(0.3f);
	loginFail->GetTransform()->SetPosition(960.f, 540.f, 0.f);
	loginFail->SetSelfActive(false);
	_lobbyManager.SetFailCanvas(loginFail);
	loginFail->GetComponent<HDData::Button>()->SetOnClickEvent(
		[=]()
		{
			LobbyManager::Instance().showOff(loginFail);
		}
	);

	// id duplication
	HDData::GameObject* idDupl = API::CreateButton(_scene, "idDupl");
	idDupl->GetComponent<HDData::Button>()->SetImage("ID_Duplication.png");
	idDupl->GetComponent<HDData::Button>()->SetSortOrder(0.3f);
	idDupl->GetTransform()->SetPosition(960.f, 540.f, 0.f);
	idDupl->SetSelfActive(false);
	_lobbyManager.SetidDupl(idDupl);
	idDupl->GetComponent<HDData::Button>()->SetOnClickEvent(
		[=]()
		{
			LobbyManager::Instance().showOff(idDupl);
		}
	);

	// name duplication
	HDData::GameObject* nameDupl = API::CreateButton(_scene, "nameDupl");
	nameDupl->GetComponent<HDData::Button>()->SetImage("name_Duplication.png");
	nameDupl->GetComponent<HDData::Button>()->SetSortOrder(0.3f);
	nameDupl->GetTransform()->SetPosition(960.f, 540.f, 0.f);
	nameDupl->SetSelfActive(false);
	_lobbyManager.SetnameDule(nameDupl);
	nameDupl->GetComponent<HDData::Button>()->SetOnClickEvent(
		[=]()
		{
			LobbyManager::Instance().showOff(nameDupl);
		}
	);

	// signup fail
	HDData::GameObject* signupFail = API::CreateButton(_scene, "signup");
	signupFail->GetComponent<HDData::Button>()->SetImage("Signup_Fail.png");
	signupFail->GetComponent<HDData::Button>()->SetSortOrder(0.3f);
	signupFail->GetTransform()->SetPosition(960.f, 540.f, 0.f);
	signupFail->SetSelfActive(false);
	_lobbyManager.SetSignupFail(signupFail);
	signupFail->GetComponent<HDData::Button>()->SetOnClickEvent(
		[=]()
		{
			LobbyManager::Instance().showOff(nameDupl);
		}
	);

	HDData::GameObject* exitJoinBtn = API::CreateButton(_scene, "exitJoinBtn", joinCanvas);
	exitJoinBtn->GetComponent<HDData::Button>()->SetSortOrder(0.2f);
	exitJoinBtn->GetComponent<HDData::Button>()->SetImage("exitJoin.png");
	exitJoinBtn->GetTransform()->SetLocalPosition(150.f, 100.f, 0.f);
	exitJoinBtn->GetComponent<HDData::Button>()->SetOnClickEvent(
		[=]()
		{
			LobbyManager::Instance().ExitJoin();
		}
	);

	HDData::GameObject* test = API::CreateButton(_scene, "test");
	test->GetComponent<HDData::Button>()->SetSortOrder(0.3f);
	test->GetComponent<HDData::Button>()->SetImage("defaultImg.png");
	test->GetTransform()->SetPosition(10, 10, 0.f);
	test->GetComponent<HDData::Button>()->SetOnClickEvent(
		[=]()
		{
			test->GetTransform()->Translate(100.f, 0, 0);
			LobbyManager::Instance().Test();
		}
	);

	joinCanvas->SetSelfActive(false);
}

