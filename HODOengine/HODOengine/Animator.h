#pragma once
#include "dllExporter.h"
#include "Component.h"
#include "AnimationController.h"
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

		void SetAllAnimationController(AnimationController* controller);
		AllAnimator* GetAllAC();

	private:
		AllAnimator* _all;
	};

}


