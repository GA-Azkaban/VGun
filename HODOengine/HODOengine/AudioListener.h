#pragma once
#include "Component.h"
#include "SoundSystem.h"

namespace HDData
{
	class AudioListner : public Component
	{
	public:
		AudioListner()
		{
			HDEngine::SoundSystem::Instance().SetAudioListner(this);
		}

		void SetAudioListner()
		{
			HDEngine::SoundSystem::Instance().SetAudioListner(this);
		}
	};
}
