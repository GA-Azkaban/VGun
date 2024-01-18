#pragma once
#include "dllExporter.h"
//#include "Component.h"
#include "Transform.h"
#include <unordered_set>
#include <vector>
#include <string>

/// <summary>
/// 게임 오브젝트는 게임 씬에 배치될 수 있는 가장 기본적인 단위의 객체입니다.
/// 게임 오브젝트는 기본적으로 Transform 컴포넌트를 갖고 있습니다.
/// 게임 오브젝트는 이외에도 다양한 컴포넌트들을 추가로 가질 수 있으며,
/// 가지고 있는 컴포넌트들에 의해 게임 오브젝트의 성질이 결정됩니다.
/// 또한 게임 오브젝트는 자식과 부모 오브젝트를 가지는 계층 구조로 구성됩니다.
/// 자식 게임 오브젝트의 위치와 각도는 부모 게임 오브젝트의 위치와 각도에 종속적입니다.
/// 2023.11.01 김민정
/// </summary>

namespace HDData
{
	class Component;
	class Transform;


	template <class ComponentType>
	concept ComponentConcept = std::is_base_of<Component, ComponentType>::value;

	class HODO_API GameObject
	{
	public:
		friend Component::Component();
		// GameObject의 생성과 초기화 작업은 Scene에서 AddGameObject()를 호출할 때 할 것임.
		~GameObject() = default;
		GameObject(std::string name = "");
		GameObject(const GameObject&) = delete;
		GameObject(GameObject&&) = delete;
		GameObject& operator=(const GameObject&) = delete;
		GameObject& operator=(GameObject&&) = delete;

		void OnEnable();
		void OnDisable();
		void Start();
		void Update();
		void LateUpdate();
		void FixedUpdate();
		void OnDestroy();

		void OnCollisionEnter();
		void OnCollisionStay();
		void OnCollisionExit();

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
		const std::unordered_set<GameObject*>& GetChildGameObjects() const;
		
		template<ComponentConcept ComponentType>
		void DeleteComponent()
		{
			for (auto iter = _components.begin(); iter != _components.end(); ++iter)
			{
				ComponentType* castedPointer = dynamic_cast<ComponentType*>(*iter);
				if (castedPointer != nullptr)
				{
					_components.erase(iter);
				}
			}
		}

		GameObject* GetParentGameObject() { return _parentGameObject; }
		Transform* GetTransform() const { return _transform; }
		bool IsActive() { return _selfActive; }

		void SetParentObject(GameObject* parentObject);
		void SetSelfActive(bool active);
		std::string GetObjectName();

	private:
		std::unordered_set<Component*> _components;

		std::unordered_set<GameObject*> _childGameObjects;
		std::string _objectName;
		GameObject* _parentGameObject;
		Transform* _transform;
		bool _selfActive;

	};
}
