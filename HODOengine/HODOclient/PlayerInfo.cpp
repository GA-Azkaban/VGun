#include "PlayerInfo.h"
#include "GameManager.h"
#include "HitEffect.h"
#include "IndicatorPool.h"
#include "UIEffect.h"

PlayerInfo::PlayerInfo()
{
	_timer = new Timer;
	_timer->duration = 5;
	_timer->onExpiration = [=]() {
		_serialkillcount = 0;
		_timer->Stop();
		};
}

PlayerInfo::PlayerInfo(PlayerInfo* info)
{
	_playerUID = info->GetPlayerUID();
	_isHost = info->GetIsHost();
	_playerNickname = info->GetPlayerNickName();

	_timer = new Timer;
	_timer->duration = 5;
	_timer->onExpiration = [=]() {
		_serialkillcount = 0;
		_timer->Stop();
		};
}

void PlayerInfo::Start()
{
	_audio = SoundManager::Instance().AddAudioSourceInObject(GetGameObject());
	this->Init();
}

void PlayerInfo::Update()
{
	if (_isShoot)
	{
		_isShoot = false;
	}

	if (_serialkillcount >= 1)
	{
		if (!_timer->IsActive())
		{
			_timer->Start();
		}
		_timer->Update();
	}
}

void PlayerInfo::Init()
{
	this->_kill = 0;
	this->_death = 0;
	this->_isDie = false;
	this->_bulletCount = 6;
	this->_currentHP = 70;
	this->_serialkillcount = 0;
	this->_state = ePlayerState::IDLE;
}

void PlayerInfo::SetServerTransform(Vector3 pos, Quaternion rot)
{
	_serverPos = pos;
	_serverRot = rot;
}

Vector3& PlayerInfo::GetServerPosition()
{
	return _serverPos;
}

Quaternion& PlayerInfo::GetServerRotation()
{
	return _serverRot;
}

void PlayerInfo::SetPlayerUID(int uid)
{
	_playerUID = uid;
}

void PlayerInfo::SetCurrentHP(int hp)
{
	_currentHP = hp;
}

void PlayerInfo::SetCurrentKill(int killCount)
{
	_kill = killCount;
}

void PlayerInfo::SetCurrentDeath(int deathCount)
{
	_death = deathCount;
}

void PlayerInfo::SetIsDie(bool isDie)
{
	_isDie = isDie;
}

void PlayerInfo::SetCurrentState(ePlayerState state)
{
	this->_prevState = _state;
	this->_state = state;
}

void PlayerInfo::SetCurrentBulletCount(int count)
{
	_bulletCount = count;
}

int& PlayerInfo::GetPlayerUID()
{
	return _playerUID;
}

bool& PlayerInfo::GetIsHost()
{
	return _isHost;
}

void PlayerInfo::SetIsHost(bool isHost)
{
	_isHost = isHost;
}

void PlayerInfo::SetNickName(std::string nickName)
{
	_playerNickname = nickName;
}

bool& PlayerInfo::GetIsDie()
{
	return _isDie;
}

void PlayerInfo::SetIsRespawn(bool isRespawn)
{
	_isRespawn = isRespawn;
}

bool PlayerInfo::GetIsRespawn()
{
	return _isRespawn;
}

bool& PlayerInfo::GetIsStateChange()
{
	return _isStateChange;
}

void PlayerInfo::SetIsStateChange(bool isChange)
{
	_isStateChange = isChange;
}

ePlayerState PlayerInfo::GetPrevPlayerState()
{
	return _prevState;
}

ePlayerState PlayerInfo::GetPlayerState()
{
	return _state;
}

void PlayerInfo::SetIsMyInfo(bool isMine)
{
	_isMyInfo = isMine;
	GameManager::Instance()->SetMyInfo(this);
}

bool PlayerInfo::GetIsMyInfo()
{
	return _isMyInfo;
}

void PlayerInfo::SetIsShoot(bool isShoot)
{
	_isShoot = isShoot;
	if (_isShoot)
	{
		_particleSystem->Play();
	}
}

void PlayerInfo::SetIsJump(bool isJump)
{
	_isJump = isJump;
}

bool PlayerInfo::GetIsShoot()
{
	return _isShoot;
}

bool PlayerInfo::GetIsJump()
{
	return _isJump;
}

void PlayerInfo::SetHitEffectObj(HitEffect* hitEffect)
{
	_hitEffect = hitEffect;
}

void PlayerInfo::SetParticleSystem(HDData::ParticleSystem* particleSystem)
{
	_particleSystem = particleSystem;
}

void PlayerInfo::PlayerAttacked(Vector3 targetPos)
{
	_hitEffect->SetEffectOn();
	IndicatorPool::Instance().SummonIndicator(targetPos);
}

void PlayerInfo::AddSerialKillCount()
{
	_serialkillcount++;

	switch (_serialkillcount)
	{
		case 1:
		{
			//kill!
			_timer->Start();
		}
		break;
		case 2:
		{
			// double kill!
			_killEffectImg->SetImage("doublekill.png");
			_killEffectImg->ChangeScale(0.5, 0.5);
			_killEffectImg->GetGameObject()->GetComponent<UIEffect>()->Play();
		}
		break;
		case 3:
		{
			// triple kill!
			_killEffectImg->SetImage("triplekill.png");
			_killEffectImg->ChangeScale(0.5, 0.5);
			_killEffectImg->GetGameObject()->GetComponent<UIEffect>()->Play();
			_serialkillcount = 0;
		}
		break;
		default:
			break;
	}
}

void PlayerInfo::SetKillEffectImg(HDData::ImageUI* img)
{
	_killEffectImg = img;
}

void PlayerInfo::SetDieEffectImg(HDData::ImageUI* img)
{
	_dieEffectImg = img;
}

void PlayerInfo::PlayHeadShotEffect()
{
	_killEffectImg->SetImage("headshot.png");
	_killEffectImg->ChangeScale(0.5, 0.5);
	_killEffectImg->GetGameObject()->GetComponent<UIEffect>()->Play();
}

void PlayerInfo::PlayDieEffect()
{
	_dieEffectImg->GetGameObject()->SetSelfActive(true);
	_dieEffectImg->FadeIn(1);
}

void PlayerInfo::PlayRespawnEffect()
{
	_dieEffectImg->GetGameObject()->SetSelfActive(false);
	// TODO) 리스폰 이펙트 
}

void PlayerInfo::SetLogUI(HDData::TextUI* txt)
{
	_killLog = txt;
}

void PlayerInfo::PlayKillLog(std::string log)
{
	_killLog->SetText("Killed by..." + log);
	_killLog->GetGameObject()->SetSelfActive(true);
	//_killLog->FadeOut(3);
}

void PlayerInfo::KillLogExit()
{
	_killLog->GetGameObject()->SetSelfActive(false);
}

void PlayerInfo::Init3DSound()
{
	//_audio->AddAudio3D()
}

bool& PlayerInfo::GetPlayerDie()
{
	return _isDie;
}

std::string& PlayerInfo::GetPlayerNickName()
{
	return _playerNickname;
}

int& PlayerInfo::GetPlayerCurrentHP()
{
	return _currentHP;
}

int& PlayerInfo::GetPlayerKillCount()
{
	return _kill;
}

int& PlayerInfo::GetPlayerDeathCount()
{
	return _death;
}

int& PlayerInfo::GetCurrentBulletCount()
{
	return _bulletCount;
}

int PlayerInfo::GetMaxBulletCount()
{
	return _maxBulletCount;
}
