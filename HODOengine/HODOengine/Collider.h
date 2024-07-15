#pragma once
#include "Component.h"
#include "dllExporter.h"
#include "MathHeader.h"
#include "PhysicsCollision.h"
#include "../HODO3DGraphicsInterface/PrimitiveHeader.h"

///
/// ���� ������Ʈ���� ���� ū ���̽� Ŭ����.
/// �ݶ��̴����� Transform�� ���õ� �������� �ٷ��.
/// 

enum class eColliderRole
{
	NONE = 0,
	PLAYER = 1,
	TERRAIN = 2
};

enum class eColliderType
{
	BOX = 1,
	SPHERE = 2,
	CAPSULE = 3
};

namespace HDData
{
	class HODO_API Collider : public Component
	{
	public:
		Collider();

	public:
		virtual void UpdateToPhysics() abstract;

	public:
		void SetPositionOffset(Vector3 pos);
		void SetRotationOffset(float x, float y, float z);
		void SetScaleOffset(Vector3 sca);

		Vector3 GetPositionOffset();
		Quaternion GetRotationOffset();
		Vector3 GetScaleOffset();

		Matrix GetTranslateMatrix();
		Matrix GetRotationMatrix();
		Matrix GetScaleMatrix();

		Matrix GetTransformMatrix();

	protected:
		Vector3 _positionOffset;
		Quaternion _rotationOffset;
		Vector3 _scaleOffset;
		HDEngine::PrimitiveBase* _debugStruct;

	public:
		void Setflag(int flag);
		int GetFlag();
		void SetTrigger(bool isTrigger);
		bool GetIsTriggerType() const;
		bool GetIsTriggerCollide() const;
		bool GetPrevIsTriggerCollide() const;
		int GetColFilterNum() const;
		bool GetIsCollide() const;
		bool GetPrevIsCollide() const;
		std::vector<PhysicsCollision*> GetCollisionStorage() const;
		std::vector<Collider*> GetTriggerStorage() const;
		eColliderRole GetColType() const;
		void SetParentCollider(HDData::Collider* col);
		void SetChildCollider(HDData::Collider* childCol);
		Collider* GetParentCollider() const;

		void Flush();

	protected:
		int _flag;
		bool _isTriggerType;
		bool _isTriggerCollide;
		bool _prevIsTriggerCollide;
		bool _isCollide;
		bool _prevIsCollide;
		int _collisionFilterNum;

	// deal with the collision by callback function
	public:
		typedef void (*CollisionCallback)(Collider* self, Collider* opponent);
		void OnCollision(Collider* opponent, int actionType);
		void AddCollisionCallback(CollisionCallback* callbackFunc, int actionType);

		// copy ver
		void Collider_OnCollisionEnter(PhysicsCollision& collision);
		void Collider_OnCollisionStay(PhysicsCollision& collision);
		void Collider_OnCollisionExit(PhysicsCollision& collision);
		void Collider_OnTriggerEnter(Collider* col);
		void Collider_OnTriggerExit(Collider* col);

	// index 0 ~ 2 is OnCollision_[Enter / Stay / Exit]
	private:
		std::vector<CollisionCallback*> _callbackFunctionVec;

		// copy ver
		std::vector<PhysicsCollision*> _collisionStorage;
		std::vector<Collider*> _triggerStorage;

	protected:
		HDData::Collider* _parentCollider;
		std::vector<HDData::Collider*> _childColliders;
		eColliderRole _colType;
	};
}


