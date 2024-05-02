#include "CollisionCallback.h"
#include <cassert>
#include <algorithm>

namespace HDEngine
{
	CollisionCallback::CollisionCallback()
	{

	}

	CollisionCallback::~CollisionCallback()
	{

	}

	void CollisionCallback::Clear()
	{
		//서로를 바꿔주고, Clear.
		std::swap(_collisions, _prevCollisions);
		_collisions.clear();

		_newCollisionPairs.clear();
		_removedCollisionPairs.clear();

		_newTriggerPairs.clear();
		_lostTriggerPairs.clear();
	}

	void CollisionCallback::CollectResults()
	{
		for (auto& it : _collisions)
		{
			// prev전 목록에 Collision이 없는데 현재 목록에는 있으면
			if (!_prevCollisions.contains(it.first))
			{
				// Collision 생성.
				_newCollisionPairs.push_back(it.first);
			}
		}

		for (auto& it : _prevCollisions)
		{
			// 현재 목록에 Collision이 없는데 prev 목록에는 있으면
			if (!_collisions.contains(it.first))
			{
				// 제거된 Collision 목록에 추가.
				_removedCollisionPairs.push_back(it.first);
			}
		}
	}

	void CollisionCallback::SendCollisionEvents()
	{
		//제거된 Collision Pair에서
		for (int i = 0; i < _removedCollisionPairs.size(); i++)
		{
			//막 제거된 CollisionPair.
			const auto& pair = _removedCollisionPairs[i];

			//해당 인덱스된 PhysicsCollision을 가져온다.
			auto& c = _prevCollisions[pair];

			//OnCollisionExit 함수들 발동.
			//pair._first->Collider_OnCollisionExit(c);
			//c.SwapObjects();
			//pair._second->Collider_OnCollisionExit(c);
			//c.SwapObjects();

			//OnCollisionExit 함수들 발동.
			if (!pair._first->GetTrigger())
			{
				pair._first->Collider_OnCollisionExit(c);
			}
			c.SwapObjects();
			if (!pair._second->GetTrigger())
			{
				pair._second->Collider_OnCollisionExit(c);
			}
			c.SwapObjects();
		}

		for (int i = 0; i < _newCollisionPairs.size(); i++)
		{
			//막 추가된 CollisionPair.
			const auto& pair = _newCollisionPairs[i];

			//해당 인덱스된 PhysicsCollision을 가져온다.
			auto& c = _collisions[pair];

			//OnCollisionEnter 함수를 발동.
			//OnCollisionExit 함수들 발동.
			if (!pair._first->GetTrigger())
			{
				pair._first->Collider_OnCollisionEnter(c);
			}
			c.SwapObjects();
			if (!pair._second->GetTrigger())
			{
				pair._second->Collider_OnCollisionEnter(c);
			}
			c.SwapObjects();
		}
	}

	void CollisionCallback::SendTriggerEvents()
	{

	}

	void CollisionCallback::OnColliderRemoved(HDData::Collider* collider)
	{

	}

	void CollisionCallback::onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs)
	{
		/*
		for (physx::PxU32 i = 0; i < nbPairs; i++)
		{
			const physx::PxContactPair& cp = pairs[i];
			if (cp.events & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
			{
				// 충돌이 감지되면 원하는 작업 수행
				physx::PxRigidDynamic* dynamicActor = nullptr;
				if (pairHeader.actors[0]->is<physx::PxRigidDynamic>())
				{
					dynamicActor = pairHeader.actors[0]->is<physx::PxRigidDynamic>();
				}
				else if (pairHeader.actors[1]->is<physx::PxRigidDynamic>())
				{
					dynamicActor = pairHeader.actors[1]->is<physx::PxRigidDynamic>();
				}
				if (dynamicActor)
				{
					dynamicActor->addForce(physx::PxVec3(0.0f, 100.0f, 0.0f), physx::PxForceMode::eIMPULSE);
				}
			}
		}
		*/

		using namespace physx;

		// 제거된 액터들에게는 이벤트를 보내지 않는다.
		if (pairHeader.flags & (PxContactPairHeaderFlag::eREMOVED_ACTOR_0 | PxContactPairHeaderFlag::eREMOVED_ACTOR_1))
		{
			return;
		}

		// 하나의 collision 마다 생성되는 객체
		HDData::PhysicsCollision col;
		col._thisVelocity = { 0.0f, 0.0f, 0.0f };
		col._otherVelocity = { 0.0f, 0.0f, 0.0f };

		// physics로부터 collision pair를 빼내기
		for (PxU32 pairIndex = 0; pairIndex < nbPairs; pairIndex++)
		{
			// 충돌된 ContactPair를 받는다.
			const PxContactPair& pair = pairs[pairIndex];

			// 압축된 Contact들의 압축된 스트림을 Read-Only로 접근할 수 있게 해주는 iterator.
			PxContactStreamIterator iter(pair.contactPatches, pair.contactPoints, pair.getInternalFaceIndices(), pair.patchCount, pair.contactCount);

			// Impulse 정보를 가져오기.
			const PxReal* impulses = pair.contactImpulses;
			const PxU32 flippedContacts = (pair.flags & PxContactPairFlag::eINTERNAL_CONTACTS_ARE_FLIPPED);

			// Impulse가 내부적으로 존재하는지 확인.
			const PxU32 hasImpulses = (pair.flags & PxContactPairFlag::eINTERNAL_HAS_IMPULSES);
			PxU32 nbContacts = 0;
			PxVec3 totalImpulse = PxVec3(0.0f);

			// 부딪힌 This / Other 액터 포인터 옮기기.
			col._thisActor = static_cast<HDData::Collider*>(pair.shapes[0]->userData);
			col._otherActor = static_cast<HDData::Collider*>(pair.shapes[1]->userData);

			// 둘 다 유효한지 확인.
			assert(col._thisActor != nullptr && col._otherActor != nullptr);

			// 스트림 내부에 더 읽어들일 수 있는 패치가 있을 때까지.
			while (iter.hasNextPatch())
			{
				// 다음 Patch로 Iterator++
				iter.nextPatch();

				// 다음 Contact가 있고, 감지할 수 있는 Contact의 포인트보다 작을 때 
				while (iter.hasNextContact() && nbContacts < HDData::PhysicsCollision::MAX_CONTACT_POINTS)
				{
					// 다음 Contact로 Iterator++
					iter.nextContact();

					// Impulse 정보 받기.
					const PxVec3 point = iter.getContactPoint();
					const PxVec3 normal = iter.getContactNormal();

					// TotalImpulse 기록하기. 
					if (hasImpulses)
					{
						totalImpulse += normal * impulses[nbContacts];
					}

					//[NOTUSED] PxU32 internalFaceIndex0 = flippedContacts ? iter.getFaceIndex1() : iter.getFaceIndex0();
					//[NOTUSED] PxU32 internalFaceIndex1 = flippedContacts ? iter.getFaceIndex0() : iter.getFaceIndex1();

					//Collision의 Contact Point를 제어하기 위해.
					HDData::PhysicsContactPoint& contact = col._contactPoints[nbContacts];
					contact._contactPoint = { point.x, point.y, point.z };
					contact._normal = { normal.x, normal.y, normal.z };
					contact._distance = iter.getSeparation();

					//Contact들의 개수를 + 1.
					nbContacts++;
				}
			}

			//PhysX 연동 추가 기록.
			col._activeContactCount = nbContacts;
			col._impulse = { totalImpulse.x, totalImpulse.y, totalImpulse.z };


			//Collision 목록 기록. (Callback 클래스에서)
			_collisions[CollidersPair(col._thisActor, col._otherActor)] = col;
		}

		// velocity 빼내기
		PxContactPairExtraDataIterator iter(pairHeader.extraDataStream, pairHeader.extraDataStreamSize);

		//다음 아이템 세트가 있을 때까지
		while (iter.nextItemSet())
		{
			//다음 Post Solver Velocity가 있을 경우에
			if (iter.postSolverVelocity)
			{
				const PxVec3 linearVelocityActor0 = iter.postSolverVelocity->linearVelocity[0];
				const PxVec3 linearVelocityActor1 = iter.postSolverVelocity->linearVelocity[1];

				//ContactPair에 접속.
				const PxContactPair& pair = pairs[iter.contactPairIndex];

				//콜라이더 Actor를 가져오는 과정이다.
				col._thisActor = static_cast<HDData::Collider*>(pair.shapes[0]->userData);
				col._otherActor = static_cast<HDData::Collider*>(pair.shapes[1]->userData);

				HDData::PhysicsCollision& collision = _collisions[CollidersPair(col._thisActor, col._otherActor)];
				//둘 다 유효한지 확인.
				assert(col._thisActor != nullptr && col._otherActor != nullptr);

				collision._thisVelocity = { linearVelocityActor0.x, linearVelocityActor0.y, linearVelocityActor0.z };
				collision._otherVelocity = { linearVelocityActor1.x, linearVelocityActor1.y, linearVelocityActor1.z };
			}
		}
	}

	void CollisionCallback::onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count)
	{
		return;
	}

	void CollisionCallback::onSleep(physx::PxActor** actors, physx::PxU32 count)
	{
		return;
	}

	void CollisionCallback::onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count)
	{
		/*
		for (physx::PxU32 i = 0; i < count; i++)
		{
			const physx::PxTriggerPair& pair = pairs[i];
			//physx::PxRigidDynamic* opponentActor = dynamic_cast<physx::PxRigidDynamic*>(pair.otherActor);
			physx::PxActor* movingActor = pair.otherActor;
			physx::PxRigidDynamic* dynamicActor = movingActor->is<physx::PxRigidDynamic>();
			dynamicActor->addForce(physx::PxVec3(0.0f, 1000.0f, 0.0f), physx::PxForceMode::eIMPULSE);
			//opponentActor->addForce(physx::PxVec3(0.0f, 10.0f, 0.0f), physx::PxForceMode::eIMPULSE);
		}
		*/

		using namespace physx;

		for (PxU32 i = 0; i < count; i++)
		{
			//Trigger Pair를 받아오기.
			const PxTriggerPair& pair = pairs[i];

			// 지워진 Shape들의 Trigger들은 무시하기.
			if (pair.flags & (PxTriggerPairFlag::eREMOVED_SHAPE_TRIGGER | PxTriggerPairFlag::eREMOVED_SHAPE_OTHER))
			{
				continue;
			}

			//Trigger들의 주소를 받기.

			HDData::Collider* trigger = static_cast<HDData::Collider*>(pair.triggerShape->userData);
			HDData::Collider* otherCollider = static_cast<HDData::Collider*>(pair.otherShape->userData);

			//둘 다 제대로 존재하는지 확인하기.
			assert(trigger != nullptr && otherCollider != nullptr);

			//otherCollider->Collider_OnTriggerEnter(trigger);

			//Collider들의 Pair 만들기.
			CollidersPair collidersPair(trigger, otherCollider);

			//상태를 확인했는데, 만약 이제 Trigger를 읽어버렸다면?
			if (pair.status & PxPairFlag::eNOTIFY_TOUCH_LOST)
			{
				//잃어버렸다고 표시.
				_lostTriggerPairs.emplace_back(collidersPair);
				trigger->AddCollisionCount(-1);
				otherCollider->AddCollisionCount(-1);
			}
			else
			{
				//새롭게 추가된 Trigger 목록에 추가.
				_newTriggerPairs.emplace_back(collidersPair);
				trigger->AddCollisionCount(1);
				otherCollider->AddCollisionCount(1);
			}
		}
	}

	void CollisionCallback::onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count)
	{
		return;
	}

	void CollisionCallback::onWake(physx::PxActor** actors, physx::PxU32 count)
	{
		return;
	}

	void CollisionCallback::ClearColliderFromCollection(HDData::Collider* collider, std::vector<CollidersPair>& collection)
	{
		// vector에서 특정 조건을 충족하는 요건을 제거
		collection.erase(std::remove_if(collection.begin(),
			collection.end(),
			[&collider](const CollidersPair& colPair) -> bool
			{ return (colPair._first == collider || colPair._second == collider); }),
			collection.end());
	}

	void CollisionCallback::ClearColliderFromCollection(HDData::Collider* collider, CollisionsPool& collection)
	{
		// unordered_map에서 특정 조건을 충족하는 요건을 제거
		const auto count = std::erase_if(collection, [&collider](const auto& item) {
			auto const& [key, value] = item;
			return (key._first == collider || key._second == collider);
			});
	}

}
