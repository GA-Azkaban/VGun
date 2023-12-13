#pragma once
#include "dllExporter.h"

/// <summary>
/// Component는 게임 오브젝트에 배치될 수 있는 객체입니다.
/// 게임 오브젝트에 배치된 컴포넌트에 따라 게임 오브젝트의 성질이 결정됩니다.
/// 2023.11.01 김민정
/// </summary>

namespace HDData
{
	class GameObject;
	class Transform;

	class HODO_API Component
	{
	public:
		friend GameObject;

		// 비활성화 된 게임 오브젝트가 활성화 될 때 호출되는 콜백 함수입니다.
		virtual void OnEnable() {};
		// 활성화 된 게임 오브젝트가 비활성화 될 때 호출되는 콜백 함수입니다.
		virtual void OnDisable() {};
		// 최초로 Update가 호출될 때 호출되는 콜백 함수입니다.
		virtual void Start() {};
		// 매 프레임마다 호출되는 콜백 함수입니다.
		virtual void Update() {};
		// 정확한 시간 간격으로 호출되는 콜백 함수입니다.
		virtual void FixedUpdate() {};
		// 게임 오브젝트가 삭제될 때 호출되는 콜백 함수입니다.
		virtual void OnDestroy() {};

		// 충돌 관련
		// 충돌체들이 서로 충돌을 시작할 때 호출되는 콜백 함수입니다.
		virtual void OnCollisionEnter() {};
		// 충돌체들이 서로 충돌하고 있는 상태일 때 호출되는 콜백 함수입니다.
		virtual void OnCollisionStay() {};
		// 충돌체들의 충돌이 끝나고 서로 떨어져나갈 때 호출되는 콜백 함수입니다.
		virtual void OnCollisionExit() {};
		virtual ~Component() {};

		GameObject* GetGameObject() const;
		Transform* GetTransform() const;

		bool GetActive() { return _isActive; };

		void SetActive(bool active) {
			if (!_isActive && active)
				OnEnable();
			if (_isActive && !active)
				OnDisable();

			_isActive = active;
		};

	protected:
		Component() : _gameObject(nullptr), _isActive(true) {};

	private:
		GameObject* _gameObject;
		bool _isActive;

	};
}

