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

	auto mainCam = _scene->GetMainCamera();
	mainCam->GetGameObject()->AddComponent<CameraMove>();
	mainCam->GetGameObject()->GetTransform()->SetPosition(2.62, 0.57, -5.48);
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
	startButton->GetTransform()->SetPosition(1600, 950, 0);

	auto startText = API::CreateTextbox(_scene, "gameStartText", startButton);
	startText->GetTransform()->SetPosition(1600, 950, 0);
	auto sTex = startText->GetComponent<HDData::TextUI>();
	sTex->SetText("GAME START");
	sTex->SetColor(DirectX::Colors::OrangeRed);
	sTex->SetFont("Resources/Font/KRAFTON_40.spriteFont");

	LobbyManager::Instance().SetInGameStartButton(startButton);

	startButton->SetSelfActive(false);


	auto quitButton = API::CreateButton(_scene, "roomQuitBtn");
	quitButton->GetTransform()->SetPosition(200, 950, 0);
	auto qBtn = quitButton->GetComponent<HDData::Button>();
	qBtn->GetButtonComp()->SetImage("exitRoom.png");
	qBtn->SetOnClickEvent([]()
		{
			NetworkManager::Instance().SendRoomLeave();
		});

	// Create Meterial
	HDEngine::MaterialDesc red;
	red.materialName = "TP_Red";
	red.albedo = "TP_Red_B.png";

	HDData::Material* M_Red = API::CreateMaterial(red);

	HDEngine::MaterialDesc green;
	green.materialName = "TP_Green";
	green.albedo = "TP_Green_B.png";

	HDData::Material* M_Green = API::CreateMaterial(green);

	HDEngine::MaterialDesc blue;
	blue.materialName = "TP_Blue";
	blue.albedo = "TP_Blue_B.png";

	HDData::Material* M_Blue = API::CreateMaterial(blue);

	float defaultX = 165;
	float r = 85;
	float g = 165;
	float b = 245;

	for (int i = 0; i < 6; ++i)
	{
		auto defaultCanvas = API::CreateImageBox(_scene, "defaultCanvas");
		defaultCanvas->GetTransform()->SetPosition(defaultX, 450, 0);
		auto img = defaultCanvas->GetComponent<HDData::ImageUI>();
		img->SetImage("back_char.png");
		img->SetWorldSpace();

		auto subCanvas = API::CreateImageBox(_scene, "subCanvas", defaultCanvas);
		subCanvas->GetComponent<HDData::ImageUI>()->SetImage("all_alpha.png");
		subCanvas->GetTransform()->SetPosition(defaultX, 800, 0);

		auto QuitMemberButton = API::CreateButton(_scene, "QuitButton");
		QuitMemberButton->GetComponent<HDData::Button>()->SetSortOrder(0.3);
		QuitMemberButton->GetComponent<HDData::Button>()->SetImage("checkbox_cross.png");
		QuitMemberButton->GetTransform()->SetPosition(defaultX, 60, 0);
		QuitMemberButton->GetComponent<HDData::Button>()->SetOnClickEvent([=]() {
			NetworkManager::Instance().SendKickPlayer(LobbyManager::Instance().GetPlayerObjects()[i]->GetComponent<PlayerInfo>()->GetPlayerNickName());
			});
		LobbyManager::Instance().GetQuitButtonObjects().push_back(QuitMemberButton);
		QuitMemberButton->SetSelfActive(false);

		auto rButton = API::CreateButton(_scene, "R", subCanvas);
		auto rBtn = rButton->GetComponent<HDData::Button>();
		rBtn->SetSortOrder(0.3);
		rButton->GetTransform()->SetPosition(r, 800, 0);
		rBtn->SetImage("r.png");
		rBtn->SetOnClickEvent([=]()
			{
				std::string nick = LobbyManager::Instance().GetRoomData()->_players[i]->GetPlayerNickName();
				NetworkManager::Instance().SendChangeTeamColor(Protocol::TEAM_COLOR_RED, nick);
			});

		auto gButton = API::CreateButton(_scene, "G", subCanvas);
		auto gBtn = gButton->GetComponent<HDData::Button>();
		gBtn->SetSortOrder(0.3);
		gButton->GetTransform()->SetPosition(g, 800, 0);
		gBtn->SetImage("g.png");
		gBtn->SetOnClickEvent([=]()
			{
				std::string nick = LobbyManager::Instance().GetRoomData()->_players[i]->GetPlayerNickName();
				NetworkManager::Instance().SendChangeTeamColor(Protocol::TEAM_COLOR_GREEN, nick);
			});

		auto bButton = API::CreateButton(_scene, "B", subCanvas);
		auto bBtn = bButton->GetComponent<HDData::Button>();
		bBtn->SetSortOrder(0.3);
		bButton->GetTransform()->SetPosition(b, 800, 0);
		bBtn->SetImage("b.png");
		bBtn->SetOnClickEvent([=]()
			{
				std::string nick = LobbyManager::Instance().GetRoomData()->_players[i]->GetPlayerNickName();
				NetworkManager::Instance().SendChangeTeamColor(Protocol::TEAM_COLOR_BLUE, nick);
			});

		defaultX += 315;
		r += 315;
		g += 315;
		b += 315;

		LobbyManager::Instance().GetTeamButtonObjects().push_back(subCanvas);
		subCanvas->SetSelfActive(false);
	}

	// player rendering

	float posX = 0;
	float posT = 165;

	for (int i = 0; i < 6; ++i)
	{
		HDData::GameObject* player = API::CreateObject(_scene, "player");
		player->LoadFBXFile("SKM_TP_X_Default.fbx");
		player->GetTransform()->SetPosition(posX, 0, 0);
		player->GetTransform()->Rotate(0, -180, 0);

		auto meshComp = player->GetComponentInChildren<HDData::SkinnedMeshRenderer>();
		meshComp->LoadMaterial(M_Red, 0);
		meshComp->LoadMaterial(M_Red, 1);
		meshComp->LoadMaterial(M_Red, 2);
		meshComp->LoadMaterial(M_Red, 3);
		meshComp->LoadMaterial(M_Red, 4);

		player->AddComponent<PlayerInfo>();

		player->AddComponent<HDData::Animator>();
		API::LoadFPAnimationFromData(player, "FP_animation.json");

		LobbyManager::Instance().GetPlayerObjects().push_back(player);

		HDData::GameObject* text = API::CreateTextbox(_scene, "player" + std::to_string(i), player);
		text->GetTransform()->SetPosition(player->GetTransform()->GetPosition());
		text->GetTransform()->SetPosition(posT, 30, 0);

		LobbyManager::Instance().GetNickNameObjects().push_back(text);

		//player->SetSelfActive(false);

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
