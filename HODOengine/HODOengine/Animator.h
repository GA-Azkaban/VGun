#pragma once
#include "dllExporter.h"
#include "Component.h"
#include "AnimationController.h"


namespace HDData
{
	class HODO_API Animator : public Component
	{
	public:
		Animator();
		~Animator();

	public:
		virtual void Start() override;
		virtual void Update() override;
		void SetAnimationController(AnimationController* controller);

	private:
		// 컨트롤러에 대한 포인터
		AnimationController* _animationController;
		std::string _current;
		std::string _prev;

	public:
		void SetFloat(std::string, float);
		void SetInt(std::string, int);
		void SetBool(std::string, bool);
		void SetTrigger(std::string, bool);
	};
}


