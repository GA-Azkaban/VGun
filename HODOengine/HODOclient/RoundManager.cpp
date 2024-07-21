﻿#include "RoundManager.h"
#include "NetworkManager.h"
#include "LobbyManager.h"
#include "PlayerMove.h"
#include "GameManager.h"
#include "MenuManager.h"
#include "MeshTransformController.h"
#include "CameraMove.h"
#include "LowHPEffect.h"

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
}

void RoundManager::Start()
{
	_gameEndTimer = new Timer;
	_gameEndTimer->duration = 2;
	_gameEndTimer->onExpiration = [&]() {
		_showResultTimer->Start();
		_resultTimerUI->GetGameObject()->SetSelfActive(true);
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
	// 라운드 초기화

	auto& obj = LobbyManager::Instance().GetPlayerObjects();
	_playerNum = LobbyManager::Instance().GetPlayerNum();
	_timerUI->SetColor(DirectX::Colors::White);
	_nowMaxKill = 0;
	_winnerUID = NULL;

	// UI 활성화, 비활성화
	_winnerTXT->GetGameObject()->SetSelfActive(false);
	for (int i = 0; i < 5; ++i)
	{
		_loserTXT[i]->GetGameObject()->SetSelfActive(false);
	}

	for (auto& p : _playerObjs)
	{
		p->SetSelfActive(false);
	}

	_initTimer->Start();
	_initTimertxt->GetGameObject()->SetSelfActive(true);

	_players.clear();

	int index = 0;

	for (auto& one : obj)
	{
		if (index >= _playerNum) break;

		PlayerInfo* info = one->GetComponent<PlayerInfo>();

		if (info->GetPlayerUID() == GameManager::Instance()->GetMyInfo()->GetPlayerUID())
		{
			GameManager::Instance()->SetMyObject(_myObj);
			_killCountObjs[index].first->SetText(GameManager::Instance()->GetMyInfo()->GetPlayerNickName());
			_killCountObjs[index].first->SetColor(DirectX::Colors::WhiteSmoke);
			_killCountObjs[index].second->SetColor(DirectX::Colors::WhiteSmoke);
			_inGameKillCounts.insert({ info->GetPlayerUID(), _killCountObjs[index] });
		}
		else
		{
			auto playerInfo = _playerObjs[index]->AddComponent<PlayerInfo>(info);
			playerInfo->SetParticleSystem(_playerObjs[index]->GetComponentInChildren<HDData::ParticleSystem>());
			_players.insert({ info->GetPlayerUID(), _playerObjs[index] });
			_killCountObjs[index].first->SetText(info->GetPlayerNickName());
			_killCountObjs[index].first->SetColor(DirectX::Colors::Red);
			_killCountObjs[index].second->SetColor(DirectX::Colors::Red);
			_inGameKillCounts.insert({ info->GetPlayerUID(), _killCountObjs[index] });
		}

		++index;
	}

	InitRound();
}

void RoundManager::EndGame()
{
	// UI 활성화, 비활성화
	SetUIActive(false);
	finRoundimg->GetGameObject()->SetSelfActive(false);
	tumbleAlphaImage->SetActive(false);
	tumbleCountText->SetActive(false);

	for (int i = 0; i < 6; ++i)
	{
		//_backIMG[i]->GetGameObject()->SetSelfActive(false);
		_killCountObjs[i].first->GetGameObject()->SetSelfActive(false);
		_killCountObjs[i].second->GetGameObject()->SetSelfActive(false);
	}

	API::SetCurrentSceneMainCamera(_endCam->GetComponent<HDData::Camera>());
	SetIsRoundStart(false);
	_endObj->SetSelfActive(true);
	CheckWinner();
}

void RoundManager::InitRound()
{
	for (int i = 0; i < _players.size() + 1; ++i)
	{
		//_backIMG[i]->GetGameObject()->SetSelfActive(true);
		_killCountObjs[i].first->GetGameObject()->SetSelfActive(true);
		_killCountObjs[i].second->GetGameObject()->SetSelfActive(true);
	}

	GameManager::Instance()->GetMyInfo()->SetParticleSystem(_myObj->GetComponentInChildren<HDData::ParticleSystem>());

	_myObj->SetSelfActive(true);

	for (auto& [uid, player] : _players)
	{
		PlayerInfo* info = player->GetComponent<PlayerInfo>();
		info->SetParticleSystem(player->GetComponentInChildren<HDData::ParticleSystem>());
		info->Init();
		player->SetSelfActive(true);
	}
}

void RoundManager::UpdateRound()
{
	UpdateRoundTimer();
	UpdateHPText();
	UpdateAmmoText();
	UpdateDesiredKillChecker();
}

void RoundManager::SetUIActive(bool isActive)
{
	for (int i = 0; i < 6; ++i)
	{
		//_backIMG[i]->GetGameObject()->SetSelfActive(isActive);
		_killCountObjs[i].first->GetGameObject()->SetSelfActive(isActive);
		_killCountObjs[i].second->GetGameObject()->SetSelfActive(isActive);
	}
	_timerUI->GetGameObject()->SetSelfActive(isActive);
	_hpUI->GetGameObject()->SetSelfActive(isActive);
	_ammoUI->GetGameObject()->SetSelfActive(isActive);
	lowHPEffect->GetGameObject()->SetSelfActive(isActive);
	tumbleImage->GetGameObject()->SetSelfActive(isActive);
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
	if (_winnerUID == NULL) return;

	int count = _players.size();

	if (_winnerUID == GameManager::Instance()->GetMyInfo()->GetPlayerUID())
	{
		_winnerTXT->SetText(GameManager::Instance()->GetMyInfo()->GetPlayerNickName());

		int index = 0;
		for (auto& [uid, player] : _players)
		{
			_loserTXT[index]->SetText(player->GetComponent<PlayerInfo>()->GetPlayerNickName());
			++index;
		}
	}
	else
	{
		_winnerTXT->SetText(_players[_winnerUID]->GetComponent<PlayerInfo>()->GetPlayerNickName());

		int index = 0;
		for (auto& [uid, player] : _players)
		{
			if (_winnerUID == uid) continue;
			_loserTXT[index]->SetText(player->GetComponent<PlayerInfo>()->GetPlayerNickName());
			++index;
		}

		_loserTXT[index]->SetText(GameManager::Instance()->GetMyInfo()->GetPlayerNickName());
	}

	_winnerTXT->GetGameObject()->SetSelfActive(true);

	for (int i = 0; i < count; ++i)
	{
		_loserTXT[i]->GetGameObject()->SetSelfActive(true);
	}
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
	API::SetCurrentSceneMainCamera(_startCam);
	_endObj->SetSelfActive(false);
	_showResultTimer->Stop();
	_resultTimerUI->GetGameObject()->SetSelfActive(false);
	NetworkManager::Instance().SendRoomLeave();

	// 로비로 복귀
	API::LoadSceneByName("MainMenu");
	LobbyManager::Instance().RefreshRoom();
	API::GetCubeMap()->LoadCubeMapTexture("Day Sun Peak Clear Gray.dds");
	API::GetCubeMap()->SetEnvLightIntensity(1.0f);
}

void RoundManager::SetWinnerText(HDData::TextUI* txt)
{
	_winnerTXT = txt;
}

void RoundManager::SetLoserText(HDData::TextUI* txt, int index)
{
	_loserTXT[index] = txt;
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

		if (nowElapsed >= 56 && nowElapsed <= 60)
		{
			for (auto& col : _weedColVector)
			{
				col->AddForce(Vector3(2.0f, -1.0f, 0.0f), 5.0f, 0);
			}
		}
		else if (nowElapsed >= 46 && nowElapsed <= 50)
		{
			for (auto& col : _weedColVector)
			{
				col->AddForce(Vector3(-2.0f, -1.0f, 0.0f), 5.0f, 0);
			}
		}
		else if (nowElapsed >= 36 && nowElapsed <= 40)
		{
			for (auto& col : _weedColVector)
			{
				col->AddForce(Vector3(0.0f, -1.0f, 2.0f), 5.0f, 0);
			}
		}
		else if (nowElapsed >= 26 && nowElapsed <= 30)
		{
			for (auto& col : _weedColVector)
			{
				col->AddForce(Vector3(0.0f, -1.0f, -2.0f), 5.0f, 0);
			}
		}
		else if (nowElapsed >= 16 && nowElapsed <= 20)
		{
			for (auto& col : _weedColVector)
			{
				col->AddForce(Vector3(1.0f, -1.0f, 1.0f), 5.0f, 0);
			}
		}
		else if (nowElapsed >= 6 && nowElapsed <= 10)
		{
			for (auto& col : _weedColVector)
			{
				col->AddForce(Vector3(-1.0f, -1.0f, -1.0f), 5.0f, 0);
			}
		}

		if (nowElapsed <= 10)
		{
			_timerUI->SetColor(DirectX::Colors::Red);
			// TODO) 사운드 이펙트 넣기
		}
		if (elapsedTime.count() >= _timer)
		{
			_isRoundStart = false;
			_gameEndTimer->Start();
			finRoundimg->GetGameObject()->GetComponent<UIEffect>()->Play();
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

void RoundManager::UpdateDesiredKillChecker()
{
	{
		int count = GameManager::Instance()->GetMyInfo()->GetPlayerKillCount();
		_inGameKillCounts[GameManager::Instance()->GetMyInfo()->GetPlayerUID()].second->SetText(std::to_string(count));
		if (count >= _nowMaxKill) { _nowMaxKill = count; _winnerUID = GameManager::Instance()->GetMyInfo()->GetPlayerUID(); }
	}

	for (auto& [uid, player] : _players)
	{
		int count = player->GetComponent<PlayerInfo>()->GetPlayerKillCount();
		_inGameKillCounts[uid].second->SetText(std::to_string(count));
		if (count >= _nowMaxKill) { _nowMaxKill = count; _winnerUID = uid; }
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

