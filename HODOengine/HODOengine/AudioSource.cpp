#include "AudioSource.h"
#include "SoundSystem.h"

HDData::AudioSource::AudioSource()
	: _soundSystem(HDEngine::SoundSystem::Instance())
{

}

void HDData::AudioSource::AddAudio(std::string soundName, std::string soundPath, HDData::SoundGroup soundGroup)
{
	_soundSystem.CreateSound(soundPath, soundGroup);
	_soundSystem.GetSoundPathList().insert(std::make_pair(soundName, soundPath));
}

void HDData::AudioSource::AddAudio3D(std::string soundName, std::string soundPath, HDData::SoundGroup soundGroup, float minDistance, float maxDistance)
{
	_soundSystem.CreateSound3D(soundPath, soundGroup, minDistance, maxDistance);
	_soundSystem.GetSoundPathList().insert(std::make_pair(soundName, soundPath));
}

void HDData::AudioSource::PlayOnce(std::string soundName)
{
	auto iter = _soundSystem.GetSoundPathList().find(soundName);
	if (iter != _soundSystem.GetSoundPathList().end())
		_soundSystem.PlayOnce(iter->second);
}

void HDData::AudioSource::PlayRepeat(std::string soundName)
{
	auto iter = _soundSystem.GetSoundPathList().find(soundName);
	if (iter != _soundSystem.GetSoundPathList().end())
		_soundSystem.PlayRepeat(iter->second);
}

void HDData::AudioSource::Play3DOnce(std::string soundName, Vector3 startPos)
{
	auto iter = _soundSystem.GetSoundPathList().find(soundName);
	if (iter != _soundSystem.GetSoundPathList().end())
		_soundSystem.Play3DOnce(iter->second, startPos);
}

void HDData::AudioSource::Play3DRepeat(std::string soundName, Vector3 startPos)
{
	auto iter = _soundSystem.GetSoundPathList().find(soundName);
	if (iter != _soundSystem.GetSoundPathList().end())
		_soundSystem.Play3DRepeat(iter->second, startPos);
}

void HDData::AudioSource::Stop(std::string soundName)
{
	auto iter = _soundSystem.GetSoundPathList().find(soundName);
	if (iter != _soundSystem.GetSoundPathList().end())
		_soundSystem.Stop(iter->second);
}

void HDData::AudioSource::StopSoundGroup(HDData::SoundGroup group)
{
	_soundSystem.StopSoundGroup(group);
}

void HDData::AudioSource::StopAll()
{
	_soundSystem.StopAll();
}

void HDData::AudioSource::Mute(std::string soundName)
{
	auto iter = _soundSystem.GetSoundPathList().find(soundName);
	if (iter != _soundSystem.GetSoundPathList().end())
		_soundSystem.Mute(iter->second);
}

void HDData::AudioSource::MuteSoundGroup(HDData::SoundGroup group)
{
	_soundSystem.MuteSoundGroup(group);
}

void HDData::AudioSource::MuteAll()
{
	_soundSystem.MuteAll();
}

void HDData::AudioSource::Paused(std::string soundName)
{
	auto iter = _soundSystem.GetSoundPathList().find(soundName);
	if (iter != _soundSystem.GetSoundPathList().end())
		_soundSystem.Paused(iter->second);
}

void HDData::AudioSource::PausedSoundGroup(HDData::SoundGroup group)
{
	_soundSystem.PausedSoundGroup(group);
}

void HDData::AudioSource::PausedAll()
{
	_soundSystem.PausedAll();
}

void HDData::AudioSource::SetSoundVolume(std::string soundName, float volume)
{
	auto iter = _soundSystem.GetSoundPathList().find(soundName);
	if (iter != _soundSystem.GetSoundPathList().end())
		_soundSystem.SetSoundVolume(iter->second, volume);
}

void HDData::AudioSource::SetSoundGroupVolume(HDData::SoundGroup group, float volume)
{
	_soundSystem.SetSoundGroupVolume(group, volume);
}

void HDData::AudioSource::SetSoundVolumeAll(float volume)
{
	_soundSystem.SetSoundVolumeAll(volume);
}

bool HDData::AudioSource::IsSoundPlaying(std::string soundName)
{
	auto iter = _soundSystem.GetSoundPathList().find(soundName);
	if (iter != _soundSystem.GetSoundPathList().end())
	{
		return _soundSystem.IsSoundPlaying(iter->second);
	}

	return false;
}
