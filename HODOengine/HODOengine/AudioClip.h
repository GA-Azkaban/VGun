#pragma once
#include "SoundGroup.h"
#include <string>
#include "../include/FMOD/fmod.hpp"

namespace HDData
{
	struct AudioClip
	{
		std::string soundPath;
		FMOD::Sound* sound;
		FMOD::Channel* channel;
		HDData::SoundGroup soundGroup;
		bool is3DSound;
	};
}