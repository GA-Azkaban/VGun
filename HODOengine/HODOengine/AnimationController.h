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
		State(std::string stateName) : stateName(stateName) {};
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
		};

	public:
		State& MakeTransition(std::string nextStateName);

		void AddCondition(std::string nextStateName, std::string conditionName, std::string condition, float value);
		void AddCondition(std::string nextStateName, std::string conditionName, std::string condition, int value);
		void AddCondition(std::string nextStateName, std::string conditionName, bool value);
		void AddCondition(std::string nextStateName, std::string conditionName);

		std::string stateName;
		std::string motion;
		std::unordered_map<std::string, std::vector<Transition*>> _transableStates;
	};

	class HODO_API AnimationController
	{
	public:
		AnimationController();
		~AnimationController();

	public:
		void Start();
		void Update();

	public:
		State* CreateState(std::string stateName, std::string fbxName);
		State& GetState(std::string stateName);
		std::unordered_map<std::string, State> GetAllStates();
		
		void SetEntryState(std::string state);
		
		State* GetCurrentState();
		State* GetPrevState();
		std::string GetCurrentStateName();
		std::string GetPrevStateName();

	private:
		std::unordered_map<std::string, State> _allStates;

		State* _entryState;
		State* _exitState;
		std::vector<State*> _anyState;

		State* _currentState;
		State* _prevState;

		// 파라미터
	public:
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


