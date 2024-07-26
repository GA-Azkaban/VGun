#include "RoundManager.h"
#include "NetworkManager.h"
#include "LobbyManager.h"
#include "PlayerMove.h"
#include "GameManager.h"
#include "MenuManager.h"
#include "MeshTransformController.h"
#include "CameraMove.h"
#include "SoundManager.h"
#include "LowHPEffect.h"
#include "DamageLog.h"
#include "KillLog.h"

RoundManager* RoundManager::_instance = nullptr;

RoundManager* RoundManager::Instance()
{
	if (_instance == nullptr)
	{
		_instance = new RoundManager;
	}

	return _instance;
}

RoundManager::RoundManager()
{
	API::CreateStaticComponent(this);

	winnerMotion[0] = "RV_victory1";
	winnerMotion[1] = "RV_victory2";
	winnerMotion[2] = "RV_victory3";
}

void RoundManager::Start()
{
	_gameEndTimer = new Timer;
	_gameEndTimer->duration = 2;
	_gameEndTimer->onExpiration = [&]() {
		_showResultTimer->Start();
		_resultTimerUI->GetGameObject()->SetSelfActive(true);
		SoundManager::Instance().PlayBGM("bgm_victory");
		EndGame();
		};

	_initTimer = new Timer;
	_initTimer->duration = 3;
	_initTimer->onExpiration = [&]() {
		_initTimer->Stop();
		startRoundimg->GetGameObject()->GetComponent<UIEffect>()->Play();
		_initTimertxt->GetGameObject()->SetSelfActive(false);
		};

	_showResultTimer = new Timer;
	_showResultTimer->duration = 10;
	_showResultTimer->onExpiration = [&]() {
		ExitGame();
		_timerUI->SetColor(DirectX::Colors::White);
		SoundManager::Instance().StopAllPlayerSFX();
		};

	_serialKillTimer = new Timer;
	_serialKillTimer->duration = 8;
	_serialKillTimer->onExpiration = [&]()
		{
			_serialKillTimer->Stop();
			GameManager::Instance()->GetMyInfo()->_serialkillcount = 0;
		};

	SetUIActive(false);
}

void RoundManager::Update()
{
	UpdateBeginEndTimer();

	if (!_isRoundStart) return;

	UpdateRound();

	const uint64 frame = 16;
	static auto updateTick = 0;

	auto currentTick = ::GetTickCount64();

	if (updateTick > currentTick)
		return;

	updateTick = currentTick + frame;

	NetworkManager::Instance().SendPlayUpdate();

	if (API::GetKeyDown(DIK_ESCAPE) && !_ESCMenuOn)	// ESC메뉴 꺼져있을때
	{
		_ESCMenuOn = true;
	}
	else if (API::GetKeyDown(DIK_ESCAPE) && _ESCMenuOn)	// ESC메뉴 켜져있을때
	{
		_ESCMenuOn = false;
	}
}

void RoundManager::SetRoundScene(HDData::Scene* scene)
{
	_roundScene = scene;
}

void RoundManager::InitGame()
{
	GetNewDataFromLobby();
	InitializeValue();
	InitRound();
}

void RoundManager::EndGame()
{
	// UI 활성화, 비활성화
	SetUIActive(false);
	finRoundimg->GetGameObject()->SetSelfActive(false);
	tumbleAlphaImage->SetActive(false);
	tumbleCountText->SetActive(false);

	hitCrosshair->SetActive(false);
	criticalCrosshair->SetActive(false);
	damageLog->SetActive(false);
	killLog->SetActive(false);

	// 킬 카운트 정리
	_inGameKillCounts.clear();
	_endObj->SetSelfActive(true);

	// 라운드 종료
	API::SetCurrentSceneMainCamera(_endCam->GetComponent<HDData::Camera>());
	SetIsRoundStart(false);
	CheckWinner();
	FinishGame();
}

void RoundManager::InitRound()
{
	// 파티클 시스템 장착
	GameManager::Instance()->GetMyInfo()->SetParticleSystem(_myObj->GetComponentInChildren<HDData::ParticleSystem>());

	// 오브젝트들 활성화
	_myObj->SetSelfActive(true);
	for (auto& [uid, player] : _players)
	{
		PlayerInfo* info = player->GetComponent<PlayerInfo>();
		info->SetParticleSystem(player->GetComponentInChildren<HDData::ParticleSystem>());
		player->SetSelfActive(true);
	}

	// 회전초 위치 재정비
	ResetWeedPos();

	// 시작 타이머 세팅
	_initTimer->Start();
	_initTimertxt->GetGameObject()->SetSelfActive(true);
	_timerUI->SetColor(DirectX::Colors::White);

	// 시작 벨 울리기 
	SoundManager::Instance().PlayUI("sfx_bell");
}

void RoundManager::UpdateRound()
{
	UpdateRoundTimer();
	UpdateHPText();
	UpdateAmmoText();
	UpdateDesiredKillChecker();
}

void RoundManager::GetNewDataFromLobby()
{
	auto& roomDataPlayers = LobbyManager::Instance().GetRoomData()->_players;
	_playerNum = LobbyManager::Instance().GetPlayerNum();

	for (auto& p : _playerObjs)
	{
		p->SetSelfActive(false);
	}

	int index = 0;

	for (auto& player : roomDataPlayers)
	{
		if (player->GetPlayerUID() == GameManager::Instance()->GetMyInfo()->GetPlayerUID())
		{
			GameManager::Instance()->SetMyObject(_myObj);
			GameManager::Instance()->GetMyInfo()->Init();

			_myKillCount.first->SetText(GameManager::Instance()->GetMyInfo()->GetPlayerNickName());
			_myKillCount.first->SetColor(DirectX::Colors::WhiteSmoke);
			_myKillCount.second->SetText(std::to_string(GameManager::Instance()->GetMyInfo()->GetPlayerKillCount()));
			_myKillCount.second->SetColor(DirectX::Colors::Yellow);
		}
		else
		{
			if (index == _playerNum - 1) 
				break;

			auto playerInfo = _playerObjs[index]->GetComponent<PlayerInfo>();
			playerInfo->GetData(player);
			playerInfo->Init();

			auto mesh = _playerObjs[index]->GetComponentInChildren<HDData::SkinnedMeshRenderer>();
			mesh->LoadMesh(GameManager::Instance()->meshes[playerInfo->meshtype]);
			mesh->LoadMaterial(API::GetMaterial("PolygonWestern_Texture_01_A"));

			playerInfo->SetParticleSystem(_playerObjs[index]->GetComponentInChildren<HDData::ParticleSystem>());

			_players.insert({ player->GetPlayerUID(), _playerObjs[index] });

			auto plMove = _myObj->GetComponent<PlayerMove>();
			if (plMove != nullptr)
			{
				plMove->InsertOtherPlayerInfo(player->GetPlayerUID(), _playerObjs[index]->GetComponent<HDData::DynamicCapsuleCollider>());
			}

			_killCountObjs[index].first->SetText(player->GetPlayerNickName());
			_killCountObjs[index].first->SetColor(DirectX::Colors::Red);
			_killCountObjs[index].second->SetText(std::to_string(playerInfo->GetPlayerKillCount()));
			_killCountObjs[index].second->SetColor(DirectX::Colors::Red);
			_inGameKillCounts.insert({ player->GetPlayerUID(), _killCountObjs[index] });
			++index;
		}
	}
}

void RoundManager::InitializeValue()
{
	// 현재 max kill, 우승자 uid 기본값으로 세팅
	_desiredKill = 0;
	_nowMaxKill = 0;
	_winnerUID = GameManager::Instance()->GetMyInfo()->GetPlayerUID();

	// 현재 UI가 가진 각 킬 카운트 활성화 (값 정리 필요)
	_myKillCount.first->GetGameObject()->SetSelfActive(true);
	_myKillCount.second->GetGameObject()->SetSelfActive(true);

	for (int i = 0; i < _players.size(); ++i)
	{
		_killCountObjs[i].first->GetGameObject()->SetSelfActive(true);
		_killCountObjs[i].second->GetGameObject()->SetSelfActive(true);
	}
}


void RoundManager::SetUIOrigin()
{
	_myKillCount.first->GetGameObject()->SetSelfActive(false);
	_myKillCount.second->GetGameObject()->SetSelfActive(false);

	for (int i = 0; i < 5; ++i)
	{
		_killCountObjs[i].first->GetGameObject()->SetSelfActive(false);
		_killCountObjs[i].second->GetGameObject()->SetSelfActive(false);
	}

	_timerUI->SetColor(DirectX::Colors::White);

	for (int i = 0; i < 5; ++i)
	{
		_killCountObjs[i].first->GetGameObject()->SetSelfActive(false);
		_killCountObjs[i].second->GetGameObject()->SetSelfActive(false);
	}

	// UI 활성화, 비활성화
	_winnerTXT->GetGameObject()->SetSelfActive(false);
	_winnerImg->GetGameObject()->SetSelfActive(false);

	_players.clear();
}

void RoundManager::SetUIActive(bool isActive)
{
	_timerUI->GetGameObject()->SetSelfActive(isActive);
	_hpUI->GetGameObject()->SetSelfActive(isActive);
	_ammoUI->GetGameObject()->SetSelfActive(isActive);
	lowHPEffect->GetGameObject()->SetSelfActive(isActive);
	tumbleImage->GetGameObject()->SetSelfActive(isActive);
	hpImage->GetGameObject()->SetSelfActive(isActive);
	ammoImage->GetGameObject()->SetSelfActive(isActive);
	if (defaultCrosshair != nullptr)
	{
		defaultCrosshair->SetActive(isActive);
	}
}

bool RoundManager::CheckHeadColliderOwner(HDData::DynamicSphereCollider* collider)
{
	auto parentCol = collider->GetParentCollider();
	if (parentCol == nullptr)
	{
		return false;
	}
	else
	{
		int uid = collider->GetParentCollider()->GetGameObject()->GetComponent<PlayerInfo>()->GetPlayerUID();
		NetworkManager::Instance().SendPlayShoot(collider->GetTransform(), uid, Protocol::HIT_LOCATION_HEAD);
		return true;
	}
}

void RoundManager::CheckBodyColliderOwner(HDData::DynamicCapsuleCollider* collider)
{
	int uid = collider->GetGameObject()->GetComponent<PlayerInfo>()->GetPlayerUID();

	NetworkManager::Instance().SendPlayShoot(collider->GetTransform(), uid, Protocol::HIT_LOCATION_BODY);
}

void RoundManager::SendJump(int uid)
{
	NetworkManager::Instance().SendPlayJump();
}

std::unordered_map<int, HDData::GameObject*>& RoundManager::GetPlayerObjs()
{
	return _players;
}

int RoundManager::GetPlayerNum()
{
	return _playerNum;
}

void RoundManager::CheckWinner()
{
	int meshtype = 0;

	if (_winnerUID == GameManager::Instance()->GetMyInfo()->GetPlayerUID())
	{
		auto info = GameManager::Instance()->GetMyInfo();
		_winnerTXT->SetText(info->GetPlayerNickName());
		meshtype = info->meshtype;
	}
	else
	{
		auto info = _players[_winnerUID]->GetComponent<PlayerInfo>();
		_winnerTXT->SetText(info->GetPlayerNickName());
		meshtype = info->meshtype;
	}

	auto mesh = winnerObj->GetComponentInChildren<HDData::SkinnedMeshRenderer>();
	// mesh->LoadMesh(GameManager::Instance()->meshes[meshtype]);
	mesh->LoadMesh(GameManager::Instance()->meshes[_winnerUID%8]);

	mesh->LoadMaterial(API::GetMaterial("PolygonWestern_Texture_01_A"));
	mesh->PlayAnimation(winnerMotion[::GetTickCount64() % 3]);

	_winnerTXT->GetGameObject()->SetSelfActive(true);
	_winnerImg->GetGameObject()->SetSelfActive(true);
}

bool RoundManager::GetIsRoundStart()
{
	return _isRoundStart;
}

void RoundManager::SetIsRoundStart(bool isStart)
{
	_isRoundStart = isStart;
	SetUIActive(isStart);
	_myObj->GetComponent<PlayerMove>()->SetMovable(isStart);
}

void RoundManager::SetEndCam(HDData::GameObject* cam)
{
	_endCam = cam;
}

void RoundManager::SetStartCam(HDData::Camera* cam)
{
	_startCam = cam;
}

HDData::GameObject* RoundManager::GetEndCam()
{
	return _endCam;
}

void RoundManager::SetRoundEndButton(HDData::GameObject* obj)
{
	_endObj = obj;
}

HDData::GameObject* RoundManager::GetRoundEndButton()
{
	return _endObj;
}

void RoundManager::ExitGame()
{
	SetUIOrigin();

	API::SetCurrentSceneMainCamera(_startCam);
	_endObj->SetSelfActive(false);
	_showResultTimer->Stop();
	_resultTimerUI->GetGameObject()->SetSelfActive(false);
	NetworkManager::Instance().SendRoomLeave();

	// 로비로 복귀
	API::LoadSceneByName("MainMenu");
	MenuManager::Instance().RenderRoomList();
	API::GetCubeMap()->LoadCubeMapTexture("Day Sun Peak Clear Gray.dds");
	API::GetCubeMap()->SetEnvLightIntensity(1.0f);
}

void RoundManager::FinishGame()
{
	SoundManager::Instance().Stop("3d_footstep");

	if (!_isRoundStart)
		return;

	_isRoundStart = false;

	tumbleAlphaImage->SetActive(false);
	tumbleCountText->SetActive(false);

	SoundManager::Instance().StopAllPlayerSFX();

	_gameEndTimer->Start();
	finRoundimg->GetGameObject()->GetComponent<UIEffect>()->Play();
	_myObj->GetComponent<PlayerMove>()->ResetState();
	_myObj->GetComponent<PlayerMove>()->StopMoving();
}

void RoundManager::SetWinnerText(HDData::TextUI* txt)
{
	_winnerTXT = txt;
}

void RoundManager::SetWinnerImg(HDData::ImageUI* img)
{
	_winnerImg = img;
}

void RoundManager::SetLoserText(HDData::TextUI* txt, int index)
{
	_loserTXT[index] = txt;
}

void RoundManager::SetLoserImg(HDData::ImageUI* img)
{
	_loserImg = img;
}

bool RoundManager::GetMenuStatus()
{
	return _ESCMenuOn;
}

void RoundManager::SetRoundTimerObject(HDData::TextUI* obj)
{
	_timerUI = obj;
	_timerUI->SetFont("Resources/Font/KRAFTON_55.spriteFont");
	_timerUI->GetTransform()->SetPosition(API::GetScreenWidth() / 2, 60.0f, 0.0f);
}

void RoundManager::SetRoundTimer(int time)
{
	_timer = time;
	_timerUI->SetText(ChangeSecToMin(time));
}

void RoundManager::SetStartTime(std::chrono::time_point<std::chrono::steady_clock> time)
{
	_start_time = time;
}

int& RoundManager::GetRoundTimer()
{
	return _timer;
}

void RoundManager::UpdateRoundTimer()
{
	if (_isRoundStart)
	{
		auto currentTime = std::chrono::steady_clock::now();
		std::chrono::duration<double> elapsedTime = currentTime - _start_time;

		auto nowElapsed = static_cast<int>(_timer - elapsedTime.count());
		_timerUI->SetText(ChangeSecToMin(nowElapsed));

		if (nowElapsed == 118)
		{
			for (int i = 0; i < _weedColVector.size(); ++i)
			{
				Vector3 axis = _weedColVector[i]->GetTransform()->GetRight();
				axis.y += i * 0.01f;
				axis.Normalize();
				_weedColVector[i]->AddTorque(axis, 3.0f, 0);
			}
		}
		else if (nowElapsed >= 107 && nowElapsed <= 110)
		{
			for (int i = 0; i < _weedColVector.size(); ++i)
			{
				Vector3 axis = _weedColVector[i]->GetTransform()->GetForward();
				axis.y += i * 0.01f;
				axis.Normalize();
				_weedColVector[i]->AddTorque(axis, 2.0f, 0);
			}
		}
		else if (nowElapsed >= 87 && nowElapsed <= 90)
		{
			for (auto& col : _weedColVector)
			{
				Vector3 axis = col->GetTransform()->GetRight() * -1;
				col->AddTorque(axis, 2.0f, 0);
			}
		}
		else if (nowElapsed >= 69 && nowElapsed <= 70)
		{
			for (auto& col : _weedColVector)
			{
				Vector3 axis = col->GetTransform()->GetForward() * -1;
				col->AddTorque(axis, 3.0f, 0);
			}
		}
		else if (nowElapsed >= 59 && nowElapsed <= 60)
		{
			for (auto& col : _weedColVector)
			{
				Vector3 axis = col->GetTransform()->GetRight();
				col->AddTorque(axis, 3.0f, 0);
			}
		}
		else if (nowElapsed >= 48 && nowElapsed <= 50)
		{
			for (auto& col : _weedColVector)
			{
				Vector3 axis = col->GetTransform()->GetForward();
				col->AddTorque(axis, 2.0f, 0);
			}
		}
		else if (nowElapsed >= 38 && nowElapsed <= 40)
		{
			for (auto& col : _weedColVector)
			{
				Vector3 axis = col->GetTransform()->GetRight() * -1;
				col->AddTorque(axis, 2.0f, 0);
			}
		}
		else if (nowElapsed >= 28 && nowElapsed <= 30)
		{
			for (auto& col : _weedColVector)
			{
				Vector3 axis = col->GetTransform()->GetForward() * -1;
				col->AddTorque(axis, 3.0f, 0);
			}
		}
		else if (nowElapsed >= 18 && nowElapsed <= 20)
		{
			for (auto& col : _weedColVector)
			{
				Vector3 axis = col->GetTransform()->GetRight();
				col->AddTorque(axis, 3.0f, 0);
			}
		}
		else if (nowElapsed >= 8 && nowElapsed <= 10)
		{
			for (auto& col : _weedColVector)
			{
				Vector3 axis = col->GetTransform()->GetForward();
				col->AddTorque(axis, 4.0f, 0);
			}
		}

		if (nowElapsed <= 10)
		{
			_timerUI->SetColor(DirectX::Colors::Red);
			// TODO) 사운드 이펙트 넣기
		}
		if (elapsedTime.count() >= _timer)
		{
			FinishGame();
		}
	}
}

std::string RoundManager::ChangeSecToMin(int second)
{
	std::string min = "";
	std::string sec = "";

	if (second / 60 == 0)
	{
		min = "00";
	}
	else
	{
		min = std::to_string(second / 60);
	}

	if (second % 60 < 10)
	{
		sec = "0" + std::to_string(second % 60);
	}
	else
	{
		sec = std::to_string(second % 60);
	}

	std::string result = min + " : " + sec;

	return result;
}

void RoundManager::SetHPObject(HDData::TextUI* txt)
{
	_hpUI = txt;
}

void RoundManager::UpdateHPText()
{
	_hpUI->SetText(std::to_string(GameManager::Instance()->GetMyInfo()->GetPlayerCurrentHP()));
}

void RoundManager::SetAmmoText(HDData::TextUI* txt)
{
	_ammoUI = txt;
}

void RoundManager::UpdateAmmoText()
{
	std::string count = std::to_string(GameManager::Instance()->GetMyInfo()->GetCurrentBulletCount());
	_ammoUI->SetText(count + "/6");
}

void RoundManager::UpdateBeginEndTimer()
{
	if (API::GetCurrentSceneName() != "InGame") return;

	_initTimer->Update();
	_gameEndTimer->Update();
	_showResultTimer->Update();
	_initTimertxt->SetText(std::to_string(static_cast<int>(_initTimer->duration - _initTimer->GetElapsedTime())));

	if (_showResultTimer->IsActive())
	{
		_resultTimerUI->SetText("Quit by..." + std::to_string(static_cast<int>(_showResultTimer->duration - _showResultTimer->GetElapsedTime())));
	}
}

void RoundManager::SetResultTimerUI(HDData::TextUI* txt)
{
	_resultTimerUI = txt;
}

Timer* RoundManager::GetGameEndTimer()
{
	return _gameEndTimer;
}

void RoundManager::SetInitRoundTimer(HDData::TextUI* txt)
{
	_initTimertxt = txt;
}

void RoundManager::StartSerialKillTimer()
{
	_serialKillTimer->Start();
}

void RoundManager::UpdateDesiredKillChecker()
{
	// 연속 킬 타이머 업데이트
	_serialKillTimer->Update();

	// 해당 킬 값으로 텍스트를 계속 세팅해줌
	{
		int count = GameManager::Instance()->GetMyInfo()->GetPlayerKillCount();

		_myKillCount.second->SetText(std::to_string(count));

// 		if (count > _nowMaxKill)
// 		{
// 			_nowMaxKill = count;
// 			_winnerUID = GameManager::Instance()->GetMyInfo()->GetPlayerUID();
// 		}
	}

	for (auto& [uid, player] : _players)
	{
		int count = player->GetComponent<PlayerInfo>()->GetPlayerKillCount();

		_inGameKillCounts[uid].second->SetText(std::to_string(count));

// 		if (count > _nowMaxKill)
// 		{
// 			_nowMaxKill = count;
// 			// _winnerUID = uid;
// 		}
	}
}

void RoundManager::SetDesiredKill(int count)
{
	_desiredKill = count;
}

int& RoundManager::GetDesiredKill()
{
	return _desiredKill;
}

void RoundManager::SetKillCountUI(HDData::TextUI* nick, HDData::TextUI* count, int index)
{
	_killCountObjs[index] = std::make_pair(nick, count);
}

//void RoundManager::SetKillCountBack(HDData::ImageUI* img, int index)
//{
//	_backIMG[index] = img;
//}

std::unordered_map<int, std::pair<HDData::TextUI*, HDData::TextUI*>>& RoundManager::GetKillCountMap()
{
	return _inGameKillCounts;
}

void RoundManager::SetAnimationDummy(HDData::GameObject* obj)
{
	_animationDummy = obj;
}

HDData::GameObject* RoundManager::GetAnimationDummy()
{
	return _animationDummy;
}

void RoundManager::SetWeedColVector(std::vector<HDData::DynamicSphereCollider*>& vec)
{
	_weedColVector = vec;
}

void RoundManager::ResetWeedPos()
{
	int weedPos[20][2] = { {-38, 14}, {-34, -26}, {-34, -14}, {-31, 8}, {-28, -15}, {-22, 1}, {-20, -30}, {-19, 19}, {-14, 14}, {-8, -25},
						{-8, -3}, {0, -2}, {0, -14}, {3, 0}, {3, -21}, {7, -30}, {14, 4}, {20, -7}, {22, 8}, {28, 13} };

	for (int i = 0; i < 20; ++i)
	{
		_weedColVector[i]->SetColliderPosition(Vector3(weedPos[i][0], 0.4f, weedPos[i][1]));
	}
}

