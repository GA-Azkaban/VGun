#include "AnimationController.h"

namespace HDData
{
	AnimationController::AnimationController()
	{

	}

	AnimationController::~AnimationController()
	{

	}

	void AnimationController::Start()
	{
		_currentState = _entryState;
		_prevState = _entryState;
	}

	void AnimationController::Update()
	{
		// 현재 state를 가져온다
		// 갈 수 있는 state에 대한 transition 조건들을 가져온다
		
		State* changeState = _currentState;

		auto transable = _currentState->_transableStates;
		// transition 조건들이 일치하는지 여부를 확인한다
		for (auto& one : transable)
		{
			for (auto& trans : one.second)
			{
				switch (trans->type)
				{
					case eConditionType::FLOAT :
					{
						if (trans->valueF == _floatParams[trans->conditionName])
						{
							changeState = &_allStates[trans->transState];
						}
					}
					break;
					case eConditionType::INT :
					{
						if (trans->valueI == _intParams[trans->conditionName])
						{
							changeState = &_allStates[trans->transState];
						}
					}
					break;
					case eConditionType::BOOL :
					{
						if (trans->valueB == _boolParams[trans->conditionName])
						{
							changeState = &_allStates[trans->transState];
						}
					}
					break;
					case eConditionType::TRIGGER :
					{

					}
					break;
					default:
					{

					}
					break;
				}
			}
		}

		if (changeState != _currentState)
		{
			_prevState = _currentState;
			_currentState = changeState;
		}
	}

	State* AnimationController::CreateState(std::string stateName, std::string fbxName)
	{
		State* state = new State(stateName);
		state->stateName = stateName;
		state->motion = fbxName;

		_allStates.insert({stateName, *state});

		return state;
	}

	HDData::State& AnimationController::GetState(std::string stateName)
	{
		return _allStates[stateName];
	}

	std::unordered_map<std::string, HDData::State> AnimationController::GetAllStates()
	{
		return _allStates;
	}

	void AnimationController::SetEntryState(std::string state)
	{
		_entryState = &_allStates[state];
	}

	HDData::State* AnimationController::GetCurrentState()
	{
		return _currentState;
	}

	HDData::State* AnimationController::GetPrevState()
	{
		return _prevState;
	}

	std::string AnimationController::GetCurrentStateName()
	{
		return _currentState->stateName;
	}

	std::string AnimationController::GetPrevStateName()
	{
		return _prevState->stateName;
	}

	void AnimationController::CreateFloatParam(std::string paramName, float defaultValue)
	{
		_floatParams.insert({paramName, defaultValue});
	}

	void AnimationController::CreateIntParam(std::string paramName, int defaultValue)
	{
		_intParams.insert({ paramName, defaultValue });
	}

	void AnimationController::CreateBoolParam(std::string boolParam, bool defaultValue)
	{
		_boolParams.insert({ boolParam, defaultValue });
	}

	void AnimationController::CreateTriggerParam(std::string boolParam)
	{
		_triggerParams.insert({ boolParam, false });
	}

	void AnimationController::SetFloatParam(std::string name, float val)
	{
		_floatParams.find(name)->second = val;
	}

	void AnimationController::SetIntParam(std::string name, int val)
	{
		_intParams.find(name)->second = val;
	}

	void AnimationController::SetBoolParam(std::string name, bool val)
	{
		_boolParams.find(name)->second = val;
	}

	void AnimationController::SetTriggerParam(std::string name)
	{
		_triggerParams.find(name)->second = !_triggerParams.find(name)->second;
	}

	//////////////////////////////////////////////////////////////////////////
	//State class
	//////////////////////////////////////////////////////////////////////////
	
	State& State::MakeTransition(std::string nextStateName)
	{
		this;
		_transableStates[nextStateName];

		return *this;
	}

	void State::AddCondition(std::string nextStateName, std::string conditionName, std::string condition, float value)
	{
		auto vec = _transableStates[stateName];

		Transition transition;
		transition.transState = nextStateName;
		transition.type = eConditionType::FLOAT;
		transition.conditionName = conditionName;
		transition.valueF = value;
		
		vec.push_back(&transition);
	}

	void State::AddCondition(std::string nextStateName, std::string conditionName, std::string condition, int value)
	{
		auto vec = _transableStates[stateName];

		Transition transition;
		transition.transState = nextStateName;
		transition.type = eConditionType::INT;
		transition.conditionName = conditionName;
		transition.valueI = value;

		vec.push_back(&transition);
	}

	void State::AddCondition(std::string nextStateName, std::string conditionName, bool value)
	{
		this;
		std::vector<Transition*>& vec = _transableStates[nextStateName];

		Transition* transition = new Transition;
		transition->transState = nextStateName;
		transition->type = eConditionType::BOOL;
		transition->conditionName = conditionName;
		transition->valueB = value;

		vec.push_back(transition);
	}

	void State::AddCondition(std::string nextStateName, std::string conditionName)
	{
		auto vec = _transableStates[stateName];

		Transition transition;
		transition.transState = nextStateName;
		transition.type = eConditionType::TRIGGER;
		transition.conditionName = conditionName;

		vec.push_back(&transition);
	}

}
