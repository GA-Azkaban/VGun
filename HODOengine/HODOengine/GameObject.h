#pragma once
#include "dllExporter.h"
#include <unordered_set>

/// <summary>
/// ���� ������Ʈ�� ���� ���� ��ġ�� �� �ִ� ���� �⺻���� ������ ��ü�Դϴ�.
/// ���� ������Ʈ�� �⺻������ Transform ������Ʈ�� ���� �ֽ��ϴ�.
/// ���� ������Ʈ�� �̿ܿ��� �پ��� ������Ʈ���� �߰��� ���� �� ������,
/// ������ �ִ� ������Ʈ�鿡 ���� ���� ������Ʈ�� ������ �����˴ϴ�.
/// ���� ���� ������Ʈ�� �ڽİ� �θ� ������Ʈ�� ������ ���� ������ �����˴ϴ�.
/// �ڽ� ���� ������Ʈ�� ��ġ�� ������ �θ� ���� ������Ʈ�� ��ġ�� ������ �������Դϴ�.
/// 2023.11.01 �����
/// </summary>

namespace hodoEngine
{
	class Component;
	class Transform;

	class HODO_API GameObject
	{
	public:
		// GameObject�� ������ �ʱ�ȭ �۾��� Scene���� AddGameObject()�� ȣ���� �� �� ����.
		GameObject() = default;
		~GameObject() = default;
		GameObject(const Component&) = delete;
		GameObject(Component&&) = delete;
		GameObject& operator=(const Component&) = delete;
		GameObject& operator=(Component&&) = delete;

		template <typename ComponentType>
		ComponentType* AddComponent() {
			auto component = new ComponentType();
			components.emplace(component);
		}
		
		void DeleteComponent(Component* component);
		
		template <typename ComponentType>
		ComponentType* GetComponent() {
			for (auto iter = components.begin(); iter != components.end(); ++iter)
			{
				static_assert()
			}
			return nullptr;
		}
		// GetComponents
		// GetTransform
		Transform* GetTransform() { return transform; }
		// SetSelfActive
	private:
		std::unordered_set<Component*> components;
		Transform* transform;
	};
}
