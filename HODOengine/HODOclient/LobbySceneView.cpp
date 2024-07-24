#include "LobbySceneView.h"
#include "CameraMove.h"
#include "PlayerInfo.h"
#include "LobbyManager.h"
#include "GameManager.h"
#include "NetworkManager.h"
#include "RoundManager.h"
#include "BtnTextScript.h"

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
	testBox1->GetTransform()->SetScale(4, 3, 3);
	auto boxRender1 = testBox1->AddComponent<HDData::MeshRenderer>();
	boxRender1->SetUseLight(false);
	boxRender1->LoadMesh("primitiveQuad");
	HDEngine::MaterialDesc boxMat1;
	boxMat1.materialName = "_blur_LobbyBackGround";
	boxMat1.albedo = "_blur_LobbyBackGround.png";
	HDData::Material* newBoxMat1 = API::CreateMaterial(boxMat1);
	boxRender1->LoadMaterial(newBoxMat1, 0);

	// MAP SELECT

	// game start button
	auto startButton = API::CreateButton(_scene, "gameStartButton");
	startButton->GetComponent<HDData::Button>()->SetImage("Button_02.png");
	startButton->GetTransform()->SetPosition(1600.0f * width / 1920, 850.0f * height / 1080, 0);
	startButton->AddComponent<BtnTextScript>();
	auto startText = API::CreateTextbox(_scene, "gameStartText", startButton);
	auto sTex = startText->GetComponent<HDData::TextUI>();
	sTex->SetText("GAME START");
	sTex->SetColor(DirectX::Colors::OrangeRed);
	sTex->SetFont("Resources/Font/KRAFTON_40.spriteFont");
	float startTextWidth = sTex->GetTextWidth();
	float startTextHeight = sTex->GetTextHeight();
	startText->GetTransform()->SetPosition(1600.0f * width / 1920 - startTextWidth * 0.75f, 850.0f * height / 1080 - startTextHeight * 0.25f, 0);

	LobbyManager::Instance().SetInGameStartButton(startButton);

	startButton->SetSelfActive(false);

	auto quitButton = API::CreateButton(_scene, "roomQuitBtn");
	quitButton->GetTransform()->SetPosition(1600.0f * width / 1920, 1000.0f * height / 1080, 0);
	quitButton->AddComponent<BtnTextScript>();
	auto qBtn = quitButton->GetComponent<HDData::Button>();
	qBtn->GetButtonComp()->SetImage("Button_02.png");
	qBtn->SetOnClickEvent([]()
		{
			NetworkManager::Instance().SendRoomLeave();
		});
	auto quitText = API::CreateTextbox(_scene, "roomQuitText", quitButton);
	auto qTex = quitText->GetComponent<HDData::TextUI>();
	qTex->SetText("QUIT ROOM");
	qTex->SetColor(DirectX::Colors::OrangeRed);
	qTex->SetFont("Resources/Font/KRAFTON_40.spriteFont");
	float qTextWidth = sTex->GetTextWidth();
	float qTextHeight = sTex->GetTextHeight();
	qTex->GetTransform()->SetPosition((1600.0f * width / 1920 - qTextWidth * 0.75f) + 20, 1000.0f * height / 1080 - qTextHeight * 0.25f, 0);

	// game Rule
	auto ruleCanvas = API::CreateImageBox(_scene, "ruleCanvas");
	ruleCanvas->GetTransform()->SetPosition(390.0f * width / 1920, 932.0f * height / 1080, 0);
	auto ruleCanvasComp = ruleCanvas->GetComponent<HDData::ImageUI>();
	ruleCanvasComp->SetImage("Button_horizon_02.png");

	auto ruleImg = API::CreateImageBox(_scene, "ruleImg");
	ruleImg->GetTransform()->SetPosition(550.0f * width / 1920, (960.0f * height / 1080) + 40.0f, 0);
	auto ruleImgComp = ruleImg->GetComponent<HDData::ImageUI>();
	ruleImgComp->ChangeScale(0.6192f, 0.6314f);
	ruleImgComp->SetImage("gamerule.png");

	// Key
	auto keyInfoCanvas = API::CreateImageBox(_scene, "keyInfo");
	keyInfoCanvas->GetTransform()->SetPosition(1000.0f * width / 1920, 932.0f * height / 1080, 0);
	auto keyCanvasImg = keyInfoCanvas->GetComponent<HDData::ImageUI>();
	keyCanvasImg->SetSortOrder(0.09f);
	keyCanvasImg->ChangeScale(0.8f,1.0f);
	keyCanvasImg->SetImage("Button_horizon_02.png");

	auto aKey = API::CreateImageBox(_scene, "aKey");
	aKey->GetTransform()->SetPosition(750.0f * width / 1920, 900.0f * height / 1080, 0);
	auto aKeyImg = aKey->GetComponent<HDData::ImageUI>();
	aKeyImg->SetSortOrder(0.15f);
	aKeyImg->SetImage("keyboard_a.png");

	auto sKey = API::CreateImageBox(_scene, "sKey");
	sKey->GetTransform()->SetPosition(790.0f * width / 1920, 900.0f * height / 1080, 0);
	auto sKeyImg = sKey->GetComponent<HDData::ImageUI>();
	sKeyImg->SetSortOrder(0.15f);
	sKeyImg->SetImage("keyboard_s.png");

	auto wKey = API::CreateImageBox(_scene, "sKey");
	wKey->GetTransform()->SetPosition(790.0f * width / 1920, 860.0f * height / 1080, 0);
	auto wKeyImg = wKey->GetComponent<HDData::ImageUI>();
	wKeyImg->SetSortOrder(0.15f);
	wKeyImg->SetImage("keyboard_w.png");
	
	auto dKey = API::CreateImageBox(_scene, "dKey");
	dKey->GetTransform()->SetPosition(830.0f * width / 1920, 900.0f * height / 1080, 0);
	auto dKeyImg = dKey->GetComponent<HDData::ImageUI>();
	dKeyImg->SetSortOrder(0.15f);
	dKeyImg->SetImage("keyboard_d.png");

	auto moveInfo = API::CreateImageBox(_scene,"Move");
	moveInfo->GetTransform()->SetPosition(890.0f * width / 1920,890.0f * height / 1080,0.0f);
	auto moveInfoImg = moveInfo->GetComponent<HDData::ImageUI>();
	moveInfoImg->ChangeScale(1.7f, 1.7f);
	moveInfoImg->SetSortOrder(0.15f);
	moveInfoImg->SetImage("howtoplay_move.png");

	auto shiftKey = API::CreateImageBox(_scene, "shiftKey");
	shiftKey->GetTransform()->SetPosition(745.0f * width / 1920, 960.0f * height / 1080, 0);
	auto shiftKeyImg = shiftKey->GetComponent<HDData::ImageUI>();
	shiftKeyImg->ChangeScale(1.6f, 1.6f);
	shiftKeyImg->SetSortOrder(0.15f);
	shiftKeyImg->SetImage("keyboard_shift.png");

	auto shiftInfo = API::CreateImageBox(_scene,"shiftInfo");
	shiftInfo->GetTransform()->SetPosition(840.0f * width / 1920, 965.0f * height / 1080, 0.0f);
	auto shiftInfoImg = shiftInfo->GetComponent<HDData::ImageUI>();
	shiftInfoImg->ChangeScale(1.6f, 1.6f);
	shiftInfoImg->SetSortOrder(0.15f);
	shiftInfoImg->SetImage("howtoplay_roll.png");

	auto spaceKey = API::CreateImageBox(_scene, "spaceKey");
	spaceKey->GetTransform()->SetPosition(980.0f * width / 1920, 960.0f * height / 1080, 0);
	auto spaceKeyImg = spaceKey->GetComponent<HDData::ImageUI>();
	spaceKeyImg->ChangeScale(1.7f,1.7f);
	spaceKeyImg->SetSortOrder(0.15f);
	spaceKeyImg->SetImage("keyboard_space.png");

	auto spaceInfo = API::CreateImageBox(_scene, "spaceKeyInfo");
	spaceInfo->GetTransform()->SetPosition(1060.0f * width / 1920, 965.0f * height / 1080, 0);
	auto spaceInfoImg = spaceInfo->GetComponent<HDData::ImageUI>();
	spaceInfoImg->ChangeScale(1.7f, 1.7f);
	spaceInfoImg->SetSortOrder(0.15f);
	spaceInfoImg->SetImage("howtoplay_jump.png");

	auto mouseKey = API::CreateImageBox(_scene, "mouseKey");
	mouseKey->GetTransform()->SetPosition(985.0f * width / 1920, 865.0f * height / 1080, 0);
	auto mouseKeyImg = mouseKey->GetComponent<HDData::ImageUI>();
	mouseKeyImg->ChangeScale(1.7f, 1.7f);
	mouseKeyImg->SetSortOrder(0.15f);
	mouseKeyImg->SetImage("mouse_left.png");

	auto mouseInfo = API::CreateImageBox(_scene, "mouseKeyInfo");
	mouseInfo->GetTransform()->SetPosition(1060.0f * width / 1920, 870.0f * height / 1080, 0);
	auto mouseInfoImg = mouseInfo->GetComponent<HDData::ImageUI>();
	mouseInfoImg->ChangeScale(1.7f, 1.7f);
	mouseInfoImg->SetSortOrder(0.15f);
	mouseInfoImg->SetImage("howtoplay_attack.png");

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
		canvasMat.albedo = "Button_vertical_02.png";
		HDData::Material* newCanvasMat = API::CreateMaterial(canvasMat);
		canvasRenderer->LoadMaterial(newCanvasMat, 0);

		auto subCanvas = API::CreateImageBox(_scene, "subCanvas");
		subCanvas->GetComponent<HDData::ImageUI>()->SetImage("all_alpha.png");
		subCanvas->GetTransform()->SetPosition(rgbCanvas * width / 1920, 800.0f * height / 1080, 0);

		/*	auto QuitMemberButton = API::CreateButton(_scene, "QuitButton");
			QuitMemberButton->GetComponent<HDData::Button>()->SetSortOrder(0.3);
			QuitMemberButton->GetComponent<HDData::Button>()->SetImage("checkbox_cross.png");
			QuitMemberButton->GetTransform()->SetPosition(rgbCanvas * width / 1920, 60.0f * height / 1080, 0);
			QuitMemberButton->GetComponent<HDData::Button>()->SetOnClickEvent([=]() {
				NetworkManager::Instance().SendKickPlayer(LobbyManager::Instance().GetPlayerObjects()[i]->GetComponent<PlayerInfo>()->GetPlayerNickName());
				});
			LobbyManager::Instance().GetQuitButtonObjects().push_back(QuitMemberButton);
			QuitMemberButton->SetSelfActive(false);*/

		defaultX += 1.2f;
		rgbCanvas += 322.5f;

		subCanvas->SetSelfActive(false);
	}

	// player rendering

	float posX = 0;
	//float posT = 165;
	float posT = 150 * width / 1920;

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

		LobbyManager::Instance().GetPlayerObjects().push_back(player);

		HDData::GameObject* text = API::CreateTextbox(_scene, "player" + std::to_string(i), player);
		auto textUI = text->GetComponent<HDData::TextUI>();
		textUI->SetFont("Resources/Font/KRAFTON_40.spriteFont");
		float textWidth = textUI->GetTextWidth();
		float textHeight = textUI->GetTextHeight();
		text->GetTransform()->SetPosition(posT * width / 1920 - textWidth * 0.75f, 770.0f * height / 1080 - textHeight * 0.25f, 0);

		LobbyManager::Instance().GetNickNameObjects().push_back(text);

		player->SetSelfActive(false);

		posX += 1;
		posT += 300;
	}


	startButton->GetComponent<HDData::Button>()->SetOnClickEvent([]() {
		NetworkManager::Instance().SendGameStart();
		});

}

HDData::Scene* LobbySceneView::GetThisScene()
{
	return _scene;
}
