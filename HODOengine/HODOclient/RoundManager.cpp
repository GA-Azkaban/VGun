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
	spawn1->GetTransform()->SetPosition(-28.57, 0, -19.55);

	auto spawn2 = API::CreateStaticObject();
	spawn2->GetTransform()->SetPosition(8.99, 0, -18.78);

	auto spawn3 = API::CreateStaticObject();
	spawn3->GetTransform()->SetPosition(13.98, 0, -4.55);

	auto spawn4 = API::CreateStaticObject();
	spawn4->GetTransform()->SetPosition(11.54, 2.742565, 3.72);

	auto spawn5 = API::CreateStaticObject();
	spawn5->GetTransform()->SetPosition(13.72, 2.194, 20.38);

	auto spawn6 = API::CreateStaticObject();
	spawn6->GetTransform()->SetPosition(-25.19, 2.742565, 12.47);

	auto spawn7 = API::CreateStaticObject();
	spawn7->GetTransform()->SetPosition(-36.57, 2.742565, 14.91);

	auto spawn8 = API::CreateStaticObject();
	spawn8->GetTransform()->SetPosition(-28.2, 0, 0.97);

	auto spawn9 = API::CreateStaticObject();
	spawn9->GetTransform()->SetPosition(-11.98, 0, 0.3);

	auto spawn10 =API::CreateStaticObject();
	spawn10->GetTransform()->SetPosition(-4.69, 0, -19.01);

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
		auto collider = spawn->AddComponent<HDData::TriggerBoxCollider>(10.f, 10.f, 10.f);
	}
}

void RoundManager::SetPlayerSpawn(int playerIndex)
{
	for (auto& player : _playerInfo)
	{
		if(player->GetPlayerIndex() != playerIndex) continue;

		for (auto& spawn : _randomSpawnPos)
		{
			auto isCollided = spawn->GetComponent<HDData::TriggerBoxCollider>()->GetIsCollided();
			
			if (!isCollided)
			{
				player->GetTransform()->SetPosition(spawn->GetTransform()->GetPosition());
			}
		}
	}
}

