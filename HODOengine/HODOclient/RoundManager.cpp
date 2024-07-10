#include "RoundManager.h"
#include "NetworkManager.h"
#include "LobbyManager.h"
#include "PlayerMove.h"
#include "GameManager.h"
#include "MenuManager.h"
#include "FPAniScript.h"
#include "MeshTransformController.h"
#include "CameraMove.h"

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
	
	_showResultTimer = new Timer;
	_showResultTimer->duration = 10;
	_showResultTimer->onExpiration = [&]() {
		ExitGame();
		};
}

void RoundManager::Update()
{
	UpdateResultTimer();

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

	_timerUI->GetGameObject()->SetSelfActive(true);
	_hpUI->GetGameObject()->SetSelfActive(true);
	_ammoUI->GetGameObject()->SetSelfActive(true);

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
			_inGameKillCounts.insert({ info->GetPlayerUID(), _killCountObjs[index] });
		}
		else
		{
			_playerObjs[index]->AddComponent<PlayerInfo>(info);
			_players.insert({ info->GetPlayerUID(), _playerObjs[index] });
			_killCountObjs[index].first->SetText(info->GetPlayerNickName());
			_inGameKillCounts.insert({ info->GetPlayerUID(), _killCountObjs[index] });
		}

		++index;
	}

	InitRound();
}

void RoundManager::EndGame()
{
	// UI 활성화, 비활성화
	_timerUI->GetGameObject()->SetSelfActive(false);
	_hpUI->GetGameObject()->SetSelfActive(false);
	_ammoUI->GetGameObject()->SetSelfActive(false);

	for (int i = 0; i < 6; ++i)
	{
		_backIMG[i]->GetGameObject()->SetSelfActive(false);
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
		_backIMG[i]->GetGameObject()->SetSelfActive(true);
		_killCountObjs[i].first->GetGameObject()->SetSelfActive(true);
		_killCountObjs[i].second->GetGameObject()->SetSelfActive(true);
	}

	HDData::SkinnedMeshRenderer* mesh = nullptr;
	mesh = _myObj->GetGameObjectByNameInChildren("meshShell")->GetComponentInChildren<HDData::SkinnedMeshRenderer>();

	_myObj->GetComponent<PlayerInfo>()->Init();
	_myObj->SetSelfActive(true);

	for (auto& [uid, player] : _players)
	{
		player->GetComponent<PlayerInfo>()->Init();
		player->SetSelfActive(true);

		HDData::SkinnedMeshRenderer* mesh = nullptr;

		PlayerInfo* info = player->GetComponent<PlayerInfo>();
		mesh = player->GetComponentInChildren<HDData::SkinnedMeshRenderer>();
	}
}

void RoundManager::UpdateRound()
{
	UpdateRoundTimer();
	UpdateHPText();
	UpdateAmmoText();
	UpdateDesiredKillChecker();
}

void RoundManager::CheckHeadColliderOwner(HDData::DynamicSphereCollider* collider)
{
	int uid = collider->GetParentCollider()->GetGameObject()->GetComponent<PlayerInfo>()->GetPlayerUID();

	NetworkManager::Instance().SendPlayShoot(collider->GetTransform(), uid, Protocol::HIT_LOCATION_HEAD);
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
			if(_winnerUID == uid) continue;
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
	_myObj->GetComponent<PlayerMove>()->SetMovable(isStart);
	_isRoundStart = isStart;
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

	// 로비로 복귀
	API::LoadSceneByName("Lobby");
	LobbyManager::Instance().RefreshRoom();
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
	_timerUI->GetTransform()->SetPosition(1350.0f, 60.0f, 0.0f);
}

void RoundManager::SetRoundTimer(int time)
{
	_timer = time;
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

		if (nowElapsed <= 10)
		{
			_timerUI->SetColor(DirectX::Colors::Red);
			// TODO) 사운드 이펙트 넣기
		}
		if (elapsedTime.count() >= _timer)
		{
			_isRoundStart = false;
			_gameEndTimer->Start();
		
		}
	}
}

std::string RoundManager::ChangeSecToMin(int second)
{
	int min = second / 60;
	int sec = second % 60;

	std::string result = std::to_string(min) + " : " + std::to_string(sec);

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

void RoundManager::UpdateResultTimer()
{
	if (API::GetCurrentSceneName() != "InGame") return;

	_gameEndTimer->Update();
	_showResultTimer->Update();

	if (!_showResultTimer->IsActive()) return;

	_resultTimerUI->SetText("Quit by..." + std::to_string(static_cast<int>(_showResultTimer->duration - _showResultTimer->GetElapsedTime())));
}

void RoundManager::SetResultTimerUI(HDData::TextUI* txt)
{
	_resultTimerUI = txt;
}

Timer* RoundManager::GetGameEndTimer()
{
	return _gameEndTimer;
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

void RoundManager::SetKillCountBack(HDData::ImageUI* img, int index)
{
	_backIMG[index] = img;
}

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

