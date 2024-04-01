#pragma once
#include "dllExporter.h"
#include <unordered_map>
#include <string>
#include <vector>

namespace HDData
{

	class Transition;

	enum class eConditionType
	{
		NONE,
		FLOAT,
		INT,
		BOOL,
		TRIGGER,

		LAST
	};

	class HODO_API State
	{
		friend class AnimationController;

	public:
		State() = default;
		State(std::string stateName, AnimationController* con) : _stateName(stateName), _isLoop(true), controller(con) {};
		~State() = default;

		struct Transition
		{
			std::string transState = "";
			eConditionType type = eConditionType::NONE;
			std::string conditionName = "";
			std::string condition = "";
			float valueF = 0.f;
			int valueI = 0;
			bool valueB = false;
			bool valueT = false;

			bool _hasTransitionMotion = false;
			std::string _transitionFBX;
		};

	public:
		State& MakeTransition(std::string nextStateName);

		bool GetIsLoop();
		void SetIsLoop(bool isLoop);
		State& AddCondition(std::string nextStateName, std::string conditionName, std::string condition, float value);
		State& AddCondition(std::string nextStateName, std::string conditionName, std::string condition, int value);
		State& AddCondition(std::string nextStateName, std::string conditionName, bool value);
		State& AddCondition(std::string nextStateName, std::string conditionName, bool value, std::string engageMotion);
		State& AddTrigger(std::string nextStateName, std::string conditionName, bool value);

		AnimationController* controller;

		std::string _stateName;
		bool _isLoop;

		std::unordered_map<std::string, std::vector<Transition*>> _transableStates;
		std::string _motion;
	};

	class HODO_API AnimationController
	{
		friend class State;

	public:
		AnimationController();
		~AnimationController();

	public:
		void Start();
		void Update();

	public:
		State* CreateState(std::string stateName, std::string FBX);
		State& GetState(std::string stateName);
		std::unordered_map<std::string, State*> GetAllStates();
		
		void SetEntryState(std::string state);
		
		bool IsStateChange();
		State* GetCurrentState();
		State* GetPrevState();
		std::string GetCurrentStateName();
		std::string GetPrevStateName();
		void SetCurrentState(std::string stateName);
		std::vector<std::string>& GetMotionBuffer();
		bool _isStateChange;

	private:
		std::unordered_map<std::string, State*> _allStates;

		State* _entryState;
		State* _exitState;
		std::vector<State*> _anyState;

		std::vector<std::string> _motionBuffer;

		State* _currentState;
		State* _prevState;

		// 파라미터
	public:
		std::string _triggerNow;
		void CreateFloatParam(std::string paramName, float defaultValue);
		void CreateIntParam(std::string paramName, int defaultValue);
		void CreateBoolParam(std::string boolParam, bool defaultValue);
		void CreateTriggerParam(std::string boolParam);

		void SetFloatParam(std::string, float);
		void SetIntParam(std::string, int);
		void SetBoolParam(std::string, bool);
		void SetTriggerParam(std::string);

	private:
		std::unordered_map<std::string, float> _floatParams;
		std::unordered_map<std::string, int> _intParams;
		std::unordered_map<std::string, bool> _boolParams;
		std::unordered_map<std::string, bool> _triggerParams;

	};

}


