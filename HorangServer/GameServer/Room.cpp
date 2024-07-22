#include "pch.h"
#include "Room.h"
#include "Player.h"
#include "GameSession.h"
#include "ClientPacketHandler.h"
#include "RoomManager.h"
#include "JobTimer.h"
#include "Log.h"
#include <random>

extern int g_gameTime;

using namespace Horang;

Room::Room()
{
	this->Initialize();
}

Room::~Room()
{
	Horang::DebugLog() << L"NnNNNNNNNNNNNNNNNNNNNN";
	this->Initialize();
}

void Room::Initialize(int32 roomId /*= 0*/, std::string roomCode /*= "0000"*/)
{
	this->_roomId = roomId;
	this->_roomCode = roomCode;
	this->_state = Protocol::eRoomState::ROOM_STATE_NONE;

	for (auto& [uid, user] : _players)
	{
		auto player = user.player.lock();
		if (player == nullptr)
			continue;

		player->ownerGameSession->_room.reset();
	}

	this->_players.clear();

	this->_roomName = "Init";
	this->_password = "Init";
	this->_maxPlayerCount = 6;
	this->_isPrivate = false;
	this->_isTeam = false;
	this->_gameStartTime = 0;

	_gameRule.Clear();
}

bool Room::Enter(PlayerWeakRef playerWeak, std::string password)
{
	LogBuffer log("Enter");

	auto player = playerWeak.lock();
	if (player == nullptr)
	{
		log.PlayerNullptr();
		return false;
	}

	if (player->ownerGameSession->_room.expired() == false)
	{
		log << "Already In Room";
		return false;
	}

	if (password != this->_password)
	{
		log << "Password Incorrect";
		player->ownerGameSession->SendError(ErrorCode::ROOM_PASSWORD_INCORRECT);
		return false;
	}

	// 게임중
	if (_state != Protocol::eRoomState::ROOM_STATE_LOBBY)
	{
		player->ownerGameSession->SendError(ErrorCode::ROOM_PLAYING);

		log << "Room Playing";
		return false;
	}

	// 인원 초과
	if (_players.size() > 6)
	{
		player->ownerGameSession->SendError(ErrorCode::ROOM_FULL);

		log << "Room Full";

		return false;
	}

	log << "Count : " << _players.size();
	log << "Player : " << player->nickname;

	_players[player->uid] = { player, Protocol::PlayerData() };

	player->ownerGameSession->_room = this->GetWeakRef();

	auto& playerData = _players[player->uid].data;
	auto userInfo = playerData.mutable_userinfo();
	player->GetUserInfo(userInfo);

	// 혼자일때 방장 권한
	if (_players.size() == 1)
		playerData.set_host(true);

	auto roomInfo = this->GetRoomInfo();
	// 접속한 사람에게 방 정보 보내주기
	{
		Protocol::S_ROOM_ENTER packet;
		packet.mutable_roominfo()->CopyFrom(roomInfo);

		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(packet);
		player->ownerGameSession->Send(sendBuffer);
	}

	// 기존 방 사람들에게 보냄
	{
		Protocol::S_ANOTHER_ENTER_ROOM packet;
		packet.mutable_roominfo()->CopyFrom(roomInfo);

		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(packet);
		this->BroadCast(sendBuffer);
	}

	log << "Success";

	return true;
}

bool Room::Leave(PlayerWeakRef playerWeak, int32 uid)
{
	LogBuffer log("Leave");

	auto player = playerWeak.lock();

	// Todo 1명만 남았을때 방 삭제
	if (this->_players.size() <= 1)
	{
		GRoomManager->Push(Horang::MakeShared<DestroyRoomJob>(this->GetWeakRef()));
		log << "Destroy Room";

		if (player != nullptr)
		{
			Protocol::S_ROOM_LEAVE packet;

			auto sendBuffer = ClientPacketHandler::MakeSendBuffer(packet);
			player->ownerGameSession->Send(sendBuffer);
		}

		return true;
	}

	if (player == nullptr)
	{
		log.PlayerNullptr();
		this->_players.erase(uid);
		return false;
	}

	// 방장이 나갔을때
	if (this->_players[player->uid].data.host() == true)
	{
		log << "Host Leave";
		for (auto& [uid, p] : this->_players)
		{
			if (p.data.host() == false)
			{
				p.data.set_host(true);
				log << "New Host : " << p.data.userinfo().nickname();
				break;
			}
		}
		this->_players[player->uid].data.set_host(false);
	}

	this->_players.erase(player->uid);
	player->ownerGameSession->_room.reset();

	log << "Player : " << player->nickname;

	// 나간거 다른 사람에게 알려주기
	{
		Protocol::S_ANOTHER_LEAVE_ROOM packet;
		auto roomInfo = packet.mutable_roominfo();
		this->GetRoomInfo(roomInfo);

		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(packet);
		this->BroadCast(sendBuffer);
	}

	{
		Protocol::S_ROOM_LEAVE packet;

		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(packet);
		player->ownerGameSession->Send(sendBuffer);
	}

	log << "Success";

	return true;
}

void Room::BroadCast(Horang::SendBufferRef sendBuffer)
{
	for (auto& [id, playerData] : _players)
	{
		auto player = playerData.player.lock();
		// Todo dummy 고려한 코드
		if (player == nullptr)
			continue;

		player->ownerGameSession->Send(sendBuffer);
		//playerData.player.lock()->ownerGameSession->Send(sendBuffer);
	}
}

void Room::ClientUpdate(PlayerWeakRef playerWeak, Protocol::C_PLAY_UPDATE& pkt)
{
	auto playerRef = playerWeak.lock();
	if (playerRef == nullptr)
		return;

	// 방 상태가 게임중이 아닐때
	if (this->_state != Protocol::eRoomState::ROOM_STATE_PLAY)
		return;

	// 플레이어가 방에 존재하지 않을때
	if (_players.find(playerRef->uid) == _players.end())
		return;

	auto& player = _players[playerRef->uid].data;
	const auto& playerData = pkt.playerdata();

	// Todo 모두 받도록 처리하기
	// player 정보 업데이트
	//_players[player->uid].data.CopyFrom(pkt.playerdata());

	player.mutable_transform()->CopyFrom(playerData.transform());
	player.set_issitting(playerData.issitting());
	player.set_animationstate(playerData.animationstate());
}

void Room::RoomStart(PlayerWeakRef playerWeak)
{
	LogBuffer log("GameStart");

	auto player = playerWeak.lock();
	if (player == nullptr)
	{
		log.PlayerNullptr();
		return;
	}

	if (this->_players[player->uid].data.host() == false)
	{
		log << "Not Host";

		// 방장이 아닐때 에러 보내기
		player->ownerGameSession->SendError(ErrorCode::ROOM_NOT_HOST);
		return;
	}

	if (this->_state == Protocol::eRoomState::ROOM_STATE_END)
	{
		log << "Ending";

		// 게임이 끝내는 중일때
		player->ownerGameSession->SendError(ErrorCode::ROOM_ENDING);
		return;
	}

	// Todo Debug용 인원수 제한 해제
	/*if (_players.size() < 2)
	{
		log << "Not Enough Player";

		// 인원수 부족시 에러 보내기
		player->ownerGameSession->SendError(ErrorCode::ROOM_NOT_ENOUGH_PLAYER);
		return;
	}*/


	log << "Player : " << player->nickname;
	log << "Count : " << _players.size();

	this->_state = Protocol::eRoomState::ROOM_STATE_PLAY;

	// 룰 설정 // Todo DB로 교체될수도 있음
	_gameRule.set_gametime(g_gameTime); // 120
	//_gameRule.set_gametime(g_gameTime); // 120
	_gameRule.set_desiredkill(30); // 30
	_gameRule.set_maxhp(100); // 100
	_gameRule.set_respawntime(2); // 2 or 3
	_gameRule.set_spawnindexrange(std::max(6, 17)); // 9

	// Todo 시작지점 설정
	// Todo 플레이어 위치 설정
	// Todo 플레이어 상태 설정
	for (auto& [uid, playerData] : _players)
	{
		auto& player = playerData.data;

		player.set_hp(static_cast<float>(_gameRule.maxhp()));
		player.set_issitting(false);
		player.set_isdead(false);
		player.set_killcount(0);
		player.set_deathcount(0);
		player.set_animationstate(Protocol::ANIMATION_STATE_NONE);
	}

	{
		Protocol::S_ROOM_START packet;
		this->GetRoomInfo(packet.mutable_roominfo());
		packet.mutable_gamerule()->CopyFrom(_gameRule);

		// 1부터 spawnindexrange까지 랜덤으로 숫자를 뽑아서 플레이어 위치 설정
		std::vector<int32> spawnIndex;
		for (int32 i = 1; i <= _gameRule.spawnindexrange(); i++)
			spawnIndex.push_back(i);

		std::random_device rd;
		std::mt19937 g(rd());

		std::shuffle(spawnIndex.begin(), spawnIndex.end(), g);

		int32 index = 0;
		for (auto& [uid, playerData] : _players)
		{
			packet.set_spawnpointindex(spawnIndex[index++]);

			auto sendBuffer = ClientPacketHandler::MakeSendBuffer(packet);
			playerData.player.lock()->ownerGameSession->Send(ClientPacketHandler::MakeSendBuffer(packet));
		}
	}

	/*{
		Protocol::S_ROOM_START packet;
		this->GetRoomInfo(packet.mutable_roominfo());
		packet.mutable_gamerule()->CopyFrom(_gameRule);

		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(packet);
		this->BroadCast(sendBuffer);
	}*/

	// Todo 시간 바꾸기
	{
		auto job = Horang::MakeShared<GameStartJob>(this->GetSharedRef());
		GJobTimer->Reserve(3000, this->weak_from_this(), job);
	}

	log << "Success";
}

void Room::GameStart()
{
	LogBuffer log("GameStart");

	if (this->_state != Protocol::eRoomState::ROOM_STATE_PLAY)
	{
		log << "Not Playing";
		return;
	}

	_gameStartTime = ::GetTickCount64();

	// Todo Debug
	_lastUpdateTime = ::GetTickCount64();

	{
		Protocol::S_GAME_START packet;

		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(packet);
		this->BroadCast(sendBuffer);
	}

	// Update 시작
	{
		log << "Update Start";

		Horang::JobRef job = Horang::MakeShared<UpdateJob>(this->GetSharedRef());
		this->Push(job);
	}

	// End 넣어두기
	{
		log << "Push End" << _gameRule.gametime() * 1000;

		auto job = Horang::MakeShared<GameEndJob>(this->GetSharedRef(), false, _gameStartTime);

		GJobTimer->Reserve(_gameRule.gametime() * 1000, this->weak_from_this(), job);
	}

	log << "Success";
}

void Room::Update()
{
	if (this->_state != Protocol::eRoomState::ROOM_STATE_PLAY)
	{
		LogBuffer log("Update Return");
		return;
	}

#ifdef _DEBUG
	// Todo Debug
	if (_lastUpdateTime < ::GetTickCount64())
	{
		LogBuffer log("Update");
		log << "Room : " << this->_roomCode;
		log << "Player Count : " << this->_players.size();

		log << "\n";
		for (auto& [uid, playerData] : _players)
		{
			log << playerData.data.userinfo().nickname();
			log << " : " <<
				playerData.data.transform().vector3().x() << " : " << playerData.data.transform().vector3().y() << " : " << playerData.data.transform().vector3().z();
		}

		if (_lastUpdateTime + 32 < ::GetTickCount64())
		{
			log << "Over Time";
		}

		_lastUpdateTime = ::GetTickCount64() + 10000;
	}
#endif // _DEBUG

	// Update후 player에게 보내기
	{
		Protocol::S_PLAY_UPDATE packet;
		this->SetUpdatePacket(packet);

		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(packet);
		this->BroadCast(sendBuffer);
	}

	// 다시 Update Job 만들어서 대기열에 넣기
	{
		auto job = Horang::MakeShared<UpdateJob>(this->GetSharedRef());

		GJobTimer->Reserve(7, this->weak_from_this(), job);
	}
}

void Room::GameEnd(bool forceEnd, uint64 startTime)
{
	LogBuffer log("GameEnd");

	log << this->_roomCode;

	if (this->_state != Protocol::eRoomState::ROOM_STATE_PLAY)
	{
		log << "Not Playing";
		return;
	}

	// 시작시간이 다름 = 이미 끝났거다 다른 게임
	if (forceEnd == false && startTime != this->_gameStartTime)
	{
		log << "Invalid Game";
		return;
	}

	// 게임 종료 처리

	this->_gameStartTime = 0;

	this->_state = Protocol::eRoomState::ROOM_STATE_END;
	{
		Protocol::S_GAME_END packet;
		this->GetRoomInfo(packet.mutable_roominfo());

		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(packet);
		this->BroadCast(sendBuffer);
	}

	{
		auto job = Horang::MakeShared<GameEndToLobbyJob>(this->GetSharedRef());
		GJobTimer->Reserve(12000, this->weak_from_this(), job);
	}

	log << "Success";
}

void Room::GameEndToLobby()
{
	LogBuffer log("GameEndToLobby");

	if (this->_state != Protocol::eRoomState::ROOM_STATE_END)
	{
		log << "Not Ending";
		return;
	}

	this->_state = Protocol::eRoomState::ROOM_STATE_LOBBY;

	log << "Success";
}

void Room::Kick(PlayerWeakRef playerWeak, std::string targetNickName /*= ""*/)
{
	LogBuffer log("Kick");

	if (this->_state != Protocol::ROOM_STATE_LOBBY)
	{
		log << "Not Lobby";
		return;
	}

	auto playerRef = playerWeak.lock();
	if (playerRef == nullptr)
	{
		log.PlayerNullptr();
		return;
	}

	log << "Host : " << playerRef->nickname;

	if (this->_players.find(playerRef->uid) == this->_players.end())
	{
		log << "Not Found Player";
		return;
	}

	if (targetNickName.length() == 0)
	{
		log << "Not Found Target";
		return;
	}

	if (_players[playerRef->uid].data.host() == false)
	{
		log << "Not Host";
		return;
	}

	for (auto& [uid, playerData] : _players)
	{
		if (playerData.data.userinfo().nickname() != targetNickName ||
			_players[playerRef->uid].data.userinfo().nickname() == targetNickName)
			continue;

		{
			Protocol::S_ROOM_KICK packet;

			auto sendBuffer = ClientPacketHandler::MakeSendBuffer(packet);
			playerData.player.lock()->ownerGameSession->Send(sendBuffer);
		}

		this->_players.erase(uid);
		playerRef->ownerGameSession->_room.reset();

		{
			Protocol::S_ANOTHER_LEAVE_ROOM packet;

			auto roomInfo = packet.mutable_roominfo();
			this->GetRoomInfo(roomInfo);

			auto sendBuffer = ClientPacketHandler::MakeSendBuffer(packet);
			this->BroadCast(sendBuffer);
		}

		log << "kickPlayer : " << targetNickName;
		break;
	}

	log << "Success";
}

void Room::ChangeTeam(PlayerWeakRef playerWeak, Protocol::eTeamColor color, std::string targetNickName /*= ""*/)
{
	LogBuffer log("ChangeTeam");

	if (this->_state != Protocol::ROOM_STATE_LOBBY)
	{
		log << "Not Lobby";
		return;
	}

	auto playerRef = playerWeak.lock();
	if (playerRef == nullptr)
	{
		log.PlayerNullptr();
		return;
	}

	log << playerRef->nickname;

	if (this->_players.find(playerRef->uid) == this->_players.end())
	{
		log << "Not Found Player";
		return;
	}

	if (color == Protocol::TEAM_COLOR_NONE ||
		color < Protocol::TEAM_COLOR_RED ||
		color > Protocol::TEAM_COLOR_BLUE)
	{
		log << "Invalid Team Color";
		return;
	}

	auto& player = _players[playerRef->uid];
	if (player.data.team() == color)
	{
		log << "Same Team";
		return;
	}

	if (targetNickName.length() == 0)
	{
		log << "No Target";
		return;
	}

	if (targetNickName == player.data.userinfo().nickname())
	{
		player.data.set_team(color);
		log << "Change My Team";
	}
	else if (player.data.host())
	{
		for (auto& [uid, playerData] : _players)
		{
			if (playerData.data.userinfo().nickname() == targetNickName)
			{
				playerData.data.set_team(color);
				log << "Change Target Team : " << targetNickName;
				break;
			}
		}
	}
	else
	{
		log << "Not Host";
		return;
	}

	// 방에 있는 사람들에게 보내주기
	{
		Protocol::S_ROOM_CHANGE_TEAM packet;
		this->GetRoomInfo(packet.mutable_roominfo());

		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(packet);
		this->BroadCast(sendBuffer);
	}

	log << "Success";
}

void Room::RoomChat(PlayerWeakRef playerWeak, std::string chat)
{
	LogBuffer log("Chat");

	if (this->_state != Protocol::ROOM_STATE_LOBBY)
	{
		log << "Not Lobby";
		return;
	}

	auto player = playerWeak.lock();
	if (player == nullptr)
	{
		log.PlayerNullptr();
		return;
	}

	if (this->_players.find(player->uid) == this->_players.end())
	{
		log << "Not Found Player";
		return;
	}

	Protocol::S_ROOM_CHAT packet;
	packet.set_nickname(player->nickname);
	packet.set_chat(chat);

	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(packet);
	this->BroadCast(sendBuffer);

	log << "Player : " << player->nickname;
}

void Room::PlayJump(PlayerWeakRef playerWeak)
{
	LogBuffer log("Jump");

	auto player = playerWeak.lock();
	{
		if (player == nullptr)
		{
			log.PlayerNullptr();
			return;
		}

		if (this->_state != Protocol::ROOM_STATE_PLAY)
		{
			log << "Not Playing";
			return;
		}

		if (_players.find(player->uid) == _players.end())
		{
			log << "Not Found Player";
			return;
		}
	}

	Protocol::S_PLAY_JUMP packet;

	packet.mutable_playerdata()->CopyFrom(_players[player->uid].data);

	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(packet);
	this->BroadCast(sendBuffer);

	log << "Player : " << player->nickname;
}

void Room::PlayShoot(PlayerWeakRef shooterWeak, uint64 targetUid, Protocol::eHitLocation hitLocation)
{
	LogBuffer log("Shoot");

	auto shooterRef = shooterWeak.lock();
	{
		if (shooterRef == nullptr)
		{
			log.PlayerNullptr();
			return;
		}

		if (this->_state != Protocol::ROOM_STATE_PLAY)
		{
			log << "Not Playing";
			return;
		}

		if (_players.find(shooterRef->uid) == _players.end())
		{
			log << "Not Found Player";
			return;
		}
	}

	Protocol::S_PLAY_SHOOT packet;

	auto& shootPlayerData = _players[shooterRef->uid].data;
	packet.mutable_shootplayer()->CopyFrom(shootPlayerData);

	// 맞은 사람이 있을 때
	if (targetUid != 0)
	{
		if (_players.find(targetUid) == _players.end())
		{
			log << "Not Found Target";
			return;
		}
		auto& targetPlayerData = _players[targetUid].data;

		// Todo 추가 검증 필요

		// 총알이 맞았을때 대미지 계산
		this->PlayHit(targetUid, hitLocation);

		// 먼저 발사 정보 보내기
		packet.mutable_shootplayer()->CopyFrom(shootPlayerData);
		packet.mutable_hitplayer()->CopyFrom(targetPlayerData);
		packet.set_hitlocation(hitLocation);

		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(packet);
		this->BroadCast(sendBuffer);

		// 체력 0이하 사망처리
		if (1 > targetPlayerData.hp() && !targetPlayerData.isdead())
		{
			this->PlayKill(shooterRef->uid, targetUid);
		}
	}
	else
	{
		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(packet);
		this->BroadCast(sendBuffer);
	}

	log << "Player : " << shooterRef->nickname;
}

void Room::PlayHit(uint64 hitPlayerUid, Protocol::eHitLocation hitLocation)
{
	LogBuffer log("Hit");

	auto& playerData = _players[hitPlayerUid].data;

	if (playerData.isdead())
	{
		log << "Already Dead";
		return;
	}

	static int32 headDamage = 100;
	static int32 bodyDamage = 34;

	switch (hitLocation)
	{
		case Protocol::HIT_LOCATION_NONE:
			log << "Hit Location None";
			break;
		case Protocol::HIT_LOCATION_NO_HIT:
			log << "No Hit";
			break;
		case Protocol::HIT_LOCATION_HEAD:
			log << "Head";
			playerData.set_hp(playerData.hp() - headDamage);
			break;
		case Protocol::HIT_LOCATION_BODY:
			log << "Body";
			playerData.set_hp(playerData.hp() - bodyDamage);
			break;
		case Protocol::HIT_LOCATION_ARM:
			log << "Arm";
			break;
		case Protocol::HIT_LOCATION_LEG:
			log << "Leg";
			break;
		default:
			break;
	}

	log << "Player : " << _players[hitPlayerUid].data.userinfo().nickname();
}

void Room::PlayKill(uint64 killPlayerUid, uint64 deathPlayerUid)
{
	LogBuffer log("Kill");

	auto& shootPlayerData = _players[killPlayerUid].data;
	auto& targetPlayerData = _players[deathPlayerUid].data;

	log << "Kill : " << shootPlayerData.userinfo().nickname();
	log << "Death : " << targetPlayerData.userinfo().nickname();

	shootPlayerData.set_killcount(shootPlayerData.killcount() + 1);
	targetPlayerData.set_deathcount(targetPlayerData.deathcount() + 1);
	targetPlayerData.set_isdead(true);

	Protocol::S_PLAY_KILL_DEATH killDeathPacket;
	killDeathPacket.mutable_killplayer()->CopyFrom(shootPlayerData);
	killDeathPacket.mutable_deathplayer()->CopyFrom(targetPlayerData);

	auto killDeathBuffer = ClientPacketHandler::MakeSendBuffer(killDeathPacket);
	this->BroadCast(killDeathBuffer);

	// 목표 킬 달성시 게임 종료
	if (shootPlayerData.killcount() >= _gameRule.desiredkill())
	{
		this->GameEnd(true, _gameStartTime);
		log << "Kill Game End";
		return;
	}

	// 리스폰 등록
	{
		auto job = Horang::MakeShared<PlayRespawnJob>(this->GetSharedRef(), deathPlayerUid);
		GJobTimer->Reserve(_gameRule.respawntime() * 1000, this->weak_from_this(), job);
	}


	log << "Success";
}

void Room::PlayRespawn(uint64 deathPlayerUid)
{
	static std::random_device rd;
	static std::mt19937 gen{ rd() };
	std::uniform_int_distribution<> dis(1, _gameRule.spawnindexrange());

	LogBuffer log("Respawn");

	if (_players.find(deathPlayerUid) == _players.end())
	{
		log << "Not Found Player";
		return;
	}

	auto& playerData = _players[deathPlayerUid].data;

	playerData.set_hp(_gameRule.maxhp());
	playerData.set_isdead(false);

	Protocol::S_PLAY_RESPAWN packet;
	packet.mutable_playerdata()->CopyFrom(playerData);
	packet.set_spawnpointindex(dis(gen));

	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(packet);
	this->BroadCast(sendBuffer);

	log << "Player : " << _players[deathPlayerUid].data.userinfo().nickname();
}

void Room::PlayRoll(uint64 playerUid)
{
	LogBuffer log("Roll");

	if (_players.find(playerUid) == _players.end())
	{
		log << "Not Found Player";
		return;
	}

	auto& playerData = _players[playerUid].data;

	// Todo Roll 처리

	Protocol::S_PLAY_ROLL packet;
	packet.mutable_playerdata()->CopyFrom(playerData);

	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(packet);
	this->BroadCast(sendBuffer);

	log << "Player : " << _players[playerUid].data.userinfo().nickname();
}

void Room::PlayReload(uint64 playerUid)
{
	LogBuffer log("Reload");

	if (_players.find(playerUid) == _players.end())
	{
		log << "Not Found Player";
		return;
	}

	auto& playerData = _players[playerUid].data;

	// Todo Reload 처리

	Protocol::S_PLAY_RELOAD packet;
	packet.mutable_playerdata()->CopyFrom(playerData);

	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(packet);
	this->BroadCast(sendBuffer);

	log << "Player : " << _players[playerUid].data.userinfo().nickname();
}

Protocol::RoomInfo Room::GetRoomInfo()
{
	Protocol::RoomInfo roomInfo;
	this->GetRoomInfo(roomInfo);
	return roomInfo;
}

void Room::GetRoomInfo(Protocol::RoomInfo& roomInfo)
{
	roomInfo.set_roomid(_roomId);
	roomInfo.set_roomcode(_roomCode);
	roomInfo.set_state(_state);

	for (auto& [uid, player] : _players)
	{
		auto playerRef = player.player.lock();
		if (playerRef == nullptr)
			continue;

		auto playerData = roomInfo.add_users();
		(*playerData) = player.data;
	}

	roomInfo.set_roomname(_roomName);
	roomInfo.set_password(_password);
	roomInfo.set_maxplayercount(_maxPlayerCount);
	roomInfo.set_currentplayercount(_players.size());
	roomInfo.set_isprivate(_isPrivate);
	roomInfo.set_isteam(_isTeam);
}

void Room::GetRoomInfo(Protocol::RoomInfo* roomInfo)
{
	this->GetRoomInfo(*roomInfo);
}

void Room::GetRoomInfoList(Protocol::RoomInfo* roomInfo)
{
	roomInfo->set_roomid(_roomId);
	roomInfo->set_roomcode(_roomCode);
	roomInfo->set_state(_state);
	roomInfo->set_roomname(_roomName);
	roomInfo->set_password(_password);
	roomInfo->set_maxplayercount(_maxPlayerCount);
	roomInfo->set_currentplayercount(_players.size());
	roomInfo->set_isprivate(_isPrivate);
	roomInfo->set_isteam(_isTeam);
}

void Room::SetUpdatePacket(Protocol::S_PLAY_UPDATE& packet)
{
	this->GetRoomInfo(packet.mutable_roominfo());
	for (auto& [uid, playerData] : _players)
	{
		auto player = packet.add_playerdata();
		player->CopyFrom(playerData.data);
	}
}

void Room::GetPlayerData(Protocol::PlayerData* playerData, int32 uid)
{
	//playerData->CopyFrom(_players[uid].data);
	*playerData = _players[uid].data;
	*playerData->mutable_userinfo() = _players[uid].data.userinfo();
}