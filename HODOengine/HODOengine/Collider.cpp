﻿#include "Collider.h"
#include "DebugSystem.h"
#include "RenderSystem.h"

namespace HDData
{

	Collider::Collider()
		: _positionOffset(Vector3::Zero)
		, _rotationOffset(Quaternion::Identity)
		, _scaleOffset(Vector3::One),
		_parentCollider(nullptr),
		_collisionFilterNum(0),
		_isTriggerType(false),
		_colType(eColliderRole::NONE),
		_colliderTag("")
	{
		
	}

	void Collider::SetPositionOffset(Vector3 pos)
	{
		_positionOffset = pos;
	}

	void Collider::SetRotationOffset(float x, float y, float z)
	{
		float yaw = DirectX::XMConvertToRadians(y);
		float pitch = DirectX::XMConvertToRadians(x);
		float roll = DirectX::XMConvertToRadians(z);
		_rotationOffset = Quaternion::CreateFromYawPitchRoll(yaw, pitch, roll);
	}

	void Collider::SetScaleOffset(Vector3 sca)
	{
		_scaleOffset = sca;
	}

	Vector3 Collider::GetPositionOffset()
	{
		return _positionOffset;
	}

	Quaternion Collider::GetRotationOffset()
	{
		return _rotationOffset;
	}

	Vector3 Collider::GetScaleOffset()
	{
		return _scaleOffset;
	}

	Matrix Collider::GetTranslateMatrix()
	{
		Matrix translateMatrix =
		{
			1,					0,					0,					0,
			0,					1,					0,					0,
			0,					0,					1,					0,
			_positionOffset.x,	_positionOffset.y,	_positionOffset.z,	1
		};

		return translateMatrix;
	}

	Matrix Collider::GetRotationMatrix()
	{
// 		Matrix rotationMatrix =
// 		{
// 			1.0f - 2.0f * (_rotationOffset.y * _rotationOffset.y + _rotationOffset.z * _rotationOffset.z),
// 			2.0f * (_rotationOffset.x * _rotationOffset.y + _rotationOffset.z * _rotationOffset.w),
// 			2.0f * (_rotationOffset.x * _rotationOffset.z - _rotationOffset.y * _rotationOffset.w),
// 			0,
// 
// 			2.0f * (_rotationOffset.x * _rotationOffset.y - _rotationOffset.z * _rotationOffset.w),
// 			1.0f - 2.0f * (_rotationOffset.x * _rotationOffset.x + _rotationOffset.z * _rotationOffset.z),
// 			2.0f * (_rotationOffset.y * _rotationOffset.z + _rotationOffset.x * _rotationOffset.w),
// 			0,
// 
// 			2.0f * (_rotationOffset.x * _rotationOffset.z + _rotationOffset.y * _rotationOffset.w),
// 			2.0f * (_rotationOffset.y * _rotationOffset.z - _rotationOffset.x * _rotationOffset.w),
// 			1.0f - 2.0f * (_rotationOffset.x * _rotationOffset.x + _rotationOffset.y * _rotationOffset.y),
// 			0,
// 
// 			0,
// 			0,
// 			0,
// 			1
// 		};

		Matrix rotationMatrix = Matrix::CreateFromQuaternion(_rotationOffset);

		return rotationMatrix;
	}

	Matrix Collider::GetScaleMatrix()
	{		
// 		Matrix scaleMatrix =
// 		{
// 			_scaleOffset.x,		0,					0,					0,
// 			0,					_scaleOffset.y,		0,					0,
// 			0,					0,					_scaleOffset.z,		0,
// 			0,					0,					0,					1
// 		};

		Matrix scaleMatrix = Matrix::CreateScale(_scaleOffset);

		return scaleMatrix;
	}

	Matrix Collider::GetTransformMatrix()
	{
		Matrix result = GetScaleMatrix() * GetRotationMatrix() * GetTranslateMatrix();
		//Matrix result = GetTranslateMatrix()  * GetRotationMatrix() * GetScaleMatrix();

		if (_parentCollider != nullptr)
		{
			result = _parentCollider->GetTransformMatrix() * result;
		}

		return result;
		//return Matrix::Identity * GetScaleMatrix() * GetRotationMatrix() * GetTranslateMatrix();
	}

	void Collider::SetColliderTag(const std::string& tag)
	{
		_colliderTag = tag;
	}

	const std::string& Collider::GetColliderTag()
	{
		return _colliderTag;
	}

	void Collider::Setflag(int flag)
	{
		_flag = flag;
	}

	int Collider::GetFlag()
	{
		return _flag;
	}

	void Collider::SetTrigger(bool isTrigger)
	{
		_isTriggerType = isTrigger;
	}

	bool Collider::GetIsTriggerType() const
{
		return _isTriggerType;
	}

	bool Collider::GetIsTriggerCollide() const
	{
		return _isTriggerCollide;
	}

	bool Collider::GetPrevIsTriggerCollide() const
	{
		return _prevIsTriggerCollide;
	}

	int Collider::GetColFilterNum() const
	{
		return _collisionFilterNum;
	}

	bool Collider::GetIsCollide() const
	{
		return _isCollide;
	}

	bool Collider::GetPrevIsCollide() const
	{
		return _prevIsCollide;
	}

	std::vector<PhysicsCollision*> Collider::GetCollisionStorage() const
	{
		return _collisionStorage;
	}

	std::vector<Collider*> Collider::GetTriggerStorage() const
	{
		return _triggerStorage;
	}

	eColliderRole Collider::GetColType() const
	{
		return _colType;
	}

	void Collider::SetParentCollider(HDData::Collider* col)
	{
		_parentCollider = col;
		col->SetChildCollider(this);
	}

	void Collider::SetChildCollider(HDData::Collider* childCol)
	{
		_childColliders.push_back(childCol);
	}

	HDData::Collider* Collider::GetParentCollider() const
{
		return _parentCollider;
	}

	void Collider::Flush()
	{
		_prevIsCollide = false;
		_isCollide = false;

		_prevIsTriggerCollide = false;
		_isTriggerCollide = false;

		//매 프레임 체크할 때 마다 초기화.
		_collisionStorage.clear();
		_triggerStorage.clear();
	}

	void Collider::OnCollision(Collider* opponent, int actionType)
	{
		assert(actionType < 3, "action type index must be lower than 2");

		(*_callbackFunctionVec[actionType])(this, opponent);
	}

	void Collider::AddCollisionCallback(CollisionCallback* callbackFunc, int actionType)
	{
		assert(actionType < 3, "action type index must be lower than 2");

		_callbackFunctionVec[actionType] = callbackFunc;
	}

	void Collider::Collider_OnCollisionEnter(PhysicsCollision& collision)
	{
		std::string tRes = "Collider_OnCollisionEnter : ";
		//PG_TRACE(tRes.append(this->_object->GetName()).c_str());

		//bool값을 변경해주고 상태를 설정해줘야 Object의 이벤트와 연결이 가능하다.
		this->_isCollide = true;
		this->_prevIsCollide = false;
		_collisionStorage.push_back(&collision);
	}

	void Collider::Collider_OnCollisionStay(PhysicsCollision& collision)
	{
		std::string tRes = "Collider_OnCollisionStay : ";

		//bool값을 변경해주고 상태를 설정해줘야 Object의 이벤트와 연결이 가능하다.
		this->_isCollide = true;
		this->_prevIsCollide = true;
		_collisionStorage.push_back(&collision);
	}

	void Collider::Collider_OnCollisionExit(PhysicsCollision& collision)
	{
		std::string tRes = "Collider_OnCollisionExit : ";
		//PG_TRACE(tRes.append(this->_object->GetName()).c_str());

		this->_isCollide = false;
		this->_prevIsCollide = true;
		_collisionStorage.push_back(&collision);
	}

	void Collider::Collider_OnTriggerEnter(Collider* col)
	{
		std::string tRes = "Collider_OnTriggerEnter : ";

		this->_isTriggerCollide = true;
		this->_prevIsTriggerCollide = false;
		_triggerStorage.push_back(col);
	}

	void Collider::Collider_OnTriggerExit(Collider* col)
	{
		std::string tRes = "Collider_OnTriggerExit : ";

		this->_isTriggerCollide = false;
		this->_prevIsTriggerCollide = true;
		_triggerStorage.push_back(col);
	}
}
