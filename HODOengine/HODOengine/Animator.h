#pragma once
#include "dllExporter.h"
#include "Component.h"
#include "AnimationController.h"
#include "UpperAnimator.h"
#include "LowerAnimator.h"
#include "AllAnimator.h"

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
		void SetUpperAnimationController(AnimationController* controller);
		void SetLowerAnimationController(AnimationController* controller);
		void SetAllAnimationController(AnimationController* controller);
		UpperAnimator* GetUpperAC();
		LowerAnimator* GetLowerAC();
		AllAnimator* GetAllAC();

	private:
		UpperAnimator* _upper;
		LowerAnimator* _lower;
		AllAnimator* _all;
	};

}


