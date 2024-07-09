#pragma once
#include <unordered_map>
#include "Singleton.h"
#include "AudioClip.h"
#include "MathHeader.h"
#include <vector>

namespace HDData
{
	class AudioListner;
}

namespace HDEngine
{

	class SoundSystem : public Singleton<SoundSystem>
	{
		friend Singleton;

	private:
		SoundSystem();
		~SoundSystem();

	public:
		void Update();

		void SetAudioListner(HDData::AudioListner* listner);

		/// <summary>
		/// Sound 추가.
		/// 거리에 따른 Sound 조절이 필요할 시 CreateSound3D 함수를 사용한다.
		/// </summary>
		/// <param name="soundName">Sound 파일 이름</param>
		/// <param name="soundGroup">Sound Group 지정</param>
		void CreateSound(std::string soundPath, HDData::SoundGroup soundGroup);

		/// <summary>
		/// 3D Sound 추가.
		/// 오브젝트에 3D Sound 를 붙일 경우 유용.
		/// </summary>
		/// <param name="soundName">Sound 파일 이름</param>
		/// <param name="soundGroup">Sound Group 지정</param>
		/// <param name="minDistance">소리가 가장 크게 들리는 범위. 이 범위 밖으로 나갈수록 소리가 점차 감소</param>
		/// <param name="maxDistance">소리가 들리는 최대 범위. 이 범위 밖으로 나가면 소리가 들리지 않음</param>
		void CreateSound3D(std::string soundPath, HDData::SoundGroup soundGroup,
			float minDistance, float maxDistance);

		/// 사운드 재생 관련.
		void PlayOnce(std::string soundPath);
		void PlayRepeat(std::string soundPath);

		void Play3DOnce(std::string soundPath, Vector3 startPos);
		void Play3DRepeat(std::string soundPath, Vector3 startPos);

		void Stop(std::string soundPath);
		void StopSoundGroup(HDData::SoundGroup group);
		void StopAll();

		void Mute(std::string soundPath);
		void MuteSoundGroup(HDData::SoundGroup group);
		void MuteAll();

		void Paused(std::string soundPath);
		void PausedSoundGroup(HDData::SoundGroup group);
		void PausedAll();

		/// volume min = 0.0f, max = 1.0f;
		void SetSoundVolume(std::string soundPath, float volume);
		void SetSoundGroupVolume(HDData::SoundGroup group, float volume);
		void SetSoundVolumeAll(float volume);

		bool IsSoundPlaying(std::string soundPath);

		void Update3DSoundPosition(std::string soundName, Vector3 position);

		std::unordered_map<std::string, std::string>& GetSoundPathList();
		std::unordered_map<std::string, HDData::AudioClip>& GetSoundList();

	private:
		/// FMOD 관련 변수들
		HDData::AudioListner* _audioListner;
		FMOD::System* _fmodSystem;
		FMOD::ChannelGroup* _channelGroupMaster;	// 오디오 채널 전체 그룹(_channelGroup 묶음)
		std::vector<FMOD::ChannelGroup*> _channelGroups;	// 오디오 채널 그룹
		std::unordered_map<std::string, std::string> _soundPathList;	// <soundName, soundPath>
		std::unordered_map<std::string, HDData::AudioClip> _soundList;

		/// FMOD 제어에 필요한 변수들
		const unsigned int MAXCHANNELGROUP;
		const unsigned int MAXSOUNDCOUNT = 64;
		const float DISTANCEFACTOR = 0.1f;
	};
}
