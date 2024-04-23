#include "AnimationController.h"
#include <algorithm>




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
		_isStateChange = false;
		std::string engageAction = "";
		bool isEngageAction = false;

		State* changeState = _currentState;
		auto transable = _currentState->_transableStates;

		for (auto& [name, state] : transable)
		{
			for (auto& trans : state)
			{
				switch (trans->type)
				{
					case eConditionType::FLOAT:
					{
						if (trans->valueF == _floatParams[trans->conditionName])
						{
							changeState = _allStates[trans->transState];
						}
					}
					break;
					case eConditionType::INT:
					{
						if (trans->valueI == _intParams[trans->conditionName])
						{
							changeState = _allStates[trans->transState];
						}
					}
					break;
					case eConditionType::BOOL:
					{
						if (trans->valueB == _boolParams[trans->conditionName])
						{
							changeState = _allStates[trans->transState];

							if (trans->_transitionFBX != "")
							{
								_motionBuffer.push_back(trans->_transitionFBX);
							}
						}
					}
					break;
					case eConditionType::TRIGGER:
					{
						if (_triggerParams[trans->conditionName] == true)
						{
							changeState = _allStates[trans->transState];
							_triggerParams[trans->conditionName] = false;
						}
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
			_isStateChange = true;
		}
	}

	State* AnimationController::CreateState(std::string stateName, std::string upper, std::string lower)
	{
		State* state = new State(stateName, this);
		state->_stateName = stateName;
		state->_motion = upper;

		_allStates.insert({ stateName, state });

		return state;
	}

	HDData::State& AnimationController::GetState(std::string stateName)
	{
		return *_allStates[stateName];
	}

	std::unordered_map<std::string, HDData::State*> AnimationController::GetAllStates()
	{
		return _allStates;
	}

	void AnimationController::SetEntryState(std::string state)
	{
		_entryState = _allStates[state];
	}

	bool AnimationController::IsStateChange()
	{
		return _isStateChange;
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
		return _currentState->_stateName;
	}

	std::string AnimationController::GetPrevStateName()
	{
		return _prevState->_stateName;
	}

	void AnimationController::SetCurrentState(std::string stateName)
	{
		_prevState = _currentState;
		_currentState = _allStates[stateName];
	}

	std::vector<std::string>& AnimationController::GetMotionBuffer()
	{
		return _motionBuffer;
	}

	void AnimationController::CreateFloatParam(std::string paramName, float defaultValue)
	{
		_floatParams.insert({ paramName, defaultValue });
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
		_triggerParams.find(name)->second = true;
	}

	//////////////////////////////////////////////////////////////////////////
	//State class
	//////////////////////////////////////////////////////////////////////////

	State& State::MakeTransition(std::string nextStateName)
	{
		_transableStates[nextStateName];
		return *this;
	}

	bool State::GetIsLoop()
	{
		return _isLoop;
	}

	void State::SetIsLoop(bool isLoop)
	{
		_isLoop = isLoop;
	}

	State& State::AddCondition(std::string nextStateName, std::string conditionName, std::string condition, float value)
	{
		std::vector<Transition*>& vec = _transableStates[nextStateName];

		Transition* transition = new Transition;
		transition->transState = nextStateName;
		transition->type = eConditionType::FLOAT;
		transition->conditionName = conditionName;
		transition->valueF = value;

		vec.push_back(transition);

		return *this;
	}

	State& State::AddCondition(std::string nextStateName, std::string conditionName, std::string condition, int value)
	{
		std::vector<Transition*>& vec = _transableStates[nextStateName];

		Transition* transition = new Transition;
		transition->transState = nextStateName;
		transition->type = eConditionType::INT;
		transition->conditionName = conditionName;
		transition->valueI = value;

		vec.push_back(transition);

		return *this;
	}

	State& State::AddCondition(std::string nextStateName, std::string conditionName, bool value)
	{
		this;
		std::vector<Transition*>& vec = _transableStates[nextStateName];

		Transition* transition = new Transition;
		transition->transState = nextStateName;
		transition->type = eConditionType::BOOL;
		transition->conditionName = conditionName;
		transition->valueB = value;

		vec.push_back(transition);

		return *this;
	}

	HDData::State& State::AddCondition(std::string nextStateName, std::string conditionName, bool value, std::string engageMotion)
	{
		std::vector<Transition*>& vec = _transableStates[nextStateName];

		Transition* transition = new Transition;
		transition->transState = nextStateName;
		transition->type = eConditionType::BOOL;
		transition->conditionName = conditionName;
		transition->valueB = value;
		transition->_transitionFBX = engageMotion;
		transition->_hasTransitionMotion = true;

		vec.push_back(transition);

		return *this;
	}

	State& State::AddTrigger(std::string nextStateName, std::string conditionName, bool value)
	{
		controller->GetAllStates().find(nextStateName)->second->SetIsLoop(false);

		std::vector<Transition*>& vec = _transableStates[nextStateName];

		Transition* transition = new Transition;
		transition->transState = nextStateName;
		transition->type = eConditionType::TRIGGER;
		transition->conditionName = conditionName;
		transition->valueT = value;

		vec.push_back(transition);

		return *this;
	}



}
