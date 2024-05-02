#include "RoundManager.h"
#include "NetworkManager.h"

RoundManager::RoundManager()
{

}

void RoundManager::Start()
{
	InitGame();
}

void RoundManager::Update()
{

}

void RoundManager::InitGame()
{
	// 라운드 초기화
	InitRound();
	NetworkManager::Instance().SendGameStart();

}

void RoundManager::EndGame()
{
	//NetworkManager::Instance().SendPlayUpdate();
}

void RoundManager::InitRound()
{
	// 타이머 초기화
	this->_timer = 0;

	// 플레이어 정보 초기화
	for (auto& player : _playerInfo)
	{
		player->Init();
	}
}

void RoundManager::UpdateRound()
{
	// 플레이어 transform 정보 갱신

	// 플레이어 state 정보 갱신

	// 플레이어 killcount, time 갱신

	// 플레이어 상태 (체력, 남은 총알 수, 위치) 를 서버에서 받아와 갱신
}

void RoundManager::InitRandomSpawn()
{
	auto spawn1 = API::CreateStaticObject();
	auto spawn2 = API::CreateStaticObject();
	auto spawn3 = API::CreateStaticObject();
	auto spawn4 = API::CreateStaticObject();
	auto spawn5 = API::CreateStaticObject();
	auto spawn6 = API::CreateStaticObject();
	auto spawn7 = API::CreateStaticObject();
	auto spawn8 = API::CreateStaticObject();
	auto spawn9 = API::CreateStaticObject();
	auto spawn10 =API::CreateStaticObject();

	_randomSpawnPos.push_back(spawn1);
	_randomSpawnPos.push_back(spawn2);
	_randomSpawnPos.push_back(spawn3);
	_randomSpawnPos.push_back(spawn4);
	_randomSpawnPos.push_back(spawn5);
	_randomSpawnPos.push_back(spawn6);
	_randomSpawnPos.push_back(spawn7);
	_randomSpawnPos.push_back(spawn8);
	_randomSpawnPos.push_back(spawn9);
	_randomSpawnPos.push_back(spawn10);

	for (auto& spawn : _randomSpawnPos)
	{
		//spawn->AddComponent<HDData::Colli>()
	}
}

void RoundManager::SetPlayerSpawn(int playerIndex)
{
	for (auto& player : _playerInfo)
	{
		if(player->GetPlayerIndex() != playerIndex) continue;

		for (auto& spawn : _randomSpawnPos)
		{
			
		}
	}
}

