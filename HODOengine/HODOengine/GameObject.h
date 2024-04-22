#pragma once
#include <unordered_set>
#include <vector>
#include <string>
#include "dllExporter.h"
//#include "Component.h"
#include "Transform.h"
#include "../HODO3DGraphicsInterface/Node.h"

/// <summary>
/// 게임 오브젝트는 게임 씬에 배치될 수 있는 가장 기본적인 단위의 객체입니다.
/// 게임 오브젝트는 기본적으로 Transform 컴포넌트를 갖고 있습니다.
/// 게임 오브젝트는 이외에도 다양한 컴포넌트들을 추가로 가질 수 있으며,
/// 가지고 있는 컴포넌트들에 의해 게임 오브젝트의 성질이 결정됩니다.
/// 또한 게임 오브젝트는 자식과 부모 오브젝트를 가지는 계층 구조로 구성됩니다.
/// 자식 게임 오브젝트의 위치와 각도는 부모 게임 오브젝트의 위치와 각도에 종속적입니다.
/// 2023.11.01 김민정
/// </summary>

namespace HDEngine
{
	class ObjectSystem;
}

namespace HDData
{
	class Component;
	class Transform;

	template <class ComponentType>
	concept ComponentConcept = std::is_base_of<Component, ComponentType>::value;

	class HODO_API GameObject
	{
	public:
		friend HDEngine::ObjectSystem;
		friend Component::Component();
		// GameObject의 생성과 초기화 작업은 Scene에서 CreateObject()를 호출할 때 할 것임.
		~GameObject() = default;
		GameObject(const GameObject&) = delete;
		GameObject(GameObject&&) = delete;
		GameObject& operator=(const GameObject&) = delete;
		GameObject& operator=(GameObject&&) = delete;

		void OnEnable();
		void OnDisable();
		void FlushEnable();
		void FlushDisable();
		void Start();
		void Update();
		void LateUpdate();
		void FixedUpdate();
		void OnDestroy();

		void OnCollisionEnter();
		void OnCollisionStay();
		void OnCollisionExit();

		template <ComponentConcept ComponentType, typename... Args>
		ComponentType* AddComponent(Args&&... args) {
			ComponentType* component = new ComponentType(std::forward<Args>(args)...);
			component->_gameObject = this;
			_componentsIndexed.push_back(component);
			return component;
		}

		template <ComponentConcept ComponentType>
		ComponentType* GetComponent() const {
			for (int i = 0; i < _componentsIndexed.size(); ++i)
			{
				ComponentType* castedPointer = dynamic_cast<ComponentType*>(_componentsIndexed[i]);
				if (castedPointer)
					return castedPointer;
			}
			return nullptr;
		}

		template <ComponentConcept ComponentType>
		ComponentType* GetComponentInChildren() const {
			ComponentType* comp = GetComponent<ComponentType>();
			if (comp != nullptr)
			{
				return comp;
			}

			for (int i = 0; i < _childGameObjectsIndexed.size(); ++i)
			{
				ComponentType* findComp = _childGameObjectsIndexed[i]->GetComponentInChildren<ComponentType>();
				if (findComp != nullptr)
				{
					return findComp;
				}
			}

			return nullptr;
		}

		template<ComponentConcept ComponentType>
		std::vector<ComponentType*> GetComponents() const {
			std::vector<ComponentType*> ret;
			for (int i = 0; i < _componentsIndexed.size(); ++i)
			{
				ComponentType* castedPointer = dynamic_cast<ComponentType*>(_componentsIndexed[i]);
				if (castedPointer)
					ret.push_back(castedPointer);
			}
			return ret;
		}

		const std::vector<Component*>& GetAllComponents() const;
		const std::vector<GameObject*>& GetChildGameObjects() const;

		template<ComponentConcept ComponentType>
		void DeleteComponent()
		{
			for (auto iter = _componentsIndexed.begin(); iter != _componentsIndexed.end(); ++iter)
			{
				ComponentType* castedPointer = dynamic_cast<ComponentType*>(*iter);
				if (castedPointer != nullptr)
				{
					_componentsIndexed.erase(iter);
				}
			}
		}

		GameObject* GetParentGameObject() { return _parentGameObject; }
		GameObject* GetGameObjectByNameInChildren(const std::string& objectName);
		Transform* GetTransform() { return _transform; }
		bool IsActive() { return _selfActive; }

		void SetParentObject(GameObject* parentObject);
		void SetSelfActive(bool active);
		bool GetSelfActive();
		bool GetParentActive();
		std::string GetObjectName();

		// 노드 계층화가 있는 오브젝트에 호출해 파일 내에 있는 계층도대로 자식 오브젝트들을 만들어준다.
		void LoadFBXFile(std::string fileName);

	private:
		GameObject(std::string name = "");

	private:
		Node* FindNodeByName(Node* node, std::string nodeName);
		void ProcessNode(Node* node, GameObject* parentObject);

	private:
		//std::unordered_set<Component*> _components;
		std::vector<Component*> _componentsIndexed;

		//std::unordered_set<GameObject*> _childGameObjects;
		std::vector<GameObject*> _childGameObjectsIndexed;

		std::string _objectName;
		GameObject* _parentGameObject;
		Transform* _transform;
		bool _selfActive;
	};
}
