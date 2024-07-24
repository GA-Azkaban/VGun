#pragma once
#define _SILENCE_CXX20_CISO646_REMOVED_WARNING
#include "dllExporter.h"
#include "PhysicsCollision.h"
#include <string>

/// <summary>
/// Component�� ���� ������Ʈ�� ��ġ�� �� �ִ� ��ü�Դϴ�.
/// ���� ������Ʈ�� ��ġ�� ������Ʈ�� ���� ���� ������Ʈ�� ������ �����˴ϴ�.
/// 2023.11.01 �����
/// </summary>

class HODOengine;

namespace HDData
{
	class GameObject;
	class Transform;

	class HODO_API Component
	{
	public:
		friend GameObject;
		friend HODOengine;

		// 비활성화 된 게임 오브젝트가 활성화 될 때 호출되는 콜백 함수입니다.
		virtual void OnEnable() {};
		// 활성화 된 게임 오브젝트가 비활성화 될 때 호출되는 콜백 함수입니다.
		virtual void OnDisable() {};
		// 최초로 Update가 호출될 때 호출되는 콜백 함수입니다.
		virtual void Start() {};
		// 매 프레임마다 호출되는 콜백 함수입니다.
		virtual void Update() {};
		// Update()가 끝나고 갱신해야 할 때 호출되는 콜백 함수입니다.
		virtual void LateUpdate() {};
		// 정확한 시간 간격으로 호출되는 콜백 함수입니다.
		virtual void FixedUpdate() {};
		// 게임 오브젝트가 삭제될 때 호출되는 콜백 함수입니다.
		virtual void OnDestroy() {};

		// 충돌 관련
		// 충돌체들이 서로 충돌을 시작할 때 호출되는 콜백 함수입니다.
		virtual void OnCollisionEnter(PhysicsCollision** _colArr, unsigned int count) {};
		// 충돌체들이 서로 충돌하고 있는 상태일 때 호출되는 콜백 함수입니다.
		virtual void OnCollisionStay(PhysicsCollision** _colArr, unsigned int count) {};
		// 충돌체들의 충돌이 끝나고 서로 떨어져나갈 때 호출되는 콜백 함수입니다.
		virtual void OnCollisionExit(PhysicsCollision** _colArr, unsigned int count) {};
		// Triggers
		virtual void OnTriggerEnter(Collider** _colArr, unsigned int count) {};
		virtual void OnTriggerExit(Collider** _colArr, unsigned int count) {};

		virtual ~Component() {};

		GameObject* GetGameObject() const;
		Transform* GetTransform() const;

		bool GetIsActive() { return _isActive; };
		bool GetIsStarted() { return _isStarted; };

		void SetActive(bool active) {
 			if (!_isActive && active)
				OnEnable();
			if (_isActive && !active) 
				OnDisable();

			_isActive = active;
		};


	protected:
		Component();

	private:
		GameObject* _gameObject;
		bool _isActive;
		bool _isStarted;
	};
}

