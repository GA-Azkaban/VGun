#include "LobbySceneView.h"
#include "CameraMove.h"
#include "PlayerInfo.h"
#include "LobbyManager.h"
#include "GameManager.h"

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
	mainCam->GetGameObject()->GetTransform()->SetPosition(-24.41, 0.3, 2.56);
	mainCam->GetGameObject()->GetTransform()->Rotate(-10, -124, 0);

	// 백그라운드
	//HDData::GameObject* mainCanvas = API::CreateImageBox(_scene, "mainmenuCanvas");
	//mainCanvas->GetComponent<HDData::ImageUI>()->SetImage("_blur_background_image.png");
	//mainCanvas->GetComponent<HDData::ImageUI>()->SetSortOrder(0.0f);
	//mainCanvas->GetTransform()->SetPosition(960.f, 540.f, 0.f);
	//mainCanvas->GetComponent < HDData::ImageUI>()->SetActive(true);

	// MAP SELECT


	// game start button
	auto startButton = API::CreateButton(_scene, "gameStartButton");
	startButton->GetComponent<HDData::Button>()->SetImage("start.png");
	startButton->GetTransform()->SetPosition(1600, 950, 0);

	auto startText = API::CreateTextbox(_scene, "gameStartText", startButton);
	auto sTex = startText->GetComponent<HDData::TextUI>();
	startText->GetTransform()->SetPosition(1600, 950, 0);
	sTex->SetText("GAME START");
	sTex->SetColor(DirectX::Colors::OrangeRed);
	startText->GetComponent<HDData::TextUI>()->SetFont("Resources/Font/KRAFTON_40.spriteFont");

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

	float defaultX = 1740;
	float r = 1660;
	float g = 1740;
	float b = 1820;

	for (int i = 0; i < 6; ++i)
	{
		auto defaultCanvas = API::CreateImageBox(_scene, "defaultCanvas");
		defaultCanvas->GetTransform()->SetPosition(defaultX, 450, 0);
		auto img = defaultCanvas->GetComponent<HDData::ImageUI>();
		img->SetImage("settingCanvas.png");

		auto teamSelectSubCanvas = API::CreateImageBox(_scene, "teamSelectSubCanvas", defaultCanvas);
		teamSelectSubCanvas->GetComponent<HDData::ImageUI>()->SetImage("subCanvas_alpha.png");
		teamSelectSubCanvas->GetTransform()->SetPosition(defaultX, 800, 0);

		auto QuitMemberButton = API::CreateButton(_scene, "QuitButton", defaultCanvas);
		QuitMemberButton->GetComponent<HDData::Button>()->SetImage("checkbox_cross.png");
		QuitMemberButton->GetTransform()->SetPosition(defaultX, 60, 0);

		auto rButton = API::CreateButton(_scene, "R", teamSelectSubCanvas);
		auto rBtn = rButton->GetComponent<HDData::Button>();
		rBtn->SetSortOrder(0.3);
		rButton->GetTransform()->SetPosition(r, 800, 0);
		rBtn->SetImage("icon_group_cog.png");
		rBtn->SetOnClickEvent([]()
			{
				GameManager::Instance()->GetMyInfo()->SetTeamID(eTeam::R);
			});

		auto gButton = API::CreateButton(_scene, "G", teamSelectSubCanvas);
		auto gBtn = gButton->GetComponent<HDData::Button>();
		gBtn->SetSortOrder(0.3);
		gButton->GetTransform()->SetPosition(g, 800, 0);
		gBtn->SetImage("icon_group_cog.png");
		gBtn->SetOnClickEvent([]()
			{
				GameManager::Instance()->GetMyInfo()->SetTeamID(eTeam::G);
			});

		auto bButton = API::CreateButton(_scene, "B", teamSelectSubCanvas);
		auto bBtn = bButton->GetComponent<HDData::Button>();
		bBtn->SetSortOrder(0.3);
		bButton->GetTransform()->SetPosition(b, 800, 0);
		bBtn->SetImage("icon_group_cog.png");
		bBtn->SetOnClickEvent([]()
			{
				GameManager::Instance()->GetMyInfo()->SetTeamID(eTeam::B);
			});

		defaultX -= 315;
		r -= 315;
		g -= 315;
		b -= 315;
	}

	// player rendering

	float posX = 0;

	for (int i = 0; i < 6; ++i)
	{
		HDData::GameObject* player = API::CreateObject(_scene);
		player->LoadFBXFile("SKM_TP_X_Default.fbx");
		player->GetTransform()->SetPosition(posX, 0, 0);

		auto meshComp = player->GetComponentInChildren<HDData::SkinnedMeshRenderer>();
		meshComp->LoadMaterial(M_Red, 0);
		meshComp->LoadMaterial(M_Red, 1);
		meshComp->LoadMaterial(M_Red, 2);
		meshComp->LoadMaterial(M_Red, 3);
		meshComp->LoadMaterial(M_Red, 4);

		meshComp->PlayAnimation("AR_idle", true);

		LobbyManager::Instance().GetPlayerObjects().push_back(player);

		HDData::GameObject* text = API::CreateTextbox(_scene, "player" + std::to_string(i), player);
		text->GetTransform()->SetPosition(player->GetTransform()->GetPosition());
		text->GetTransform()->SetPosition(posX, 0, 0);

		LobbyManager::Instance().GetNickNameObjects().push_back(text);

		posX += 1;
	}

}

HDData::Scene* LobbySceneView::GetThisScene()
{
	return _scene;
}
