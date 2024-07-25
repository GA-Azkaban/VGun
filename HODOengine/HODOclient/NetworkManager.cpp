#include "pch.h"
#include <string>
#include <chrono>
#include "NetworkManager.h"
#include "SoundManager.h"
#include "GameSetting.h"

#include "Service.h"
#include "ServerPacketHandler.h"
#include "ServerSession.h"

#include "RoundManager.h"
#include "LobbyManager.h"
#include "GameManager.h"
#include "MenuManager.h"
#include "GameStruct.h"
#include "ErrorCode.h"
#include "PlayerMove.h"

#include <fstream>

extern int g_argc;
extern WCHAR g_ipAddress[256];
extern int g_port;

NetworkManager& NetworkManager::Instance()
{
	if (_instance == nullptr)
	{
		_instance = new NetworkManager;
	}

	return *_instance;
}

NetworkManager* NetworkManager::_instance = nullptr;

NetworkManager::NetworkManager()
{
	API::CreateStaticComponent(this);
}

NetworkManager::~NetworkManager()
{

}

void NetworkManager::Start()
{
	ServerPacketHandler::Init();

	_service = Horang::MakeShared<Horang::ClientService>(
		Horang::NetAddress(g_ipAddress, g_port),
		Horang::MakeShared<Horang::IocpCore>(),
		Horang::MakeShared<ServerSession>,
		1
	);

	_service->Start();
}

void NetworkManager::Update()
{
	_service->GetIocpCore()->Dispatch(0);

	auto& playerObj = RoundManager::Instance()->GetPlayerObjs();

	if (playerObj.size() == 0) return;

	for (auto& [uid, player] : playerObj)
	{
		auto info = player->GetComponent<PlayerInfo>();
		if (player->GetComponent<PlayerInfo>()->GetIsDie())
		{
			continue;
		}
		Interpolation(player->GetTransform(), info->GetServerPosition(), info->GetServerRotation(), 1.0f);
	}
}

void NetworkManager::RecvPlayShoot(Protocol::PlayerData playerData)
{
	if (GameManager::Instance()->GetMyInfo()->GetPlayerUID() == playerData.userinfo().uid())
	{
		GameManager::Instance()->GetMyInfo()->SetIsShoot(true);
	}
	else
	{
		auto players = RoundManager::Instance()->GetPlayerObjs();
		players[playerData.userinfo().uid()]->GetComponent<PlayerInfo>()->SetIsShoot(true);
		players[playerData.userinfo().uid()]->GetComponent<HDData::AudioSource>()->
			Play3DOnce("3d_fire");
	}
}

void NetworkManager::RecvPlayShoot(Protocol::PlayerData playerData, Protocol::PlayerData hitPlayerData, Protocol::eHitLocation hitLocation)
{
	int myUID = GameManager::Instance()->GetMyInfo()->GetPlayerUID();

	// 쏜 사람 (총구 이펙트만 주면 됨)
	if (myUID == playerData.userinfo().uid())
	{
		GameManager::Instance()->GetMyInfo()->SetIsShoot(true);
		int damage = 34;
		if (hitLocation == Protocol::eHitLocation::HIT_LOCATION_HEAD)
		{
			damage = 100;
		}
		GameManager::Instance()->GetMyInfo()->DisplayDamageLog(hitPlayerData.userinfo().nickname(), damage, hitPlayerData.hp());
	}
	else
	{
		auto players = RoundManager::Instance()->GetPlayerObjs();
		players[playerData.userinfo().uid()]->GetComponent<PlayerInfo>()->SetIsShoot(true);
		players[playerData.userinfo().uid()]->GetComponent<HDData::AudioSource>()->
			Play3DOnce("3d_fire");
	}

	// 맞은 사람 ) 체력 변화
	if (myUID == hitPlayerData.userinfo().uid())
	{
		ConvertDataToPlayerInfo(hitPlayerData,
			GameManager::Instance()->GetMyObject(),
			GameManager::Instance()->GetMyInfo());
		Vector3 enemyPos{ playerData.transform().vector3().x(), playerData.transform().vector3().y(), playerData.transform().vector3().z() };
		GameManager::Instance()->GetMyInfo()->PlayerAttacked(enemyPos);
	}
	else
	{
		ConvertDataToPlayerInfo(hitPlayerData,
			RoundManager::Instance()->GetPlayerObjs()[hitPlayerData.userinfo().uid()],
			RoundManager::Instance()->GetPlayerObjs()[hitPlayerData.userinfo().uid()]->GetComponent<PlayerInfo>());
	}

}

void NetworkManager::RecvPlayKillDeath(Protocol::PlayerData deathPlayerData, Protocol::PlayerData killPlayerData)
{
	int myUID = GameManager::Instance()->GetMyInfo()->GetPlayerUID();

	if (myUID == deathPlayerData.userinfo().uid())
	{
		ConvertDataToPlayerInfo(deathPlayerData,
			GameManager::Instance()->GetMyObject(),
			GameManager::Instance()->GetMyInfo());

		GameManager::Instance()->GetMyInfo()->PlayDieEffect();
		GameManager::Instance()->GetMyInfo()->PlayKillLog(killPlayerData.userinfo().nickname());
		GameManager::Instance()->GetMyInfo()->DisplayKillLog(killPlayerData.userinfo().nickname(), deathPlayerData.userinfo().nickname(), KillLog::KillLogType::ENEMYKILLPLAYER);
	}
	else
	{
		// 모든 데스 갱신
		ConvertDataToPlayerInfo(deathPlayerData,
			RoundManager::Instance()->GetPlayerObjs()[deathPlayerData.userinfo().uid()],
			RoundManager::Instance()->GetPlayerObjs()[deathPlayerData.userinfo().uid()]->GetComponent<PlayerInfo>());
		
		GameManager::Instance()->GetMyObject()->GetComponent<PlayerMove>()->GetOtherPlayerCols()[deathPlayerData.userinfo().uid()]->OnDisable();
		GameManager::Instance()->GetMyInfo()->DisplayKillLog(killPlayerData.userinfo().nickname(), deathPlayerData.userinfo().nickname(), KillLog::KillLogType::ENEMYKILLENEMY);
	}

	if (myUID == killPlayerData.userinfo().uid())
	{
		ConvertDataToPlayerInfo(killPlayerData,
			GameManager::Instance()->GetMyObject(),
			GameManager::Instance()->GetMyInfo());

		GameManager::Instance()->GetMyInfo()->AddSerialKillCount();
		GameManager::Instance()->GetMyInfo()->DisplayKillLog(killPlayerData.userinfo().nickname(), deathPlayerData.userinfo().nickname(), KillLog::KillLogType::PLAYERKILLENEMY);
	}
	else
	{
		// 모든 킬 갱신
		ConvertDataToPlayerInfo(killPlayerData,
			RoundManager::Instance()->GetPlayerObjs()[killPlayerData.userinfo().uid()],
			RoundManager::Instance()->GetPlayerObjs()[killPlayerData.userinfo().uid()]->GetComponent<PlayerInfo>());
	}


}

void NetworkManager::RecvPlayRespawn(Protocol::PlayerData playerData, int32 spawnPointIndex)
{
	if (GameManager::Instance()->GetMyInfo()->GetPlayerUID() == playerData.userinfo().uid())
	{
		// 위치 갱신
		auto pos = API::GetSpawnPointArr()[spawnPointIndex];
		//auto pos = API::GetSpawnPointArr()[1];
		ConvertDataToPlayerInfo(playerData,
			GameManager::Instance()->GetMyObject(),
			GameManager::Instance()->GetMyInfo());

		GameManager::Instance()->GetMyObject()->GetTransform()->SetPosition(pos);
		GameManager::Instance()->GetMyInfo()->SetServerTransform(pos, Quaternion{ 0, 0, 0, 0 });
		GameManager::Instance()->GetMyInfo()->PlayRespawnEffect();
		GameManager::Instance()->GetMyInfo()->KillLogExit();
	}
	else
	{
		auto pos = API::GetSpawnPointArr()[spawnPointIndex];
		//auto pos = API::GetSpawnPointArr()[1];
		ConvertDataToPlayerInfo(playerData,
			RoundManager::Instance()->GetPlayerObjs()[playerData.userinfo().uid()],
			RoundManager::Instance()->GetPlayerObjs()[playerData.userinfo().uid()]->GetComponent<PlayerInfo>());

		auto player = RoundManager::Instance()->GetPlayerObjs()[playerData.userinfo().uid()];
		player->GetTransform()->SetPosition(pos);
		player->GetComponent<PlayerInfo>()->SetServerTransform(pos, Quaternion{ 0, 0, 0, 0 });

		GameManager::Instance()->GetMyObject()->GetComponent<PlayerMove>()->GetOtherPlayerCols()[playerData.userinfo().uid()]->OnEnable();
	}
}

void NetworkManager::SendPlayRoll(Protocol::PlayerData playerData)
{
	Protocol::C_PLAY_ROLL packet;

	packet.mutable_playerdata()->CopyFrom(playerData);

	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(packet);
	this->_service->BroadCast(sendBuffer);
}

void NetworkManager::RecvPlayRoll(Protocol::PlayerData playerData)
{

}

void NetworkManager::SendPlayReload(Protocol::PlayerData playerData)
{
	Protocol::C_PLAY_RELOAD packet;

	packet.mutable_playerdata()->CopyFrom(playerData);

	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(packet);
	this->_service->BroadCast(sendBuffer);
}

void NetworkManager::RecvPlayReload(Protocol::PlayerData playerData)
{

}

void NetworkManager::Connected()
{
	_isConnect = true;

	// #if _DEBUG
	FILE* pFile = nullptr;

	if (AllocConsole())
		freopen_s(&pFile, "CONOUT$", "w", stdout);
	else
		ASSERT_CRASH(false);

	std::cout << "Connected" << std::endl;
	// #endif
}

void NetworkManager::Disconnected()
{
	_isConnect = false;
	std::cout << "Disconnect" << std::endl;
}

void NetworkManager::SendLogin(std::string id, std::string password)
{
	Protocol::C_SIGNIN packet;
	packet.set_id(id);
	packet.set_password(password);

	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(packet);
	this->_service->BroadCast(sendBuffer);
}

void NetworkManager::SendCreateAccount(std::string id, std::string password, std::string nickname)
{
	Protocol::C_SIGNUP packet;
	packet.set_id(id);
	packet.set_password(password);
	packet.set_nickname(nickname);

	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(packet);
	this->_service->BroadCast(sendBuffer);
}

void NetworkManager::RecvFail(int32 errorCode)
{
	// Todo
	// 로그인에 실패했을때 모든 처리를 여기서
	LobbyManager::Instance().LoginFAIL(errorCode);

	// 방 입장에 실패했을 때
	LobbyManager::Instance().RoomEnterFAIL(errorCode);
}

void NetworkManager::SendAutoLogin(std::string nickName)
{
	Protocol::C_AUTOLOGIN packet;

	packet.set_nickname(nickName);

	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(packet);
	this->_service->BroadCast(sendBuffer);
}

void NetworkManager::RecvLogin(int32 uid, std::string nickName)
{
	// remove pyramid
	// 로그인이 성공했을때 처리
	LobbyManager::Instance().LoginSucess(uid, nickName);

	GameManager::Instance()->GetMyInfo()->SetPlayerUID(uid);
	GameManager::Instance()->GetMyInfo()->SetNickName(nickName);
	GameManager::Instance()->GetMyInfo()->SetIsMyInfo(true);

	GameSetting::Instance().SetMyNickname(nickName);

	SoundManager::Instance().PlayUI("sfx_entry");
	API::LoadSceneByName("MainMenu");
}

void NetworkManager::RecvCreateAccount()
{
	LobbyManager::Instance().ShowSignSuccess();
}

void NetworkManager::SendLogout()
{
	// 로그아웃을 보냈다
	Protocol::C_SIGNOUT packet;

	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(packet);
	this->_service->BroadCast(sendBuffer);

	// Todo 보내고 동작을 해야될까?
}

void NetworkManager::RecvLogout()
{
	// Todo 로그아웃을 받아서 동작해야할까?
	API::LoadSceneByName("Login");
}

void NetworkManager::SendRoomListRequest()
{
	Protocol::C_ROOM_LIST_REQUEST packet;

	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(packet);
	this->_service->BroadCast(sendBuffer);
}

void NetworkManager::RecvRoomList(Protocol::S_ROOM_LIST roomList)
{
	auto& list = MenuManager::Instance().GetRoomList();

	list.clear();

	for (auto& room : roomList.roominfo())
	{
		roominfo* info = new roominfo;

		info->id = room.roomid();
		info->title = room.roomname();
		info->maxPlayerCount = room.maxplayercount();
		info->currentPlayerCount = room.currentplayercount();

		info->isPrivate = room.isprivate();
		info->isTeam = room.isteam();

		list.push_back(info);
	}

	MenuManager::Instance().RenderRoomList();
}

void NetworkManager::SendRoomEnter(std::string roomCode, std::string password /*= ""*/)
{
	Protocol::C_ROOM_ENTER packet;
	packet.set_roomcode(roomCode);
	packet.set_password(password);

	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(packet);
	this->_service->BroadCast(sendBuffer);
}

void NetworkManager::SendRoomLeave()
{
	Protocol::C_ROOM_LEAVE packet;

	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(packet);
	this->_service->BroadCast(sendBuffer);
}

void NetworkManager::RecvRoomEnter(Protocol::RoomInfo roomInfo)
{
	//// Todo RoomInfo 설정
	//auto info = LobbyManager::Instance().GetRoomData();

	//info->_players.clear();

	//info->roomid = roomInfo.roomid();

	//info->isPrivate = roomInfo.isprivate();

	//info->roomName = roomInfo.roomname();
	//info->password = roomInfo.password();

	//info->currentPlayerCount = roomInfo.currentplayercount();

	//if (roomInfo.users().empty())
	//{
	//	return;
	//}

	//for (auto& player : roomInfo.users())
	//{
	//	PlayerInfo* one = new PlayerInfo;

	//	one->SetNickName(player.userinfo().nickname());
	//	one->SetIsHost(player.host());
	//	one->SetPlayerUID(player.userinfo().uid());

	//	// 플레이어 정보 받기	
	//	info->_players.push_back(one);
	//}

	LobbyManager::Instance().RoomEnterSUCCESS();
}

void NetworkManager::RecvRoomLeave(Protocol::RoomInfo roomInfo)
{
	GameManager::Instance()->GetMyInfo()->SetIsHost(false);
	API::LoadSceneByName("MainMenu");
}

void NetworkManager::SendRoomCreate(std::string roomName, std::string password /*= ""*/, int32 maxPlayerCount /*= 6*/, bool isPrivate /*= false*/, bool isTeam /*= true*/)
{
	Protocol::C_ROOM_CREATE packet;

	packet.set_roomname(roomName);
	packet.set_password(password);
	packet.set_maxplayercount(maxPlayerCount);
	packet.set_isprivate(isPrivate);
	packet.set_isteam(isTeam);

	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(packet);
	this->_service->BroadCast(sendBuffer);
}

void NetworkManager::SetRoom(Protocol::RoomInfo roomInfo)
{
	Protocol::C_ROOM_CREATE packet;

	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(packet);
	this->_service->BroadCast(sendBuffer);
}

void NetworkManager::SendRoomChat(std::string chat)
{
	Protocol::C_ROOM_CHAT packet;

	packet.set_chat(chat);

	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(packet);
	this->_service->BroadCast(sendBuffer);
}

void NetworkManager::RecvRoomChat(std::string nickName, std::string chat)
{
	// Todo 채팅이 깨질수도 있음


}

void NetworkManager::RecvAnotherPlayerEnter(Protocol::RoomInfo roomInfo)
{
	auto info = LobbyManager::Instance().GetRoomData();

	info->_players.clear();

	for (auto& player : roomInfo.users())
	{
		PlayerInfo* one = new PlayerInfo;
		one->SetNickName(player.userinfo().nickname());
		one->SetIsHost(player.host());
		one->SetPlayerUID(player.userinfo().uid());
		one->SetCurrentHP(player.hp());

		if (player.host() && (GameManager::Instance()->GetMyInfo()->GetPlayerNickName() == player.userinfo().nickname()))
		{
			GameManager::Instance()->GetMyInfo()->SetIsHost(true);
		}

		info->_players.push_back(one);
	}

	LobbyManager::Instance().RefreshRoom();
}

void NetworkManager::RecvAnotherPlayerLeave(Protocol::RoomInfo roomInfo)
{
	auto info = LobbyManager::Instance().GetRoomData();

	info->_players.clear();

	for (auto& player : roomInfo.users())
	{
		PlayerInfo* one = new PlayerInfo;
		one->SetPlayerUID(player.userinfo().uid());
		one->SetNickName(player.userinfo().nickname());
		one->SetIsHost(player.host());
		one->SetCurrentHP(player.hp());

		if (player.host() && (GameManager::Instance()->GetMyInfo()->GetPlayerNickName() == player.userinfo().nickname()))
		{
			GameManager::Instance()->GetMyInfo()->SetIsHost(true);
		}

		info->_players.push_back(one);
	}

	LobbyManager::Instance().RefreshRoom();
}

void NetworkManager::SendKickPlayer(std::string targetNickName)
{
	Protocol::C_ROOM_KICK packet;

	packet.set_targetnickname(targetNickName);

	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(packet);
	this->_service->BroadCast(sendBuffer);
}

void NetworkManager::RecvKickPlayer(Protocol::RoomInfo roomInfo)
{
	auto info = LobbyManager::Instance().GetRoomData();

	info->_players.clear();

	for (auto& player : roomInfo.users())
	{
		PlayerInfo* one = new PlayerInfo;
		one->SetNickName(player.userinfo().nickname());
		one->SetIsHost(player.host());
		one->SetCurrentHP(player.hp());

		info->_players.push_back(one);
	}
}

//void NetworkManager::SendChangeTeamColor(Protocol::eTeamColor teamColor, std::string targetNickName)
//{
//	Protocol::C_ROOM_CHANGE_TEAM packet;
//
//	packet.set_teamcolor(teamColor);
//	packet.set_targetnickname(targetNickName);
//
//	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(packet);
//	this->_service->BroadCast(sendBuffer);
//}

void NetworkManager::SendGameStart()
{
	Protocol::C_ROOM_START packet;

	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(packet);
	this->_service->BroadCast(sendBuffer);
}

void NetworkManager::RecvRoomStart(Protocol::RoomInfo roomInfo, Protocol::GameRule gameRule, int32 spawnpointindex)
{
	// 라운드 초기화
	RoundManager::Instance()->SetIsRoundStart(false);
	RoundManager::Instance()->InitGame();

	// 스폰 포인트로 위치 갱신
	auto pos = API::GetSpawnPointArr()[spawnpointindex];

	GameManager::Instance()->GetMyObject()->GetTransform()->SetPosition(pos);
	GameManager::Instance()->GetMyInfo()->SetServerTransform(pos, Quaternion{ 0, 0, 0, 0 });

	// 씬 로드
	API::LoadSceneByName("InGame");
	API::SetRecursiveMouseMode(true);
	API::ShowWindowCursor(false);
	API::GetCubeMap()->LoadCubeMapTexture("Sunset.dds");
	API::GetCubeMap()->SetEnvLightIntensity(2.0f);

	// Todo roomInfo, gameRule 설정
	RoundManager::Instance()->SetRoundTimer(gameRule.gametime());
	RoundManager::Instance()->SetDesiredKill(gameRule.desiredkill());

	GameManager::Instance()->GetMyObject()->GetComponent<PlayerMove>()->SetIsIngamePlaying(true);
}

void NetworkManager::RecvGameStart()
{
	RoundManager::Instance()->SetIsRoundStart(true);
	RoundManager::Instance()->SetStartTime(std::chrono::steady_clock::now());

	//GameManager::Instance()->GetMyObject()->GetComponent<PlayerMove>()->SetIsIngamePlaying(true);
}

void NetworkManager::RecvGameEnd(Protocol::RoomInfo roomInfo)
{
	API::SetRecursiveMouseMode(false);
	API::ShowWindowCursor(true);
	RoundManager::Instance()->SetIsRoundStart(false);
	RoundManager::Instance()->GetGameEndTimer()->Start();
	GameManager::Instance()->GetMyObject()->GetComponent<PlayerMove>()->SetIsIngamePlaying(false);
	GameManager::Instance()->GetMyObject()->GetComponent<PlayerMove>()->SetMovable(false);
	LobbyManager::Instance().RefreshRoom();
}

void NetworkManager::SendPlayUpdate()
{
	Protocol::C_PLAY_UPDATE packet;

	auto& playerobj = RoundManager::Instance()->_myObj;

	auto vector3 = packet.mutable_playerdata()->mutable_transform()->mutable_vector3();
	vector3->set_x(playerobj->GetTransform()->GetPosition().x);
	vector3->set_y(playerobj->GetTransform()->GetPosition().y);
	vector3->set_z(playerobj->GetTransform()->GetPosition().z);

	auto quaternion = packet.mutable_playerdata()->mutable_transform()->mutable_quaternion();
	quaternion->set_w(playerobj->GetTransform()->GetRotation().w);
	quaternion->set_x(playerobj->GetTransform()->GetRotation().x);
	quaternion->set_y(playerobj->GetTransform()->GetRotation().y);
	quaternion->set_z(playerobj->GetTransform()->GetRotation().z);

	packet.mutable_playerdata()->
		set_animationstate(ConvertStateToEnum(RoundManager::Instance()->GetAnimationDummy()->GetComponent<HDData::Animator>()->GetAllAC()->GetCurrentState()));

	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(packet);
	this->_service->BroadCast(sendBuffer);
}

void NetworkManager::RecvPlayUpdate(Protocol::S_PLAY_UPDATE playUpdate)
{
	auto& playerObjs = RoundManager::Instance()->GetPlayerObjs();
	auto& roominfo = playUpdate.roominfo();

	for (auto& playerData : roominfo.users())
	{
		if (playerData.userinfo().uid() == GameManager::Instance()->GetMyInfo()->GetPlayerUID())
		{
			GameManager::Instance()->GetMyInfo()->SetCurrentHP(playerData.hp());
			continue;
		}

		auto& playerObj = playerObjs[playerData.userinfo().uid()];
		auto playerInfo = playerObj->GetComponent<PlayerInfo>();

		Vector3 pos = {
			playerData.transform().vector3().x(),
			playerData.transform().vector3().y(),
			playerData.transform().vector3().z() };
		Quaternion rot = {
			playerData.transform().quaternion().x(),
			playerData.transform().quaternion().y(),
			playerData.transform().quaternion().z(),
			playerData.transform().quaternion().w() };

		playerInfo->SetServerTransform(pos, rot);
		playerInfo->SetCurrentHP(playerData.hp());

		// Animation

		auto animationState = ConvertAnimationStateToEnum(playerData.animationstate());

		if (playerInfo->GetPlayerState() == animationState)
			continue;

		if (animationState == ePlayerState::IDLE &&
			playerInfo->GetIsInterpolation())
		{
			switch (playerInfo->GetPlayerState())
			{
				case ePlayerState::WALK_R:
				case ePlayerState::WALK_L:
				case ePlayerState::WALK_F:
				case ePlayerState::WALK_B:
				case ePlayerState::WALK:
					playerInfo->SetCurrentState(playerInfo->GetPlayerState());
					break;
				default:
					playerInfo->SetCurrentState(animationState);
					break;
			}
		}
		else
		{
			playerInfo->SetCurrentState(animationState);
		}
	}
}

void NetworkManager::SendPlayJump()
{
	Protocol::C_PLAY_JUMP packet;

	auto& mine = RoundManager::Instance()->_myObj;
	auto info = GameManager::Instance()->GetMyInfo();

	auto data = ConvertPlayerInfoToData(mine, info);
	*packet.mutable_playerdata() = data;

	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(packet);
	this->_service->BroadCast(sendBuffer);
}

void NetworkManager::RecvPlayJump(Protocol::PlayerData playerData)
{
	if (playerData.userinfo().uid() == GameManager::Instance()->GetMyInfo()->GetPlayerUID()) return;
	RoundManager::Instance()->GetPlayerObjs()[playerData.userinfo().uid()]->GetComponent<PlayerInfo>()->SetIsJump(true);
}

void NetworkManager::SendPlayShoot(HDData::Transform* transform, uint64 hitTargetUid /*= 0*/, Protocol::eHitLocation hitLocation /*= Protocol::eHitLocation::HIT_LOCATION_NO_HIT*/)
{
	Protocol::C_PLAY_SHOOT packet;

	auto pos = packet.mutable_transform()->mutable_vector3();
	pos->set_x(transform->GetPosition().x);
	pos->set_y(transform->GetPosition().y);
	pos->set_z(transform->GetPosition().z);

	auto rot = packet.mutable_transform()->mutable_quaternion();
	rot->set_w(transform->GetRotation().w);
	rot->set_x(transform->GetRotation().x);
	rot->set_y(transform->GetRotation().y);
	rot->set_z(transform->GetRotation().z);

	packet.set_hittargetuid(hitTargetUid);
	packet.set_hitlocation(hitLocation);

	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(packet);
	this->_service->BroadCast(sendBuffer);
}

bool NetworkManager::IsConnected()
{
	return _isConnect;
}

Protocol::PlayerData NetworkManager::ConvertPlayerInfoToData(HDData::GameObject* mine, PlayerInfo* info)
{
	Protocol::PlayerData data;

	data.mutable_transform()->mutable_vector3()->set_x(mine->GetTransform()->GetPosition().x);
	data.mutable_transform()->mutable_vector3()->set_y(mine->GetTransform()->GetPosition().y);
	data.mutable_transform()->mutable_vector3()->set_z(mine->GetTransform()->GetPosition().z);

	data.mutable_transform()->mutable_quaternion()->set_x(mine->GetTransform()->GetRotation().x);
	data.mutable_transform()->mutable_quaternion()->set_y(mine->GetTransform()->GetRotation().y);
	data.mutable_transform()->mutable_quaternion()->set_z(mine->GetTransform()->GetRotation().z);
	data.mutable_transform()->mutable_quaternion()->set_w(mine->GetTransform()->GetRotation().w);

	data.set_host(info->GetIsHost());

	//data.set_animationstate(ConvertStateToEnum(RoundManager::Instance()->GetAnimationDummy()->GetComponent<HDData::Animator>()->GetAllAC()->GetCurrentState()));

	data.mutable_userinfo()->set_uid(info->GetPlayerUID());
	data.mutable_userinfo()->set_nickname(info->GetPlayerNickName());

	return data;
}

void NetworkManager::ConvertDataToPlayerInfo(Protocol::PlayerData data, HDData::GameObject* mine, PlayerInfo* info)
{
	mine->GetTransform()->SetPosition(data.transform().vector3().x(), data.transform().vector3().y(), data.transform().vector3().z());
	mine->GetTransform()->SetRotation(data.transform().quaternion().x(), data.transform().quaternion().y(), data.transform().quaternion().z(), data.transform().quaternion().w());

	info->SetCurrentKill(data.killcount());
	info->SetCurrentDeath(data.deathcount());
	info->SetCurrentHP(data.hp());
	info->SetIsDie(data.isdead());
	//info->SetCurrentState(ConvertAnimationStateToEnum(data.animationstate()));
}

void NetworkManager::Interpolation(HDData::Transform* current, Vector3 serverPos, Quaternion serverRot, float intermediateValue)
{
	auto dt = API::GetDeltaTime();

	Vector3 currentPos = current->GetPosition();
	Quaternion currentRot = current->GetRotation();

	//if (currentPos == serverPos && currentRot == serverRot)
	//	return;
	float speed = 8.4f;

	Vector3 posDif = serverPos - currentPos;
	//Vector3 nomal = {posDif.x / posDif.Length(), 0.0f, posDif.z / posDif.Length()};
	////nomal.Normalize();

	//auto directionVector = nomal * speed * dt;
	auto plInfo = current->GetGameObject()->GetComponent<PlayerInfo>();

	float dif = posDif.Length();
	if (plInfo->GetPlayerState() == ePlayerState::IDLE)
	{
		if (dif > 0.1f)
		{
			current->SetPosition(serverPos);
		}
	}
	else
	{
		if (dif > 0.001f)
		{
			current->SetPosition(serverPos);
		}
	}
	//float dif = posDif.Length();
	//auto plMove = current->GetGameObject()->GetComponent<PlayerMove>();
	//auto plInfo = current->GetGameObject()->GetComponent<PlayerInfo>();

	//if (dif > 0.01f && plMove->GetPlayerMoveEnum(1) != ePlayerMoveState::IDLE)
	//if (plInfo->GetPlayerState() == ePlayerState::IDLE)
	//{
	//	if (dif > 0.1f)
	//	{
	//		current->SetPosition(serverPos);
	//	}
	//}
	//else
	//{
	//	if (dif > 0.001f)
	//	{
	//		current->SetPosition(serverPos);
	//	}
	//}
	 
	//if (posDif.Length() > 15.0f)
	//{
	//	current->SetPosition(serverPos);
	//}
	//else if (posDif.Length() > 10.0f)
	//{
	//	// current->Translate(directionVector * 2);
	//	current->SetPosition(currentPos + directionVector * 2);
	//	//current->SetPosition(serverPos);

	//	current->GetGameObject()->GetComponent<PlayerInfo>()->SetIsInterpolation(true);
	//}
	//else if (posDif.Length() > directionVector.Length())
	//{
	//	// current->Translate(directionVector);
	//	current->SetPosition(currentPos + directionVector);
	//	//current->SetPosition(serverPos);

	//	current->GetGameObject()->GetComponent<PlayerInfo>()->SetIsInterpolation(true);
	//}
	//else
	//{
	//	//current->SetPosition(serverPos);

	//	current->GetGameObject()->GetComponent<PlayerInfo>()->SetIsInterpolation(false);
	//}
	
	//////
	//class CustomQueue
	//{
	//public:
	//	void push(const std::pair<Vector3, Vector3>& value) { m_queue.push(value); }
	//	void pop() { if (!m_queue.empty()) { m_queue.pop(); } }
	//	std::pair<Vector3, Vector3>& front() { return m_queue.front(); }
	//	bool empty() const { return m_queue.empty(); }
	//	std::size_t size() const { return m_queue.size(); }
	//	void printQueue() const
	//	{
	//		std::queue<std::pair<Vector3, Vector3>> tempQueue = m_queue;
	//		while (!tempQueue.empty())
	//		{
	//			std::cout.fixed;
	//			std::cout.precision(6);
	//			std::cout <<
	//				tempQueue.front().first.x << " / \t" <<
	//				tempQueue.front().first.y << " / \t" <<
	//				tempQueue.front().first.z <<
	//				" \t|\t" <<
	//				tempQueue.front().second.x << " / \t" <<
	//				tempQueue.front().second.y << " / \t" <<
	//				tempQueue.front().second.z;
	//			//std::cout << std::endl;
	//			tempQueue.pop();
	//		}
	//	}
	//	~CustomQueue()
	//	{
	//		printQueue();
	//	}
	//private:
	//	std::queue<std::pair<Vector3, Vector3>> m_queue;
	//};

	//static CustomQueue posQueue;
	//if (posQueue.size() > 1)
	//	posQueue.pop();
	//posQueue.push({ currentPos,serverPos });

	////std::cout << std::endl;
	//posQueue.printQueue();
	//std::cout << "\t" << directionVector.Length() << std::endl;
	/////////

	float dot = serverRot.Dot(currentRot);
	float angleDif = 2.0f * acos(dot);
	if (angleDif > 0.01f)
	{
		// 로테이션 구면 선형 보간
		Quaternion interpolatedRot = Quaternion::Slerp(currentRot, serverRot, dt * intermediateValue * 10);

		// 현재 Transform에 보간된 값 설정
		current->SetRotation(interpolatedRot);
	}
	else
	{
		current->SetRotation(serverRot);
	}
}

Protocol::eAnimationState NetworkManager::ConvertStateToEnum(const std::string& state)
{
	if (state == "IDLE")
	{
		return Protocol::eAnimationState::ANIMATION_STATE_IDLE;
	}
	else if (state == "RUN_R")
	{
		return Protocol::eAnimationState::ANIMATION_STATE_RIGHT;
	}
	else if (state == "RUN_L")
	{
		return Protocol::eAnimationState::ANIMATION_STATE_LEFT;
	}
	else if (state == "RUN_F")
	{
		return Protocol::eAnimationState::ANIMATION_STATE_FORWARD;
	}
	else if (state == "RUN_B")
	{
		return Protocol::eAnimationState::ANIMATION_STATE_BACK;
	}
	else if (state == "FIRE")
	{
		return Protocol::eAnimationState::ANIMATION_STATE_SHOOT;
	}
	else if (state == "JUMP")
	{
		return Protocol::eAnimationState::ANIMATION_STATE_JUMP;
	}
	else if (state == "ROLL_F")
	{
		return Protocol::eAnimationState::ANIMATION_STATE_ROLL_FORWARD;
	}
	else if (state == "ROLL_B")
	{
		return Protocol::eAnimationState::ANIMATION_STATE_ROLL_BACK;
	}
	else if (state == "ROLL_R")
	{
		return Protocol::eAnimationState::ANIMATION_STATE_ROLL_RIGHT;
	}
	else if (state == "ROLL_L")
	{
		return Protocol::eAnimationState::ANIMATION_STATE_ROLL_LEFT;
	}
	else if (state == "RELOAD")
	{
		return Protocol::eAnimationState::ANIMATION_STATE_RELOAD;
	}
	else if (state == "DIE")
	{
		return Protocol::eAnimationState::ANIMATION_STATE_DEATH;
	}
	else
	{
		return Protocol::eAnimationState::ANIMATION_STATE_NONE;
	}
}

ePlayerState NetworkManager::ConvertAnimationStateToEnum(Protocol::eAnimationState state)
{
	switch (state)
	{
		case Protocol::ANIMATION_STATE_NONE:
		{
			return ePlayerState::NONE;
		}
		break;
		case Protocol::ANIMATION_STATE_SHOOT:
		{
			return ePlayerState::FIRE;
		}
		break;
		case Protocol::ANIMATION_STATE_IDLE:
		{
			return ePlayerState::IDLE;
		}
		break;
		case Protocol::ANIMATION_STATE_FORWARD:
		{
			return ePlayerState::WALK_F;
		}
		break;
		case Protocol::ANIMATION_STATE_BACK:
		{
			return ePlayerState::WALK_B;
		}
		break;
		case Protocol::ANIMATION_STATE_LEFT:
		{
			return ePlayerState::WALK_L;
		}
		break;
		case Protocol::ANIMATION_STATE_RIGHT:
		{
			return ePlayerState::WALK_R;
		}
		break;
		case Protocol::ANIMATION_STATE_JUMP:
		{
			return ePlayerState::JUMP;
		}
		break;
		case Protocol::ANIMATION_STATE_ROLL_FORWARD:
		{
			return ePlayerState::ROLL_F;
		}
		break;
		case Protocol::ANIMATION_STATE_ROLL_BACK:
		{
			return ePlayerState::ROLL_B;
		}
		break;
		case Protocol::ANIMATION_STATE_ROLL_RIGHT:
		{
			return ePlayerState::ROLL_R;
		}
		break;
		case Protocol::ANIMATION_STATE_ROLL_LEFT:
		{
			return ePlayerState::ROLL_L;
		}
		break;
		case Protocol::ANIMATION_STATE_RELOAD:
		{
			return ePlayerState::RELOAD;
		}
		break;
		case Protocol::ANIMATION_STATE_DEATH:
		{
			return ePlayerState::DIE;
		}
		break;
		default:
		{
			return ePlayerState::IDLE;
		}
		break;
	}
}

