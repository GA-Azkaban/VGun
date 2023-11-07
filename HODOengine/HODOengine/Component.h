#pragma once
#include "dllExporter.h"

/// <summary>
/// Component�� ���� ������Ʈ�� ��ġ�� �� �ִ� ��ü�Դϴ�.
/// ���� ������Ʈ�� ��ġ�� ������Ʈ�� ���� ���� ������Ʈ�� ������ �����˴ϴ�.
/// 2023.11.01 �����
/// </summary>

namespace hodoEngine
{
	class GameObject;
	class Transform;
	class Collision;

	class HODO_API Component
	{
	public:
		friend GameObject;

		// ��Ȱ��ȭ �� ���� ������Ʈ�� Ȱ��ȭ �� �� ȣ��Ǵ� �ݹ� �Լ��Դϴ�.
		virtual void OnEnable() {};
		// Ȱ��ȭ �� ���� ������Ʈ�� ��Ȱ��ȭ �� �� ȣ��Ǵ� �ݹ� �Լ��Դϴ�.
		virtual void OnDisable() {};
		// ���ʷ� Update�� ȣ��� �� ȣ��Ǵ� �ݹ� �Լ��Դϴ�.
		virtual void Start() {};
		// �� �����Ӹ��� ȣ��Ǵ� �ݹ� �Լ��Դϴ�.
		virtual void Update() {};
		// ��Ȯ�� �ð� �������� ȣ��Ǵ� �ݹ� �Լ��Դϴ�.
		virtual void FixedUpdate() {};
		// ���� ������Ʈ�� ������ �� ȣ��Ǵ� �ݹ� �Լ��Դϴ�.
		virtual void OnDestroy() {};

		// �浹 ����
		// �浹ü���� ���� �浹�� ������ �� ȣ��Ǵ� �ݹ� �Լ��Դϴ�.
		virtual void OnCollisionEnter(const Collision& collision) {};
		// �浹ü���� ���� �浹�ϰ� �ִ� ������ �� ȣ��Ǵ� �ݹ� �Լ��Դϴ�.
		virtual void OnCollisionStay(const Collision& collision) {};
		// �浹ü���� �浹�� ������ ���� ���������� �� ȣ��Ǵ� �ݹ� �Լ��Դϴ�.
		virtual void OnCollisionExit(const Collision& collision) {};
		virtual ~Component() {};

		GameObject* GetGameObject() const { return _gameObject; };

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
