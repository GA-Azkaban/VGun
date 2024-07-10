#include "LobbySceneView.h"
#include "CameraMove.h"
#include "PlayerInfo.h"
#include "LobbyManager.h"
#include "GameManager.h"
#include "NetworkManager.h"
#include "FPAniScript.h"
#include "RoundManager.h"

LobbySceneView::LobbySceneView()
{

}

LobbySceneView::~LobbySceneView()
{

}

void LobbySceneView::Initialize()
{
	_scene = API::CreateScene("Lobby");

	int width = static_cast<int>(API::GetScreenWidth());
	int height = static_cast<int>(API::GetScreenHeight());

	auto mainCam = _scene->GetMainCamera();
	//mainCam->GetGameObject()->AddComponent<CameraMove>();
	mainCam->GetGameObject()->GetTransform()->SetPosition(2.6f, 0.57f, -4.4f);
	mainCam->GetGameObject()->GetTransform()->Rotate(0, -0.5, 0);

	// 백그라운드
	auto testBox1 = API::CreateObject(_scene);
	testBox1->GetComponent<HDData::Transform>()->SetPosition(2.5f, 0.f, 1.0f);
	testBox1->GetTransform()->SetScale(4, 4, 4);
	auto boxRender1 = testBox1->AddComponent<HDData::MeshRenderer>();
	boxRender1->SetUseLight(false);
	boxRender1->LoadMesh("primitiveQuad");
	HDEngine::MaterialDesc boxMat1;
	boxMat1.materialName = "_blur_background_image";
	boxMat1.albedo = "_blur_background_image.png";
	HDData::Material* newBoxMat1 = API::CreateMaterial(boxMat1);
	boxRender1->LoadMaterial(newBoxMat1, 0);

	// MAP SELECT


	// game start button
	auto startButton = API::CreateButton(_scene, "gameStartButton");
	startButton->GetComponent<HDData::Button>()->SetImage("start.png");
	startButton->GetTransform()->SetPosition(1600.0f * width / 1920, 950.0f * height / 1080, 0);

	auto startText = API::CreateTextbox(_scene, "gameStartText", startButton);
	auto sTex = startText->GetComponent<HDData::TextUI>();
	sTex->SetText("GAME START");
	sTex->SetColor(DirectX::Colors::OrangeRed);
	sTex->SetFont("Resources/Font/KRAFTON_40.spriteFont");
	float startTextWidth = sTex->GetTextWidth();
	float startTextHeight = sTex->GetTextHeight();
	startText->GetTransform()->SetPosition(1600.0f * width / 1920 - startTextWidth * 0.75f, 950.0f * height / 1080 - startTextHeight * 0.25f, 0);

	LobbyManager::Instance().SetInGameStartButton(startButton);

	startButton->SetSelfActive(false);

	auto quitButton = API::CreateButton(_scene, "roomQuitBtn");
	quitButton->GetTransform()->SetPosition(320.0f * width / 1920, 950.0f * height / 1080, 0);
	auto qBtn = quitButton->GetComponent<HDData::Button>();
	qBtn->GetButtonComp()->SetImage("exitRoom.png");
	qBtn->SetOnClickEvent([]()
		{
			NetworkManager::Instance().SendRoomLeave();
		});

	// Create Meterial
	HDEngine::MaterialDesc red;
	red.materialName = "TP_defalut";
	red.albedo = "PolygonWestern_Texture_01_A.png";

	HDData::Material* M_Red = API::CreateMaterial(red);


	float defaultX = -0.5f;
	float rgbCanvas = 155.0f;

	for (int i = 0; i < 6; ++i)
	{
		auto defaultCanvas = API::CreateObject(_scene, "defaultCanvas");
		defaultCanvas->GetTransform()->SetPosition(defaultX, 0.9f, 0.9f);
		defaultCanvas->GetTransform()->SetScale(1.1f, 0.5f, 0.0f);
		defaultCanvas->GetTransform()->Rotate(0.0f, 0.0f, 90.0f);
		auto canvasRenderer = defaultCanvas->AddComponent<HDData::MeshRenderer>();
		canvasRenderer->SetUseLight(false);
		canvasRenderer->LoadMesh("primitiveQuad");
		HDEngine::MaterialDesc canvasMat;
		canvasMat.materialName = "canvas";
		canvasMat.albedo = "settingCanvas.png";
		HDData::Material* newCanvasMat = API::CreateMaterial(canvasMat);
		canvasRenderer->LoadMaterial(newCanvasMat, 0);

		auto subCanvas = API::CreateImageBox(_scene, "subCanvas");
		subCanvas->GetComponent<HDData::ImageUI>()->SetImage("all_alpha.png");
		subCanvas->GetTransform()->SetPosition(rgbCanvas * width / 1920, 800.0f * height / 1080, 0);

		auto QuitMemberButton = API::CreateButton(_scene, "QuitButton");
		QuitMemberButton->GetComponent<HDData::Button>()->SetSortOrder(0.3);
		QuitMemberButton->GetComponent<HDData::Button>()->SetImage("checkbox_cross.png");
		QuitMemberButton->GetTransform()->SetPosition(rgbCanvas * width / 1920, 60.0f * height / 1080, 0);
		QuitMemberButton->GetComponent<HDData::Button>()->SetOnClickEvent([=]() {
			NetworkManager::Instance().SendKickPlayer(LobbyManager::Instance().GetPlayerObjects()[i]->GetComponent<PlayerInfo>()->GetPlayerNickName());
			});
		LobbyManager::Instance().GetQuitButtonObjects().push_back(QuitMemberButton);
		QuitMemberButton->SetSelfActive(false);

		defaultX += 1.2f;
		rgbCanvas += 322.5f;

		subCanvas->SetSelfActive(false);
	}

	// player rendering

	float posX = 0;
	//float posT = 165;
	float posT = 100 * width / 1920;

	for (int i = 0; i < 6; ++i)
	{
		HDData::GameObject* player = API::CreateObject(_scene, "player");
		player->LoadFBXFile("SKM_CowboyTP_X_default.fbx");
		player->GetTransform()->SetPosition(posX, 0, 0);
		player->GetTransform()->Rotate(0, -180, 0);

		auto meshComp = player->GetComponentInChildren<HDData::SkinnedMeshRenderer>();
		meshComp->LoadAnimation("TP");
		meshComp->LoadMaterial(M_Red, 0);
		meshComp->PlayAnimation("RV_idle", true, 0.1, true, 0.1);
		player->AddComponent<PlayerInfo>();

		/*player->AddComponent<HDData::Animator>();
		API::LoadFPAnimationFromData(player, "FP_animation.json");*/

		LobbyManager::Instance().GetPlayerObjects().push_back(player);

		HDData::GameObject* text = API::CreateTextbox(_scene, "player" + std::to_string(i), player);
		auto textUI = text->GetComponent<HDData::TextUI>();
		float textWidth = textUI->GetTextWidth();
		float textHeight = textUI->GetTextHeight();
		text->GetTransform()->SetPosition(posT * width / 1920 - textWidth * 0.75f, 30.0f * height / 1080 - textHeight * 0.25f, 0);

		LobbyManager::Instance().GetNickNameObjects().push_back(text);

		player->SetSelfActive(false);

		posX += 1;
		posT += 315;
	}


	startButton->GetComponent<HDData::Button>()->SetOnClickEvent([]() {
		NetworkManager::Instance().SendGameStart();
		});

}

HDData::Scene* LobbySceneView::GetThisScene()
{
	return _scene;
}
