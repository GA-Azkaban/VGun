#include "LoginSceneView.h"
#include "LobbyManager.h"
#include "NetworkManager.h"

#include "../HODOengine/AudioSource.h"
#include "BtnTextScript.h"
#include "BtnImageScript.h"
#include "BtnHoveringScript.h"

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
	_scene = API::CreateScene("Login");

	LoginView();
}

void LoginSceneView::LoginView()
{
	UINT width = API::GetScreenWidth();
	UINT height = API::GetScreenHeight();

	int originalwidth = 4000;
	int originalHeight = 2248;

	int targetWidth = 2560;
	int targetHeight = 1440;

	float widthScale = static_cast<float>(targetWidth) / originalwidth;
	float heightScale = static_cast<float>(targetHeight) / originalHeight;
	float scale = (widthScale < heightScale) ? widthScale : heightScale;

	/// main canvas
	HDData::GameObject* mainCanvas = API::CreateImageBox(_scene, "loginCanvas");
	mainCanvas->GetTransform()->SetPosition(960.f * width / 1920, 540.f * height / 1080, 0.f);
	auto mainCanvasImage = mainCanvas->GetComponent<HDData::ImageUI>();
	mainCanvasImage->SetIsIgnoreFocus(true);
	mainCanvasImage->SetImage("_blur_background_image.png");
	mainCanvasImage->ChangeScale(scale, scale);
	mainCanvasImage->SetSortOrder(0.1f);
	mainCanvasImage->SetActive(true);

	// login Control Object
	HDData::GameObject* loginControlObject = API::CreateImageBox(_scene, "loginControlObject");
	loginControlObject->GetTransform()->SetPosition(-500.0f * width / 1920, -500.0f * height / 1080, 0.0f);
	loginControlObject->SetSelfActive(true);
	//loginControlObject->GetComponent<HDData::ImageUI>()->ChangeScale(static_cast<float>(width) / 1920, static_cast<float>(height) / 1080);
	_lobbyManager.SetLobbyMainCanvas(loginControlObject);

	// gameLogo
	auto gameLogo = API::CreateImageBox(_scene, "gameLogo", loginControlObject);
	gameLogo->GetTransform()->SetPosition(API::GetScreenWidth() / 2, 600.0f, 0.0f);
	auto gameLogoComp = gameLogo->GetComponent<HDData::ImageUI>();
	gameLogoComp->SetSortOrder(0.12f);
	gameLogoComp->SetIsIgnoreFocus(true);
	gameLogoComp->SetImage("gameLogo.png");

	// TeamLogo
	auto teamLogo = API::CreateImageBox(_scene, "teamLogo");
	teamLogo->GetTransform()->SetPosition(150.0f,1300.0f,0.0f);
	auto teamLogoComp = teamLogo->GetComponent<HDData::ImageUI>();
	teamLogoComp->SetSortOrder(0.1f);
	teamLogoComp->SetImage("teamLogo.png");
	teamLogoComp->ChangeScale(0.3f,0.3f);

	// id input box
	HDData::GameObject* idTextbox = API::CreateTextInputBox(_scene, "idTextBox", loginControlObject);
	auto id = idTextbox->GetComponent<HDData::TextInputBoxUI>();
	idTextbox->GetTransform()->SetPosition(950.0f * width / 1920, (440.f * height / 1080) + 600, 0.f);
	idTextbox->GetComponent<HDData::TextInputBoxUI>()->GetBackgroundImage()->SetSortOrder(0.2f);
	idTextbox->GetComponent<HDData::TextInputBoxUI>()->GetCursorImage()->SetSortOrder(0.21f);
	idTextbox->GetComponent<HDData::TextInputBoxUI>()->GetTextUI()->SetSortOrder(0.21f);

	HDData::GameObject* idTextLabel = API::CreateTextbox(_scene, "idTextLabel", loginControlObject);
	idTextLabel->GetTransform()->SetPosition(900.f * width / 1920, (440.f * height / 1080) + 500, 0.f);
	idTextLabel->GetComponent<HDData::TextUI>()->SetText("NICKNAME");
	idTextLabel->GetComponent<HDData::TextUI>()->SetFont("Resources/Font/KRAFTON_40.spriteFont");
	idTextLabel->GetComponent<HDData::TextUI>()->SetColor(DirectX::Colors::Red);
	idTextLabel->GetComponent<HDData::TextUI>()->SetSortOrder(0.21f);

	// password input box
	HDData::GameObject* passwordTextbox = API::CreateTextInputBox(_scene, "passwordTextBox", loginControlObject);
	passwordTextbox->SetSelfActive(false);
	auto pw = passwordTextbox->GetComponent<HDData::TextInputBoxUI>();
	passwordTextbox->GetTransform()->SetPosition(960.0f * width / 1920, (540.f * height / 1080) + 500, 0.f);
	passwordTextbox->GetComponent<HDData::TextInputBoxUI>()->GetBackgroundImage()->SetSortOrder(0.2f);
	passwordTextbox->GetComponent<HDData::TextInputBoxUI>()->GetCursorImage()->SetSortOrder(0.21f);
	passwordTextbox->GetComponent<HDData::TextInputBoxUI>()->GetTextUI()->SetSortOrder(0.21f);

	HDData::GameObject* passwordTextboxLabel = API::CreateTextbox(_scene, "passwordTextboxLabel", loginControlObject);
	passwordTextboxLabel->SetSelfActive(false);
	passwordTextboxLabel->GetTransform()->SetPosition(725.f * width / 1920, (540.f * height / 1080) + 500, 0.f);
	passwordTextboxLabel->GetComponent<HDData::TextUI>()->SetText("PASSWORD");
	passwordTextboxLabel->GetComponent<HDData::TextUI>()->SetFont("Resources/Font/KRAFTON_25.spriteFont");
	passwordTextboxLabel->GetComponent<HDData::TextUI>()->SetColor(DirectX::Colors::Red);
	//passwordTextboxLabel->GetComponent<HDData::TextUI>()->SetColor({ 163 / 255.0f, 29 / 255.0f, 17 / 255.0f,1.0f });
	passwordTextboxLabel->GetComponent<HDData::TextUI>()->SetSortOrder(0.21f);

	// login button
	HDData::GameObject* loginBtn = API::CreateButton(_scene, "loginBtn", loginControlObject);
	loginBtn->GetTransform()->SetPosition((865.f * width / 1920)+120.0f, (640.f * height / 1080) + 500, 0.f);
	loginBtn->GetComponent<HDData::Button>()->SetImage("AlphaBtn.png");
	loginBtn->GetComponent<HDData::Button>()->SetSortOrder(0.65f);
	loginBtn->AddComponent<BtnTextScript>();
	loginBtn->GetComponent<HDData::Button>()->SetOnClickEvent
	(
		[=]()
		{
			NetworkManager::Instance().SendAutoLogin(id->GetTextUI()->GetText());
			id->SetOrigin();
			//if ((id->GetCurrentText() == "") && (pw->GetCurrentText() == ""))
			//{
			//	NetworkManager::Instance().SendAutoLogin();
			//}
			//else
			//{
			//	std::string ID = id->GetCurrentText();
			//	std::string password = pw->GetCurrentText();
			//	LobbyManager::Instance().Login(ID, password);
			//}
		}
	);
	HDData::GameObject* loginText = API::CreateTextbox(_scene, "loginText", loginBtn);
	loginText->GetTransform()->SetPosition(loginBtn->GetTransform()->GetPosition());
	auto logintxt = loginText->GetComponent<HDData::TextUI>();
	logintxt->SetFont("Resources/Font/KRAFTON_40.spriteFont");
	logintxt->SetDefaultColor(DirectX::Colors::Red);
	logintxt->SetSortOrder(0.65f);
	logintxt->SetText("LOGIN");

	// sign up control
	HDData::GameObject* joinControlObject = API::CreateImageBox(_scene, "joinControlObject");
	joinControlObject->GetTransform()->SetPosition(-500.0f * width / 1920, -500.0f * height / 1080, 0.0f);
	joinControlObject->SetSelfActive(false);
	_lobbyManager.SetJoinCanvas(joinControlObject);

	// join Btn
	HDData::GameObject* joinBtn = API::CreateButton(_scene, "joinBtn", loginControlObject);
	joinBtn->GetTransform()->SetPosition((1055.f * width / 1920)+200.0f, (640.f * height / 1080) + 500, 0.f);
	joinBtn->GetComponent<HDData::Button>()->SetImage("125x45.png");
	joinBtn->GetComponent<HDData::Button>()->SetSortOrder(0.6f);
	joinBtn->SetSelfActive(true);
	joinBtn->AddComponent<BtnTextScript>();
	HDData::GameObject* joinText = API::CreateTextbox(_scene, "joinText", joinBtn);
	joinText->GetTransform()->SetPosition(joinBtn->GetTransform()->GetPosition());
	joinText->GetComponent<HDData::TextUI>()->SetFont("Resources/Font/KRAFTON_40.spriteFont");
	joinText->GetComponent<HDData::TextUI>()->SetDefaultColor(DirectX::Colors::Red);
	joinText->GetComponent<HDData::TextUI>()->SetText("SIGN");

	// join canvas
	HDData::GameObject* joinCanvas = API::CreateImageBox(_scene, "joinCanvas", joinControlObject);
	joinCanvas->GetComponent<HDData::ImageUI>()->SetImage("joinCanvas.png");
	joinCanvas->GetTransform()->SetScale(500.0f, 500.0f, 0.0f);
	joinCanvas->GetComponent<HDData::ImageUI>()->SetSortOrder(0.15f);
	joinCanvas->GetTransform()->SetPosition(960.f * width / 1920, 540.f * height / 1080, 0.f);

	_lobbyManager.SetJoinCanvas(joinCanvas);

	HDData::GameObject* newIDtextbox = API::CreateTextInputBox(_scene, "newIDtextbox", joinControlObject);
	newIDtextbox->GetTransform()->SetPosition(960.f * width / 1920, 340.f * height / 1080, 0.f);
	auto newIDtext = newIDtextbox->GetComponent<HDData::TextInputBoxUI>()->GetTextUI();
	newIDtextbox->GetComponent<HDData::TextInputBoxUI>()->GetBackgroundImage()->SetSortOrder(0.2f);
	newIDtextbox->GetComponent<HDData::TextInputBoxUI>()->GetCursorImage()->SetSortOrder(0.21f);
	newIDtextbox->GetComponent<HDData::TextInputBoxUI>()->GetTextUI()->SetSortOrder(0.21f);

	HDData::GameObject* newIDLabel = API::CreateTextbox(_scene, "newIDLabel", newIDtextbox);
	newIDLabel->GetTransform()->SetPosition(760.f * width / 1920, 340.f * height / 1080, 0.f);
	newIDLabel->GetComponent<HDData::TextUI>()->SetText("ID");
	newIDLabel->GetComponent<HDData::TextUI>()->SetFont("Resources/Font/KRAFTON_25.spriteFont");
	newIDLabel->GetComponent<HDData::TextUI>()->SetColor(DirectX::XMVectorSet(239.0f / 255.0f, 96.0f / 255.0f, 0.0f, 1.0f));
	newIDLabel->GetComponent<HDData::TextUI>()->SetSortOrder(0.21f);

	HDData::GameObject* newPasswordTextbox = API::CreateTextInputBox(_scene, "newPasswordTextbox", joinControlObject);
	newPasswordTextbox->GetTransform()->SetPosition(960.f * width / 1920, 440.f * height / 1080, 0.f);
	auto newPWtext = newPasswordTextbox->GetComponent<HDData::TextInputBoxUI>()->GetTextUI();
	newPasswordTextbox->GetComponent<HDData::TextInputBoxUI>()->GetBackgroundImage()->SetSortOrder(0.2f);
	newPasswordTextbox->GetComponent<HDData::TextInputBoxUI>()->GetCursorImage()->SetSortOrder(0.21f);
	newPasswordTextbox->GetComponent<HDData::TextInputBoxUI>()->GetTextUI()->SetSortOrder(0.21f);

	HDData::GameObject* newPasswordLabel = API::CreateTextbox(_scene, "newPasswordLabel", newPasswordTextbox);
	newPasswordLabel->GetTransform()->SetPosition(740.f * width / 1920, 440.f * height / 1080, 0.f);
	newPasswordLabel->GetComponent<HDData::TextUI>()->SetText("Password");
	newPasswordLabel->GetComponent<HDData::TextUI>()->SetFont("Resources/Font/KRAFTON_25.spriteFont");
	newPasswordLabel->GetComponent<HDData::TextUI>()->SetColor(DirectX::XMVectorSet(239.0f / 255.0f, 96.0f / 255.0f, 0.0f, 1.0f));
	newPasswordLabel->GetComponent<HDData::TextUI>()->SetSortOrder(0.21f);

	HDData::GameObject* newNicknameTextbox = API::CreateTextInputBox(_scene, "newNicknameTextbox", joinControlObject);
	newNicknameTextbox->GetTransform()->SetPosition(960.f * width / 1920, 540.f * height / 1080, 0.f);
	auto newNNtext = newNicknameTextbox->GetComponent<HDData::TextInputBoxUI>()->GetTextUI();
	newNicknameTextbox->GetComponent<HDData::TextInputBoxUI>()->GetBackgroundImage()->SetSortOrder(0.2f);
	newNicknameTextbox->GetComponent<HDData::TextInputBoxUI>()->GetCursorImage()->SetSortOrder(0.21f);
	newNicknameTextbox->GetComponent<HDData::TextInputBoxUI>()->GetTextUI()->SetSortOrder(0.21f);

	HDData::GameObject* newNicknameLabel = API::CreateTextbox(_scene, "newNicknameLabel", newNicknameTextbox);
	newNicknameLabel->GetTransform()->SetPosition(740.f * width / 1920, 540.f * height / 1080, 0.f);
	newNicknameLabel->GetComponent<HDData::TextUI>()->SetText("Nickname");
	newNicknameLabel->GetComponent<HDData::TextUI>()->SetFont("Resources/Font/KRAFTON_25.spriteFont");
	newNicknameLabel->GetComponent<HDData::TextUI>()->SetColor(DirectX::XMVectorSet(239.0f / 255.0f, 96.0f / 255.0f, 0.0f, 1.0f));
	newNicknameLabel->GetComponent<HDData::TextUI>()->SetSortOrder(0.21f);

	HDData::GameObject* makeAccountBtn = API::CreateButton(_scene, "makeAccountBtn", joinControlObject);
	makeAccountBtn->GetComponent<HDData::Button>()->SetImage("125x45.png");
	makeAccountBtn->GetComponent<HDData::Button>()->SetSortOrder(0.21f);
	makeAccountBtn->GetTransform()->SetPosition(960.f * width / 1920, 640.f * height / 1080, 0.f);
	makeAccountBtn->AddComponent<BtnTextScript>();
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
	makeAccountText->GetComponent<HDData::TextUI>()->SetDefaultColor(DirectX::Colors::Red);
	makeAccountText->GetComponent<HDData::TextUI>()->SetText("SIGN");

	// 이부분을 어떻게 처리하지
	// login sucess canvas
	HDData::GameObject* loginSucess = API::CreateButton(_scene, "sucessImg");
	loginSucess->GetComponent<HDData::Button>()->SetImage("Login_Complete.png");
	loginSucess->GetComponent<HDData::Button>()->SetSortOrder(0.3f);
	loginSucess->GetTransform()->SetPosition(960.f * width / 1920, 540.f * height / 1080, 0.f);
	loginSucess->SetSelfActive(false);
	_lobbyManager.SetSucessCanvas(loginSucess);
	loginSucess->GetComponent<HDData::Button>()->SetOnClickEvent(
		[=]()
		{
			LobbyManager::Instance().showOff(loginSucess);
		}
	);

	///login fail canvas
	HDData::GameObject* loginFail = API::CreateButton(_scene, "failImg");
	loginFail->GetComponent<HDData::Button>()->SetImage("Login_Fail.png");
	loginFail->GetComponent<HDData::Button>()->SetSortOrder(0.4f);
	loginFail->GetTransform()->SetPosition(960.f * width / 1920, 540.f * height / 1080, 0.f);
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
	idDupl->GetTransform()->SetPosition(960.f * width / 1920, 540.f * height / 1080, 0.f);
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
	nameDupl->GetTransform()->SetPosition(960.f * width / 1920, 540.f * height / 1080, 0.f);
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
	signupFail->GetTransform()->SetPosition(960.f * width / 1920, 540.f * height / 1080, 0.f);
	signupFail->SetSelfActive(false);
	_lobbyManager.SetSignupFail(signupFail);
	signupFail->GetComponent<HDData::Button>()->SetOnClickEvent(
		[=]()
		{
			LobbyManager::Instance().showOff(nameDupl);
		}
	);

	HDData::GameObject* signupSuccess = API::CreateButton(_scene, "signUpSUCC");
	signupSuccess->GetComponent<HDData::Button>()->SetImage("signup_s.png");
	signupSuccess->GetComponent<HDData::Button>()->SetSortOrder(0.4f);
	signupSuccess->GetTransform()->SetPosition(960.f * width / 1920, 540.f * height / 1080, 0.f);
	signupSuccess->SetSelfActive(false);
	LobbyManager::Instance().SetSignupSuccess(signupSuccess);
	signupSuccess->GetComponent<HDData::Button>()->SetOnClickEvent(
		[=]()
		{
			LobbyManager::Instance().showOff(signupSuccess);
			LobbyManager::Instance().showOff(joinControlObject);
			LobbyManager::Instance().showOn(loginControlObject);
		}
	);

	HDData::GameObject* exitJoinBtn = API::CreateButton(_scene, "exitJoinBtn", joinControlObject);
	exitJoinBtn->AddComponent<BtnHoveringScript>();
	exitJoinBtn->GetComponent<HDData::Button>()->SetSortOrder(0.5f);
	exitJoinBtn->GetComponent<HDData::Button>()->SetImage("checkbox_cross.png");
	exitJoinBtn->GetComponent<HDData::Button>()->ChangeScale(2.0f, 2.0f);
	exitJoinBtn->GetTransform()->SetPosition((1300.f * width / 1920) - 150, (200.f * height / 1080) + 120, 0.f);
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

	//EXIT Btn
	HDData::GameObject* exit_Btn = API::CreateButton(_scene, "TestingBtn");
	exit_Btn->GetTransform()->SetPosition(2480.0f, 1400.0f, 0.0f);
	exit_Btn->GetComponent<HDData::Button>()->SetImage("AlphaBtn.png");
	exit_Btn->GetComponent<HDData::Button>()->SetSortOrder(0.6f);
	exit_Btn->GetComponent<HDData::Button>()->ChangeScale(static_cast<float>(width) / 1920, static_cast<float>(height) / 1080);
	exit_Btn->AddComponent<BtnTextScript>();
	HDData::GameObject* tempText = API::CreateTextbox(_scene, "tempText", exit_Btn);
	tempText->GetTransform()->SetPosition(exit_Btn->GetTransform()->GetPosition());
	tempText->GetComponent<HDData::TextUI>()->SetFont("Resources/Font/KRAFTON_40.spriteFont");
	tempText->GetComponent<HDData::TextUI>()->SetText("EXIT");

	exit_Btn->GetComponent<HDData::Button>()->SetOnClickEvent(
		[]()
		{
			API::QuitWindow();
		}
	);

}

