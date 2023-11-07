#pragma once
#include "dllExporter.h"
#include "Component.h"
#include "Transform.h"
#include <vector>
#include <unordered_set>
#include <string>

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
		friend Component::Component();
		// GameObject�� ������ �ʱ�ȭ �۾��� Scene���� AddGameObject()�� ȣ���� �� �� ����.
		GameObject() = default;
		~GameObject() = default;
		GameObject(std::string name = "");
		GameObject(const Component&) = delete;
		GameObject(Component&&) = delete;
		GameObject& operator=(const Component&) = delete;
		GameObject& operator=(Component&&) = delete;

		template <typename ComponentType>
		ComponentType* AddComponent() {
			static_assert(std::is_base_of<Component, ComponentType>::value, "Only derived class from Component class is allowed.");
			ComponentType* component = new ComponentType();
			component->gameObject = this;
			components.emplace(component);
			return component;
		}

		template <typename ComponentType>
		ComponentType* GetComponent() const {
			static_assert(std::is_base_of<Component, ComponentType>::value, "Only derived class from Component class is allowed.");
			for (auto iter = components.begin(); iter != components.end(); ++iter)
			{
				ComponentType* castedPointer = dynamic_cast<ComponentType*>(*iter);
				if (castedPointer)
					return castedPointer;
			}
			return nullptr;
		}

		template<typename ComponentType>
		std::vector<ComponentType*> GetComponents() const {
			std::vector<ComponentType*> ret;
			static_assert(std::is_base_of<Component, ComponentType>::value, "Only derived class from Component class is allowed.");
			for (auto iter = components.begin(); iter != components.end(); ++iter)
			{
				ComponentType* castedPointer = dynamic_cast<ComponentType*>(*iter);
				if (castedPointer)
					ret.push_back(castedPointer);
			}
			return ret;
		}
		
		void DeleteComponent(Component* component);

		GameObject* GetParentGameObject() { return _parentGameObject; }
		Transform* GetTransform() const { return _transform; }

		void SetSelfActive(bool active);

	private:
		GameObject(GameObject* parent);
		std::unordered_set<Component*> _components;
		std::vector<GameObject*> _childrenGameObjects;
		GameObject* _parentGameObject;
		Transform* _transform;
		std::string _objectName;
		bool _selfActive = true;

	};
}
