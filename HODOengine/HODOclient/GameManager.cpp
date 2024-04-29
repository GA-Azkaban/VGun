#include "GameManager.h"

GameManager* GameManager::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new GameManager;
	}

	return instance;
}

GameManager* GameManager::instance = nullptr;

GameManager::GameManager()
{

}

void GameManager::Start()
{

}

void GameManager::Update()
{

}

void GameManager::InitGame()
{
	// 라운드 초기화
	InitRound();


}

void GameManager::EndGame()
{

}

void GameManager::InitRound()
{
	// 타이머 초기화
	// 각 팀당 킬카운트 초기화

	// 플레이어 정보 초기화
}

void GameManager::UpdateRound()
{
	// 플레이어 transform 정보 갱신

	// 플레이어 state 정보 갱신

	// 플레이어 killcount, time 갱신

	// 플레이어 상태 (체력, 남은 총알 수, 위치) 를 서버에서 받아와 갱신
}

