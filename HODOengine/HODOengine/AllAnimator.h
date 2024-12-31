#pragma once
#include "dllExporter.h"
#include "Component.h"
#include "AnimationController.h"
#include "SkinnedMeshRenderer.h"

namespace HDData
{
	class HODO_API AllAnimator
	{
	public:
		AllAnimator();
		~AllAnimator();

	public:
		void Start();
		void Update();
		void SetAnimationController(AnimationController* controller);
		void SetMeshRenderer(SkinnedMeshRenderer* renderer);

	private:
		// 컨트롤러에 대한 포인터
		AnimationController* _animationController;
		SkinnedMeshRenderer* _meshRenderer;
		bool _engageIng;
	public:
		std::string GetCurrentState();
		void SetCurrentState(std::string stateName);
		void SetFloat(std::string, float);
		void SetInt(std::string, int);
		void SetBool(std::string, bool);
		void SetTrigger(std::string);
	};

}


