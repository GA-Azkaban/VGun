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
	teamLogo->GetTransform()->SetPosition(150.0f, 1300.0f, 0.0f);
	auto teamLogoComp = teamLogo->GetComponent<HDData::ImageUI>();
	teamLogoComp->SetSortOrder(0.12f);
	teamLogoComp->SetImage("teamLogo.png");
	teamLogoComp->ChangeScale(0.3f, 0.3f);

	// id input box
	HDData::GameObject* idTextbox = API::CreateTextInputBox(_scene, "idTextBox", loginControlObject);
	auto id = idTextbox->GetComponent<HDData::TextInputBoxUI>();
	//idTextbox->GetTransform()->SetPosition(950.0f * width / 1920, (440.f * height / 1080) + 600, 0.f);
	idTextbox->GetTransform()->SetPosition(API::GetScreenWidth() / 2, (440.f * height / 1080) + 600, 0.f);
	idTextbox->GetComponent<HDData::TextInputBoxUI>()->GetBackgroundImage()->SetSortOrder(0.2f);
	idTextbox->GetComponent<HDData::TextInputBoxUI>()->GetCursorImage()->SetSortOrder(0.21f);
	idTextbox->GetComponent<HDData::TextInputBoxUI>()->GetTextUI()->SetSortOrder(0.21f);

	HDData::GameObject* idTextLabel = API::CreateTextbox(_scene, "idTextLabel", loginControlObject);
	idTextLabel->GetTransform()->SetPosition(900.f * width / 1920, (440.f * height / 1080) + 500, 0.f);
	idTextLabel->GetComponent<HDData::TextUI>()->SetText("NICKNAME");
	idTextLabel->GetComponent<HDData::TextUI>()->SetFont("Resources/Font/KRAFTON_40.spriteFont");
	idTextLabel->GetComponent<HDData::TextUI>()->SetColor(DirectX::Colors::Red);
	idTextLabel->GetComponent<HDData::TextUI>()->SetSortOrder(0.21f);

	// login button
	HDData::GameObject* loginBtn = API::CreateButton(_scene, "loginBtn", loginControlObject);
	loginBtn->GetTransform()->SetPosition((API::GetScreenWidth() / 2) - 10.0f, (API::GetScreenHeight() / 2) + 600.0f, 0.f);
	loginBtn->GetComponent<HDData::Button>()->SetImage("AlphaBtn.png");
	loginBtn->GetComponent<HDData::Button>()->SetSortOrder(0.66f);
	loginBtn->AddComponent<BtnTextScript>();
	loginBtn->GetComponent<HDData::Button>()->SetOnClickEvent
	(
		[=]()
		{
			NetworkManager::Instance().SendAutoLogin(id->GetTextUI()->GetText());
			id->SetOrigin();
		}
	);
	HDData::GameObject* loginText = API::CreateTextbox(_scene, "loginText", loginBtn);
	loginText->GetTransform()->SetPosition(loginBtn->GetTransform()->GetPosition());
	auto logintxt = loginText->GetComponent<HDData::TextUI>();
	logintxt->SetFont("Resources/Font/KRAFTON_40.spriteFont");
	logintxt->SetDefaultColor(DirectX::Colors::Red);
	logintxt->SetSortOrder(0.65f);
	logintxt->SetText("LOGIN");

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
			//LobbyManager::Instance().showOff(joinControlObject);
			LobbyManager::Instance().showOn(loginControlObject);
		}
	);

	//EXIT Btn
	HDData::GameObject* exit_Btn = API::CreateButton(_scene, "TestingBtn");
	exit_Btn->GetTransform()->SetPosition(1850.0f * width / 1920, 1050.0f * height / 1080, 0.0f);
	exit_Btn->GetComponent<HDData::Button>()->SetImage("AlphaBtn.png");
	exit_Btn->GetComponent<HDData::Button>()->SetSortOrder(0.6f);
	//exit_Btn->GetComponent<HDData::Button>()->ChangeScale(static_cast<float>(width) / 1920, static_cast<float>(height) / 1080);
	exit_Btn->AddComponent<BtnTextScript>();

	HDData::GameObject* tempText = API::CreateTextbox(_scene, "tempText", exit_Btn);
	tempText->GetTransform()->SetPosition(1850.0f * width / 1920, 1050.0f * height / 1080, 0.0f);
	tempText->GetComponent<HDData::TextUI>()->SetFont("Resources/Font/KRAFTON_40.spriteFont");
	tempText->GetComponent<HDData::TextUI>()->SetSortOrder(0.5f);;
	tempText->GetComponent<HDData::TextUI>()->SetText("EXIT");

	exit_Btn->GetComponent<HDData::Button>()->SetOnClickEvent(
		[]()
		{
			API::QuitWindow();
		}
	);

}

