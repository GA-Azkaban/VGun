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
		State(AnimationController* con, std::string stateName) : anicom(con), stateName(stateName), _isLoop(true) {};
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
		};

	public:
		State& MakeTransition(std::string nextStateName);

		bool GetIsLoop();
		void SetIsLoop(bool isLoop);
		void AddCondition(std::string nextStateName, std::string conditionName, std::string condition, float value);
		void AddCondition(std::string nextStateName, std::string conditionName, std::string condition, int value);
		void AddCondition(std::string nextStateName, std::string conditionName, bool value);
		void AddTrigger(std::string nextStateName, std::string conditionName, bool value);


		AnimationController* anicom;

		std::string stateName;
		std::string motion;
		bool _isLoop;
		std::unordered_map<std::string, std::vector<Transition*>> _transableStates;
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
		State* CreateState(std::string stateName, std::string fbxName);
		State& GetState(std::string stateName);
		std::unordered_map<std::string, State*> GetAllStates();
		
		void SetEntryState(std::string state);
		
		bool IsStateChange();
		State* GetCurrentState();
		State* GetPrevState();
		std::string GetCurrentStateName();
		std::string GetPrevStateName();
		void SetCurrentState(std::string stateName);

	private:
		std::unordered_map<std::string, State*> _allStates;

		State* _entryState;
		State* _exitState;
		std::vector<State*> _anyState;

		State* _currentState;
		State* _prevState;
		bool _isStateChange;

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


