#pragma once
#include <string>
#include "MathHeader.h"
#include "dllExporter.h"
#include "Component.h"
#include "SoundGroup.h"

namespace HDEngine
{
	class SoundSystem;
}

/// <summary>
/// 씬 내에서 음원을 추가, 재생하는 컴포넌트
/// </summary>

namespace HDData
{
	class HODO_API AudioSource : public Component
	{
	public:
		AudioSource();

		void AddAudio(std::string soundName, std::string soundPath, HDData::SoundGroup soundGroup);
		void AddAudio3D(std::string soundName, std::string soundPath, HDData::SoundGroup soundGroup,
			float minDistance, float maxDistance);

		void PlayOnce(std::string soundName);
		void PlayOnceIfNotPlaying(std::string soundName);
		void PlayOnceAfterStop(std::string soundName, HDData::SoundGroup stopGroup);
		void PlayRepeat(std::string soundName);

		void Play3DOnce(std::string soundName, Vector3 startPos);
		void Play3DRepeat(std::string soundName, Vector3 startPos);

		void Stop(std::string soundName);
		void StopSoundGroup(HDData::SoundGroup group);
		void StopAll();

		void Mute(std::string soundName);
		void MuteSoundGroup(HDData::SoundGroup group);
		void MuteAll();

		void Paused(std::string soundName);
		void PausedSoundGroup(HDData::SoundGroup group);
		void PausedAll();

		/// volume min = 0.0f, max = 1.0f;
		void SetSoundVolume(std::string soundName, float volume);
		void SetSoundGroupVolume(HDData::SoundGroup group, float volume);
		void SetSoundVolumeAll(float volume);

		bool IsSoundPlaying(std::string soundName);

		void UpdateSoundPos(Vector3 pos);

	private:
		HDEngine::SoundSystem& _soundSystem;
	};
}
