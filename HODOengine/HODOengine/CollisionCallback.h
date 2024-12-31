#define _SILENCE_CXX20_CISO646_REMOVED_WARNING
#pragma once

#include "../include/physX/PxPhysics.h"
#include "../include/physX/PxPhysicsAPI.h"

#include <unordered_map>
#include <vector>
#include <functional>
#include <string>

#include "PhysicsCollision.h"
#include "Collider.h"

namespace HDEngine
{
	class CollidersPair
	{
	public:
		CollidersPair() = default;
		CollidersPair(HDData::Collider* first, HDData::Collider* second)
			: _first(first), _second(second)
		{}

		HDData::Collider* _first;
		HDData::Collider* _second;
	};
}

// CollidersPair를 Unordered_map의 Key로 쓰기 위해 std 오버로드.
// 커스텀 클래스를 Key로 사용하려면, 해당 클래스에서 사용될 hash / equal_to를 템플릿 특수화해줘야 한다.
namespace std
{
	template<>
	struct hash<HDEngine::CollidersPair>
	{
		size_t operator()(const HDEngine::CollidersPair& t) const
		{
			return std::hash<HDData::Collider*>()(t._first) ^ std::hash<HDData::Collider*>()(t._second);
		}
	};

	template<>
	struct equal_to<HDEngine::CollidersPair>
	{
		bool operator()(const HDEngine::CollidersPair& lhs, const HDEngine::CollidersPair& rhs) const
		{
			return (lhs._first == rhs._first) && (lhs._second == rhs._second);
		}
	};
}

namespace HDEngine
{
	class CollisionCallback : public physx::PxSimulationEventCallback
	{
	public:
		CollisionCallback();
		~CollisionCallback();

		// 충돌하는 두 놈과 충돌정보를 map으로 묶어 관리.
		using CollisionsPool = std::unordered_map<CollidersPair, HDData::PhysicsCollision>;

		// 관리되는 목록을 Clear하는 함수.
		void Clear();

		// New / Old / Removed 콜리젼 생성, 알맞은 Trigger Pair 생성.
		void CollectResults();

		// Collision Event들을 관리되고 있는 오브젝트들에게 보낸다.
		void SendCollisionEvents();

		// Trigger Event들을 관리되고 있는 오브젝트들에게 보낸다.
		void SendTriggerEvents();

		// Collider가 제거되었을 때 호출됨. 캐시된 이벤트들은 해당 오브젝트에 대해 제거되어야 함.
		void OnColliderRemoved(HDData::Collider* collider);

	public:
		CollisionsPool _collisions;
		CollisionsPool _prevCollisions;

		//Enter Event (OnCollisionEnter)를 위해 새롭게 생성된 Collision 세트들.
		std::vector<CollidersPair> _newCollisionPairs;

		//Exit Event (OnCollisionExit)를 위한 제거된 Collision 세트들.
		std::vector<CollidersPair> _removedCollisionPairs;

		//Enter Event (OnTriggerEnter)를 위해 새롭게 생성된 Trigger 세트들.
		std::vector<CollidersPair> _newTriggerPairs;

		//Exit Event (OnTriggerExit)를 위한 제거되어 잃어버린 Trigger 세트들.
		std::vector<CollidersPair> _lostTriggerPairs;

	public:
		virtual void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs)override;
		virtual void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count)override;
		virtual void onSleep(physx::PxActor** actors, physx::PxU32 count)override;
		virtual void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count)override;
		virtual void onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count)override;
		virtual void onWake(physx::PxActor** actors, physx::PxU32 count)override;

	private:
		void ClearColliderFromCollection(HDData::Collider* collider, std::vector<CollidersPair>& collection);
		void ClearColliderFromCollection(HDData::Collider* collider, CollisionsPool& collection);
	};
}
