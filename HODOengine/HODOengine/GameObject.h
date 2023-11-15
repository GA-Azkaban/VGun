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

	template <class ComponentType>
	concept ComponentConcept = std::is_base_of<Component, ComponentType>::value;

	class GameObject
	{
	public:
		friend Component::Component();
		// GameObject�� ������ �ʱ�ȭ �۾��� Scene���� AddGameObject()�� ȣ���� �� �� ����.
		~GameObject() = default;
		GameObject(std::string name = "");
		GameObject(const GameObject&) = delete;
		GameObject(GameObject&&) = delete;
		GameObject& operator=(const GameObject&) = delete;
		GameObject& operator=(GameObject&&) = delete;

		template <ComponentConcept ComponentType>
		ComponentType* AddComponent() {
			ComponentType* component = new ComponentType();
			component->_gameObject = this;
			_components.emplace(component);
			return component;
		}

		template <ComponentConcept ComponentType>
		ComponentType* GetComponent() const {
			for (auto iter = _components.begin(); iter != _components.end(); ++iter)
			{
				ComponentType* castedPointer = dynamic_cast<ComponentType*>(*iter);
				if (castedPointer)
					return castedPointer;
			}
			return nullptr;
		}

		template<ComponentConcept ComponentType>
		std::vector<ComponentType*> GetComponents() const {
			std::vector<ComponentType*> ret;
			for (auto iter = _components.begin(); iter != _components.end(); ++iter)
			{
				ComponentType* castedPointer = dynamic_cast<ComponentType*>(*iter);
				if (castedPointer)
					ret.push_back(castedPointer);
			}
			return ret;
		}

		const std::unordered_set<Component*>& GetAllComponents() const;
		
		void DeleteComponent(Component* component);

		GameObject* GetParentGameObject() { return _parentGameObject; }
		Transform* GetTransform() const { return _transform; }

		void SetParentObject(GameObject* parentObject);
		void SetSelfActive(bool active);

	private:
		std::unordered_set<Component*> _components;
		std::vector<GameObject*> _childrenGameObjects;
		std::string _objectName;
		GameObject* _parentGameObject;
		Transform* _transform;
		bool _selfActive;

	};
}
