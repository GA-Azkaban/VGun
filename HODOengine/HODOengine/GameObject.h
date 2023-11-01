#pragma once
#include "dllExporter.h"
#include <unordered_set>

/// <summary>
/// 게임 오브젝트는 게임 씬에 배치될 수 있는 가장 기본적인 단위의 객체입니다.
/// 게임 오브젝트는 기본적으로 Transform 컴포넌트를 갖고 있습니다.
/// 게임 오브젝트는 이외에도 다양한 컴포넌트들을 추가로 가질 수 있으며,
/// 가지고 있는 컴포넌트들에 의해 게임 오브젝트의 성질이 결정됩니다.
/// 또한 게임 오브젝트는 자식과 부모 오브젝트를 가지는 계층 구조로 구성됩니다.
/// 자식 게임 오브젝트의 위치와 각도는 부모 게임 오브젝트의 위치와 각도에 종속적입니다.
/// 2023.11.01 김민정
/// </summary>

namespace hodoEngine
{
	class Component;
	class Transform;

	class HODO_API GameObject
	{
	public:
		// GameObject의 생성과 초기화 작업은 Scene에서 AddGameObject()를 호출할 때 할 것임.
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
