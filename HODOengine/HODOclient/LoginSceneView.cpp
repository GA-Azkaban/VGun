#include "LoginSceneView.h"
#include "LobbyManager.h"
#include "FadeInOut.h"
#include "NetworkManager.h"

#include "../HODOengine/AudioSource.h"
#include "BtnScript.h"

LoginSceneView::LoginSceneView()
	: _lobbyManager(LobbyManager::Instance()),
	_fadeInOut(FadeInOut::Instance())
{

}

LoginSceneView::~LoginSceneView()
{

}

void LoginSceneView::Initialize()
{
	// Create Scene
	_scene = API::CreateScene("Login");

	LoginView();

	//API::LoadScene(_scene);
	//API::LoadScene(menu);
}

void LoginSceneView::LoginView()
{
	// test ID
	// testingAcc
	// 1234

	HDData::GameObject* mainCanvas = API::CreateImageBox(_scene, "mainmenuCanvas");
	mainCanvas->GetComponent<HDData::ImageUI>()->SetImage("_blur_background_image.png");
	mainCanvas->GetComponent<HDData::ImageUI>()->SetSortOrder(0.0f);
	mainCanvas->GetTransform()->SetPosition(960.f, 540.f, 0.f);
	mainCanvas->GetComponent < HDData::ImageUI>()->SetActive(true);

	auto skybox = API::CreateObject(_scene);
	auto skyboxComp = skybox->AddComponent<HDData::CubeMapRenderer>();
	skyboxComp->LoadCubeMapTexture("sunsetcube1024.dds");

	/// main canvas

	// 임시 동작을 위한 큐브맵 나중에 지워야 함
	//auto skybox = API::CreateObject(_scene);
	//auto skyboxComp = skybox->AddComponent<HDData::CubeMapRenderer>();
	//skyboxComp->LoadCubeMapTexture("sunsetcube1024.dds");

	// 여기서부터 로비 UI
	//HDData::GameObject* mainCanvas = API::CreateImageBox(_scene, "mainCanvas");
	//mainCanvas->GetComponent<HDData::ImageUI>()->SetImage("white.png");
	//mainCanvas->GetComponent<HDData::ImageUI>()->SetSortOrder(0.0f);
	//mainCanvas->GetTransform()->SetPosition(960.f, 540.f, 0.f);

	// fade option canvas
	// 페이드 아웃용 검은색 캔버스는 0.5f 1에 가까울수록 UI 낮을수록 배경
	//HDData::GameObject* loginfadeCanvas = API::CreateImageBox(_scene, "fadeCanvas");
	//loginfadeCanvas->GetComponent<HDData::ImageUI>()->SetImage("black.png");
	//loginfadeCanvas->GetComponent<HDData::ImageUI>()->SetSortOrder(0.9f);
	//loginfadeCanvas->GetTransform()->SetPosition(960.0f, 540.f, 0.f);
	//loginfadeCanvas->GetComponent<HDData::ImageUI>()->FadeOut();
	//loginfadeCanvas->GetComponent<HDData::ImageUI>()->SetIsIgnoreFocus(true);

	// fade option canvas
	//HDData::GameObject* fadeCanvas = API::CreateImageBox(_scene, "blackCanvas");
	//fadeCanvas->GetComponent<HDData::ImageUI>()->SetImage("black.png");
	//fadeCanvas->GetComponent<HDData::ImageUI>()->SetSortOrder(0.9f);
	//fadeCanvas->GetTransform()->SetPosition(960.0f, 540.f, 0.f);
	//fadeCanvas->GetComponent<HDData::ImageUI>()->FadeOut();
	//fadeCanvas->GetComponent<HDData::ImageUI>()->SetIsIgnoreFocus(true);
	//_lobbyManager.SetFadeCanvas(fadeCanvas);

	// fadeIn Test
	FadeInOut::Instance().FadeOut();

	// login Control Object
	HDData::GameObject* loginControlObject = API::CreateImageBox(_scene, "loginControlObject");
	loginControlObject->GetTransform()->SetPosition(-500.0f, -500.0f, 0.0f);
	loginControlObject->SetSelfActive(true);
	_lobbyManager.SetLobbyMainCanvas(loginControlObject);

	// id input box
	HDData::GameObject* idTextbox = API::CreateTextInputBox(_scene, "idTextBox", loginControlObject);
	auto id = idTextbox->GetComponent<HDData::TextInputBoxUI>();
	idTextbox->GetTransform()->SetPosition(960.0f, 440.f, 0.f);
	idTextbox->GetComponent<HDData::TextInputBoxUI>()->GetBackgroundImage()->SetSortOrder(0.1f);
	idTextbox->GetComponent<HDData::TextInputBoxUI>()->GetCursorImage()->SetSortOrder(0.11f);
	idTextbox->GetComponent<HDData::TextInputBoxUI>()->GetTextUI()->SetSortOrder(0.11f);

	HDData::GameObject* idTextLabel = API::CreateTextbox(_scene, "idTextLabel", loginControlObject);
	idTextLabel->GetTransform()->SetPosition(770.f, 440.f, 0.f);
	idTextLabel->GetComponent<HDData::TextUI>()->SetText("ID");
	idTextLabel->GetComponent<HDData::TextUI>()->SetColor(DirectX::XMVectorSet(239.0f / 255.0f, 96.0f / 255.0f, 0.0f, 1.0f));
	idTextLabel->GetComponent<HDData::TextUI>()->SetSortOrder(0.11f);

	// password input box
	HDData::GameObject* passwordTextbox = API::CreateTextInputBox(_scene, "passwordTextBox", loginControlObject);
	auto pw = passwordTextbox->GetComponent<HDData::TextInputBoxUI>();
	passwordTextbox->GetTransform()->SetPosition(960.0f, 540.f, 0.f);
	passwordTextbox->GetComponent<HDData::TextInputBoxUI>()->GetBackgroundImage()->SetSortOrder(0.1f);
	passwordTextbox->GetComponent<HDData::TextInputBoxUI>()->GetCursorImage()->SetSortOrder(0.11f);
	passwordTextbox->GetComponent<HDData::TextInputBoxUI>()->GetTextUI()->SetSortOrder(0.11f);

	HDData::GameObject* passwordTextboxLabel = API::CreateTextbox(_scene, "passwordTextboxLabel", loginControlObject);
	passwordTextboxLabel->GetTransform()->SetPosition(725.f, 540.f, 0.f);
	passwordTextboxLabel->GetComponent<HDData::TextUI>()->SetText("PASSWORD");
	passwordTextboxLabel->GetComponent<HDData::TextUI>()->SetColor(DirectX::XMVectorSet(239.0f / 255.0f, 96.0f / 255.0f, 0.0f, 1.0f));
	passwordTextboxLabel->GetComponent<HDData::TextUI>()->SetSortOrder(0.11f);

	// login button
	HDData::GameObject* loginBtn = API::CreateButton(_scene, "loginBtn");
	loginBtn->GetTransform()->SetPosition(865.f, 640.f, 0.f);
	loginBtn->GetComponent<HDData::Button>()->SetImage("AlphaBtn.png");
	loginBtn->GetComponent<HDData::Button>()->SetSortOrder(0.11f);
	loginBtn->AddComponent<BtnScript>();
	loginBtn->GetComponent<HDData::Button>()->SetOnClickEvent
	(
		[=]()
		{
			NetworkManager::Instance().SendAutoLogin();
			//std::string ID = id->GetCurrentText();
			//std::string password = pw->GetCurrentText();
			//LobbyManager::Instance().Login(ID, password);
		}
	);
	HDData::GameObject* loginText = API::CreateTextbox(_scene, "loginText", loginBtn);
	loginText->GetTransform()->SetPosition(loginBtn->GetTransform()->GetPosition());
	loginText->GetComponent<HDData::TextUI>()->SetFont("Resources/Font/KRAFTON_40.spriteFont");
	loginText->GetComponent<HDData::TextUI>()->SetDefaultColor(DirectX::Colors::OrangeRed);
	loginText->GetComponent<HDData::TextUI>()->SetText("LOGIN");

	// sign up control
	HDData::GameObject* joinControlObject = API::CreateImageBox(_scene, "joinControlObject");
	joinControlObject->GetTransform()->SetPosition(-500.0f, -500.0f, 0.0f);
	joinControlObject->SetSelfActive(false);
	_lobbyManager.SetJoinCanvas(joinControlObject);

	//signup Btn
	HDData::GameObject* joinBtn = API::CreateButton(_scene, "joinBtn",loginControlObject);
	joinBtn->GetTransform()->SetPosition(1055.f, 640.f, 0.f);
	joinBtn->GetComponent<HDData::Button>()->SetImage("125x45.png");
	joinBtn->GetComponent<HDData::Button>()->SetSortOrder(0.6f);
	joinBtn->SetSelfActive(true);
	joinBtn->AddComponent<BtnScript>();
	HDData::GameObject* joinText = API::CreateTextbox(_scene, "joinText", joinBtn);
	joinText->GetTransform()->SetPosition(joinBtn->GetTransform()->GetPosition());
	joinText->GetComponent<HDData::TextUI>()->SetFont("Resources/Font/KRAFTON_40.spriteFont");
	joinText->GetComponent<HDData::TextUI>()->SetDefaultColor(DirectX::Colors::OrangeRed);
	joinText->GetComponent<HDData::TextUI>()->SetText("SIGN");

	// join canvas
	HDData::GameObject* joinCanvas = API::CreateImageBox(_scene, "joinCanvas", joinControlObject);
	joinCanvas->GetComponent<HDData::ImageUI>()->SetImage("joinCanvas.png");
	joinCanvas->GetTransform()->SetScale(500.0f, 500.0f, 0.0f);
	joinCanvas->GetComponent<HDData::ImageUI>()->SetSortOrder(0.15f);
	joinCanvas->GetTransform()->SetPosition(960.f, 540.f, 0.f);

	_lobbyManager.SetJoinCanvas(joinCanvas);

	HDData::GameObject* newIDtextbox = API::CreateTextInputBox(_scene, "newIDtextbox", joinControlObject);
	newIDtextbox->GetTransform()->SetPosition(960.f, 340.f, 0.f);
	auto newIDtext = newIDtextbox->GetComponent<HDData::TextInputBoxUI>()->GetTextUI();
	newIDtextbox->GetComponent<HDData::TextInputBoxUI>()->GetBackgroundImage()->SetSortOrder(0.2f);
	newIDtextbox->GetComponent<HDData::TextInputBoxUI>()->GetCursorImage()->SetSortOrder(0.21f);
	newIDtextbox->GetComponent<HDData::TextInputBoxUI>()->GetTextUI()->SetSortOrder(0.21f);

	HDData::GameObject* newIDLabel = API::CreateTextbox(_scene, "newIDLabel", newIDtextbox);
	newIDLabel->GetTransform()->SetPosition(760.f, 340.f, 0.f);
	newIDLabel->GetComponent<HDData::TextUI>()->SetText("ID");
	newIDLabel->GetComponent<HDData::TextUI>()->SetColor(DirectX::XMVectorSet(239.0f / 255.0f, 96.0f / 255.0f, 0.0f, 1.0f));
	newIDLabel->GetComponent<HDData::TextUI>()->SetSortOrder(0.21f);

	HDData::GameObject* newPasswordTextbox = API::CreateTextInputBox(_scene, "newPasswordTextbox", joinControlObject);
	newPasswordTextbox->GetTransform()->SetPosition(960.f, 440.f, 0.f);
	auto newPWtext = newPasswordTextbox->GetComponent<HDData::TextInputBoxUI>()->GetTextUI();
	newPasswordTextbox->GetComponent<HDData::TextInputBoxUI>()->GetBackgroundImage()->SetSortOrder(0.2f);
	newPasswordTextbox->GetComponent<HDData::TextInputBoxUI>()->GetCursorImage()->SetSortOrder(0.21f);
	newPasswordTextbox->GetComponent<HDData::TextInputBoxUI>()->GetTextUI()->SetSortOrder(0.21f);

	HDData::GameObject* newPasswordLabel = API::CreateTextbox(_scene, "newPasswordLabel", newPasswordTextbox);
	newPasswordLabel->GetTransform()->SetPosition(740.f, 440.f, 0.f);
	newPasswordLabel->GetComponent<HDData::TextUI>()->SetText("Password");
	newPasswordLabel->GetComponent<HDData::TextUI>()->SetColor(DirectX::XMVectorSet(239.0f / 255.0f, 96.0f / 255.0f, 0.0f, 1.0f));
	newPasswordLabel->GetComponent<HDData::TextUI>()->SetSortOrder(0.21f);

	HDData::GameObject* newNicknameTextbox = API::CreateTextInputBox(_scene, "newNicknameTextbox", joinControlObject);
	newNicknameTextbox->GetTransform()->SetPosition(960.f, 540.f, 0.f);
	auto newNNtext = newNicknameTextbox->GetComponent<HDData::TextInputBoxUI>()->GetTextUI();
	newNicknameTextbox->GetComponent<HDData::TextInputBoxUI>()->GetBackgroundImage()->SetSortOrder(0.2f);
	newNicknameTextbox->GetComponent<HDData::TextInputBoxUI>()->GetCursorImage()->SetSortOrder(0.21f);
	newNicknameTextbox->GetComponent<HDData::TextInputBoxUI>()->GetTextUI()->SetSortOrder(0.21f);

	HDData::GameObject* newNicknameLabel = API::CreateTextbox(_scene, "newNicknameLabel", newNicknameTextbox);
	newNicknameLabel->GetTransform()->SetPosition(740.f, 540.f, 0.f);
	newNicknameLabel->GetComponent<HDData::TextUI>()->SetText("Nickname");
	newNicknameLabel->GetComponent<HDData::TextUI>()->SetColor(DirectX::XMVectorSet(239.0f / 255.0f, 96.0f / 255.0f, 0.0f, 1.0f));
	newNicknameLabel->GetComponent<HDData::TextUI>()->SetSortOrder(0.21f);

	HDData::GameObject* makeAccountBtn = API::CreateButton(_scene, "makeAccountBtn", joinControlObject);
	makeAccountBtn->GetComponent<HDData::Button>()->SetImage("125x45.png");
	makeAccountBtn->GetComponent<HDData::Button>()->SetSortOrder(0.21f);
	makeAccountBtn->GetTransform()->SetPosition(960.f, 640.f, 0.f);
	makeAccountBtn->AddComponent<BtnScript>();
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
	HDData::GameObject* makeAccountText = API::CreateTextbox(_scene, "makeAccountText", makeAccountBtn);
	makeAccountText->GetTransform()->SetPosition(makeAccountBtn->GetTransform()->GetPosition());
	makeAccountText->GetComponent<HDData::TextUI>()->SetFont("Resources/Font/KRAFTON_40.spriteFont");
	makeAccountText->GetComponent<HDData::TextUI>()->SetSortOrder(0.2f);
	makeAccountText->GetComponent<HDData::TextUI>()->SetDefaultColor(DirectX::Colors::OrangeRed);
	makeAccountText->GetComponent<HDData::TextUI>()->SetText("SIGN");

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
			//FadeInOut::Instance().FadeIn();
			API::LoadSceneByName("MainMenu");
		}
	);

	///login fail canvas
	HDData::GameObject* loginFail = API::CreateButton(_scene, "failImg");
	loginFail->GetComponent<HDData::Button>()->SetImage("Login_Fail.png");
	loginFail->GetComponent<HDData::Button>()->SetSortOrder(0.4f);
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
	idDupl->GetComponent<HDData::Button>()->SetSortOrder(0.4f);
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
	nameDupl->GetComponent<HDData::Button>()->SetSortOrder(0.4f);
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
	signupFail->GetComponent<HDData::Button>()->SetSortOrder(0.4f);
	signupFail->GetTransform()->SetPosition(960.f, 540.f, 0.f);
	signupFail->SetSelfActive(false);
	_lobbyManager.SetSignupFail(signupFail);
	signupFail->GetComponent<HDData::Button>()->SetOnClickEvent(
		[=]()
		{
			LobbyManager::Instance().showOff(nameDupl);
		}
	);

	HDData::GameObject* exitJoinBtn = API::CreateButton(_scene, "exitJoinBtn", joinControlObject);
	exitJoinBtn->GetComponent<HDData::Button>()->SetSortOrder(0.2f);
	exitJoinBtn->GetComponent<HDData::Button>()->SetImage("checkbox_cross.png");
	exitJoinBtn->GetTransform()->SetPosition(1300.f, 200.f, 0.f);
	exitJoinBtn->GetComponent<HDData::Button>()->SetOnClickEvent(
		[=]()
		{
			if (joinControlObject->GetSelfActive())
			{

				LobbyManager::Instance().showOff(joinControlObject);
				LobbyManager::Instance().showOn(loginControlObject);

			}
		}
	);

	joinBtn->GetComponent<HDData::Button>()->SetOnClickEvent(
		[=]()
		{
			/*LobbyManager::Instance().Join();*/
			if (loginControlObject->GetSelfActive())
			{
				LobbyManager::Instance().showOff(loginControlObject);
				LobbyManager::Instance().showOn(joinControlObject);
			}
		}
	);

}

