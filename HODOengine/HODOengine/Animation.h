#pragma once
#include <string>

namespace HDData
{
	class Animation
	{
	public:
		Animation();
		~Animation();

	public:
		bool GetIsLoop();
		void SetIsLoop(bool isLoop);
		float GetExitTime();
		void SetExitTime(float time);
		bool IsAnimationEnd();
		
	private:
		std::string _name;
		float _exitTime;
		bool _isLoop;
		std::string _fbx;

	};
}


