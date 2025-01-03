﻿#include "SoundSystem.h"
#include "AudioListener.h"
#include "AudioSource.h"
#include "Transform.h"
#include "TimeSystem.h"

HDEngine::SoundSystem::SoundSystem()
	: _audioListner(), _fmodSystem(), _channelGroupMaster(),
	MAXCHANNELGROUP(static_cast<unsigned int>(HDData::SoundGroup::MAX))
{
	_channelGroups.resize(MAXCHANNELGROUP);

	// FMOD::System 초기화
	FMOD::System_Create(&_fmodSystem);	// FMOD System 객체 생성
	_fmodSystem->init(MAXSOUNDCOUNT, FMOD_INIT_NORMAL, NULL);	// FMOD System 객체 초기화
	_fmodSystem->getMasterChannelGroup(&_channelGroupMaster);	// FMOD System 채널 그룹 초기화
	_fmodSystem->set3DSettings(1.0f, DISTANCEFACTOR, 1.0f);	// 3D 사운드에 대한 크기, 거리 계수 등을 설정

	// FMOD::ChannelGroup 초기화
	for (unsigned int i = 0; i < MAXCHANNELGROUP; ++i)
	{
		// 개별 Group 초기화
		_channelGroups[i] = nullptr;
		std::string groupName = "GroupName" + std::to_string(i);
		_fmodSystem->createChannelGroup(groupName.c_str(), &_channelGroups[i]);
		_channelGroupMaster->addGroup(_channelGroups[i]);	// 개별 group을 masterGroup에 추가
	}
}

HDEngine::SoundSystem::~SoundSystem()
{
	// Sound 해제
	for (auto& eachSound : _2DSoundList)
	{
		eachSound.second.sound->release();
	}
	for (auto& eachSound : _3DSoundList)
	{
		for (auto& eachSound : eachSound.second)
		{
			eachSound.second.sound->release();
		}
	}

	// System과 Channel Group 해제
	for (unsigned int i = 0; i < MAXCHANNELGROUP; ++i)
	{
		_channelGroups[i]->release();
	}
	_channelGroupMaster->release();

	_fmodSystem->close();
	_fmodSystem->release();
}

void HDEngine::SoundSystem::Update()
{
	static FMOD_VECTOR lastListnerPos = { 0.0f, 0.0f, 0.0f };

	const Vector3& listenerPos = _audioListner->GetTransform()->GetPosition();
	const Vector3& listenerForward = _audioListner->GetTransform()->GetForward();
	const Vector3& listenerUp = _audioListner->GetTransform()->GetUp();

	FMOD_VECTOR listenerPosVec{ listenerPos.x, listenerPos.y, listenerPos.z };
	FMOD_VECTOR listenerForwardVec{ listenerForward.x, listenerForward.y, listenerForward.z };
	FMOD_VECTOR listenerUpVec{ listenerUp.x, listenerUp.y, listenerUp.z };

	_fmodSystem->set3DListenerAttributes(0, &listenerPosVec, 0, &listenerForwardVec, &listenerUpVec);
	_fmodSystem->update();

	Update3DSoundPosition();
}

void HDEngine::SoundSystem::SetAudioListner(HDData::AudioListner* listner)
{
	_audioListner = listner;
}

void HDEngine::SoundSystem::CreateSound(std::string soundPath, HDData::SoundGroup soundGroup)
{
	HDData::AudioClip audioClip;
	_fmodSystem->createSound(soundPath.c_str(), FMOD_2D, NULL, &(audioClip.sound));

	audioClip.soundPath = soundPath;
	audioClip.channel = nullptr;
	audioClip.channel->setChannelGroup(_channelGroups[(int)soundGroup]);
	audioClip.soundGroup = soundGroup;
	audioClip.is3DSound = false;

	_2DSoundList.insert(std::make_pair(soundPath, audioClip));
}

void HDEngine::SoundSystem::CreateSound3D(HDData::AudioSource* audioSource, std::string soundPath, HDData::SoundGroup soundGroup, float minDistance, float maxDistance)
{
	HDData::AudioClip audioClip;
	_fmodSystem->createSound(soundPath.c_str(), FMOD_3D | FMOD_3D_LINEARROLLOFF, NULL, &(audioClip.sound));
	audioClip.sound->set3DMinMaxDistance(minDistance * DISTANCEFACTOR, maxDistance * DISTANCEFACTOR);

	audioClip.soundPath = soundPath;
	audioClip.channel = nullptr;
	audioClip.channel->setChannelGroup(_channelGroups[(int)soundGroup]);
	audioClip.soundGroup = soundGroup;
	audioClip.is3DSound = true;

	_3DSoundList[audioSource].insert(std::make_pair(soundPath, audioClip));
}

void HDEngine::SoundSystem::PlayOnce(std::string soundPath)
{
	auto soundIter = _2DSoundList.find(soundPath);
	if (soundIter != _2DSoundList.end())
	{
		soundIter->second.sound->setMode(FMOD_LOOP_OFF);
		_fmodSystem->playSound(soundIter->second.sound, _channelGroups[(int)soundIter->second.soundGroup],
			false, &(soundIter->second.channel));
	}
}

void HDEngine::SoundSystem::PlayRepeat(std::string soundPath)
{
	auto soundIter = _2DSoundList.find(soundPath);
	if (soundIter != _2DSoundList.end())
	{
		soundIter->second.channel->stop();
		soundIter->second.sound->setMode(FMOD_LOOP_NORMAL);
		_fmodSystem->playSound(soundIter->second.sound, _channelGroups[(int)soundIter->second.soundGroup],
			false, &(soundIter->second.channel));
	}
}

void HDEngine::SoundSystem::Play3DOnce(HDData::AudioSource* audioSource, std::string soundPath)
{
	auto sourceIter = _3DSoundList.find(audioSource);
	if (sourceIter != _3DSoundList.end())
	{
		auto soundIter = sourceIter->second.find(soundPath);
		if (soundIter != sourceIter->second.end())
		{
			Vector3 startPos = audioSource->GetTransform()->GetPosition();
			FMOD_VECTOR startPosition{ startPos.x, startPos.y, startPos.z };
			FMOD_VECTOR velocity{ 0.0f, 0.0f, 0.0f };
			soundIter->second.sound->setMode(FMOD_LOOP_OFF);
			soundIter->second.channel->set3DAttributes(&startPosition, &velocity);
			soundIter->second.channel->setPaused(false);
			_fmodSystem->playSound(soundIter->second.sound, _channelGroups[(int)soundIter->second.soundGroup],
				true, &(soundIter->second.channel));
		}
	}
}

void HDEngine::SoundSystem::Play3DRepeat(HDData::AudioSource* audioSource, std::string soundPath)
{
	auto sourceIter = _3DSoundList.find(audioSource);
	if (sourceIter != _3DSoundList.end())
	{
		auto soundIter = sourceIter->second.find(soundPath);
		if (soundIter != sourceIter->second.end())
		{
			soundIter->second.channel->stop();
			Vector3 startPos = audioSource->GetTransform()->GetPosition();
			FMOD_VECTOR startPosition{ startPos.x, startPos.y, startPos.z };
			FMOD_VECTOR velocity{ 0, 0, 0 };
			soundIter->second.sound->setMode(FMOD_LOOP_NORMAL);
			_fmodSystem->playSound(soundIter->second.sound, _channelGroups[(int)soundIter->second.soundGroup],
				true, &(soundIter->second.channel));
			soundIter->second.channel->set3DAttributes(&startPosition, &velocity);
			soundIter->second.channel->setPaused(false);
		}
	}
}

void HDEngine::SoundSystem::Stop(HDData::AudioSource* audioSource, std::string soundPath)
{
	auto _2dSoundIter = _2DSoundList.find(soundPath);
	if (_2dSoundIter != _2DSoundList.end())
	{
		_2dSoundIter->second.channel->stop();
	}
	auto sourceIter = _3DSoundList.find(audioSource);
	if (sourceIter != _3DSoundList.end())
	{
		auto _3dSoundIter = sourceIter->second.find(soundPath);
		if (_3dSoundIter != sourceIter->second.end())
		{
			_3dSoundIter->second.channel->stop();
		}
	}
}

void HDEngine::SoundSystem::StopSoundGroup(HDData::SoundGroup group)
{
	_channelGroups[(int)group]->stop();
}

void HDEngine::SoundSystem::StopAll()
{
	for (unsigned int i = 0; i < MAXCHANNELGROUP; ++i)
	{
		_channelGroups[i]->stop();
	}
}

void HDEngine::SoundSystem::Mute(HDData::AudioSource* audioSource, std::string soundPath)
{
	auto _2dSoundIter = _2DSoundList.find(soundPath);
	if (_2dSoundIter != _2DSoundList.end())
	{
		bool isMuted = false;
		_2dSoundIter->second.channel->getMute(&isMuted);
		_2dSoundIter->second.channel->setMute(!isMuted);
	}
	auto sourceIter = _3DSoundList.find(audioSource);
	if (sourceIter != _3DSoundList.end())
	{
		auto _3dSoundIter = sourceIter->second.find(soundPath);
		if (_3dSoundIter != sourceIter->second.end())
		{
			bool isMuted = false;
			_3dSoundIter->second.channel->getMute(&isMuted);
			_3dSoundIter->second.channel->setMute(!isMuted);
		}
	}
}

void HDEngine::SoundSystem::MuteSoundGroup(HDData::SoundGroup group)
{
	bool isMuted = false;
	_channelGroups[(int)group]->getMute(&isMuted);
	_channelGroups[(int)group]->setMute(!isMuted);
}

void HDEngine::SoundSystem::MuteAll()
{
	bool isMuted = false;
	_channelGroupMaster->getMute(&isMuted);
	_channelGroupMaster->setMute(!isMuted);
}

void HDEngine::SoundSystem::Paused(HDData::AudioSource* audioSource, std::string soundPath)
{
	auto _2dSoundIter = _2DSoundList.find(soundPath);
	if (_2dSoundIter != _2DSoundList.end())
	{
		bool isPaused = false;
		_2dSoundIter->second.channel->getPaused(&isPaused);
		_2dSoundIter->second.channel->setPaused(!isPaused);
	}
	auto sourceIter = _3DSoundList.find(audioSource);
	if (sourceIter != _3DSoundList.end())
	{
		auto _3dSoundIter = sourceIter->second.find(soundPath);
		if (_3dSoundIter != sourceIter->second.end())
		{
			bool isPaused = false;
			_3dSoundIter->second.channel->getPaused(&isPaused);
			_3dSoundIter->second.channel->setPaused(!isPaused);
		}
	}
}

void HDEngine::SoundSystem::PausedSoundGroup(HDData::SoundGroup group)
{
	bool isPaused = false;
	_channelGroups[(int)group]->getPaused(&isPaused);
	_channelGroups[(int)group]->setPaused(!isPaused);
}

void HDEngine::SoundSystem::PausedAll()
{
	bool isPaused = false;
	_channelGroupMaster->getPaused(&isPaused);
	_channelGroupMaster->setPaused(!isPaused);
}

void HDEngine::SoundSystem::SetSoundVolume(HDData::AudioSource* audioSource, std::string soundPath, float volume)
{
	auto _2dSoundIter = _2DSoundList.find(soundPath);
	if (_2dSoundIter != _2DSoundList.end())
	{
		float v = (volume > 1.0f) ? 1.0f : volume;
		v = (volume < 0.0f) ? 0.0f : volume;
		_2dSoundIter->second.channel->setVolume(v);
	}
	auto sourceIter = _3DSoundList.find(audioSource);
	if (sourceIter != _3DSoundList.end())
	{
		auto _3dSoundIter = sourceIter->second.find(soundPath);
		if (_3dSoundIter != sourceIter->second.end())
		{
			float v = (volume > 1.0f) ? 1.0f : volume;
			v = (volume < 0.0f) ? 0.0f : volume;
			_3dSoundIter->second.channel->setVolume(v);
		}
	}
}

void HDEngine::SoundSystem::SetSoundGroupVolume(HDData::SoundGroup group, float volume)
{
	float v = (volume > 1.0f) ? 1.0f : volume;
	v = (volume < 0.0f) ? 0.0f : volume;
	_channelGroups[(int)group]->setVolume(v);
}

void HDEngine::SoundSystem::SetSoundVolumeAll(float volume)
{
	float v = (volume > 1.0f) ? 1.0f : volume;
	v = (volume < 0.0f) ? 0.0f : volume;
	_channelGroupMaster->setVolume(v);
}

bool HDEngine::SoundSystem::IsSoundPlaying(HDData::AudioSource* audioSource, std::string soundPath)
{
	auto _2dSoundIter = _2DSoundList.find(soundPath);
	if (_2dSoundIter != _2DSoundList.end())
	{
		bool isPlaying = false;
		_2dSoundIter->second.channel->isPlaying(&isPlaying);
		return isPlaying;
	}
	auto sourceIter = _3DSoundList.find(audioSource);
	if (sourceIter != _3DSoundList.end())
	{
		auto _3dSoundIter = sourceIter->second.find(soundPath);
		if (_3dSoundIter != sourceIter->second.end())
		{
			bool isPlaying = false;
			_3dSoundIter->second.channel->isPlaying(&isPlaying);
			return isPlaying;
		}
	}
	return false;
}

void HDEngine::SoundSystem::Update3DSoundPosition()
{
	for (auto& eachSource : _3DSoundList)
	{
		Vector3 position = eachSource.first->GetTransform()->GetPosition();
		FMOD_VECTOR pos{ position.x, position.y, position.z };
		FMOD_VECTOR velocity{ 0, 0, 0 };
		for (auto& eachSound : eachSource.second)
		{
			eachSound.second.channel->set3DAttributes(&pos, &velocity);
		}
	}
}

std::unordered_map<std::string, std::string>& HDEngine::SoundSystem::GetSoundPathList()
{
	return _soundPathList;
}

std::unordered_map<std::string, HDData::AudioClip>& HDEngine::SoundSystem::Get2DSoundList()
{
	return _2DSoundList;
}

std::unordered_map<std::string, HDData::AudioClip>& HDEngine::SoundSystem::Get3DSoundList(HDData::AudioSource* audioSource)
{
	if (_3DSoundList.find(audioSource) != _3DSoundList.end())
	{
		return _3DSoundList[audioSource];
	}
	else
	{
		std::unordered_map<std::string, HDData::AudioClip> emptyMap;
		return emptyMap;
	}
}

