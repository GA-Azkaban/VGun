﻿#include "Transform.h"
#include "GameObject.h"
#include "DynamicCapsuleCollider.h"

namespace HDData
{
	Transform::Transform()
		: _nodeTransform(new HDCommon::Transform())
	{

	}

	Vector3 Transform::GetPosition() const
	{
		/// 이득우의 게임수학 p.619 참조
		// 내 로컬 포지션에 부모의 월드 트랜스폼을 반영해 내 월드 포지션을 구한다.
		if (GetGameObject()->GetParentGameObject() == nullptr)
		{
			return _nodeTransform->_position;
		}

		Matrix parentMatrix = GetGameObject()->GetParentGameObject()->GetTransform()->GetWorldTM();

		return Vector3::Transform(_nodeTransform->_position, parentMatrix);

		// 		Vector3 parentPosition = GetGameObject()->GetParentGameObject()->GetTransform()->GetWorldPosition();
		// 		Quaternion parentRotation = GetGameObject()->GetParentGameObject()->GetTransform()->GetWorldRotation();
		// 		Vector3 parentScale = GetGameObject()->GetParentGameObject()->GetTransform()->GetWorldScale();
		// 
		// 		return parentPosition + parentRotation * (parentScale * _position);
	}

	Vector3& Transform::GetPositionRef()
	{
		if (GetGameObject()->GetParentGameObject() == nullptr)
		{
			return _nodeTransform->_position;
		}
	}

	Quaternion Transform::GetRotation() const
	{
		if (GetGameObject()->GetParentGameObject() == nullptr)
		{
			return _nodeTransform->_rotation;
		}

		Quaternion parentQuat = GetGameObject()->GetParentGameObject()->GetTransform()->GetRotation();

		return Quaternion::Concatenate(_nodeTransform->_rotation, parentQuat);
	}

	Vector3 Transform::GetScale() const
	{
		if (GetGameObject()->GetParentGameObject() == nullptr)
		{
			return _nodeTransform->_scale;
		}

		Vector3 parentScale = GetGameObject()->GetParentGameObject()->GetTransform()->GetScale();
		Vector3 result = { 1.f, 1.f, 1.f };
		result *= _nodeTransform->_scale;
		result *= parentScale;

		return result;
	}

	Vector3 Transform::GetLocalPosition() const
	{
		return _nodeTransform->_position;
	}

	Quaternion Transform::GetLocalRotation() const
	{
		return _nodeTransform->_rotation;
	}

	Vector3 Transform::GetLocalScale() const
	{
		return _nodeTransform->_scale;
	}

	Matrix Transform::GetWorldTM() const
	{
		if (GetGameObject()->GetParentGameObject() == nullptr)
		{
			return GetLocalTM();
		}

		Matrix parentWorldTM = GetGameObject()->GetParentGameObject()->GetTransform()->GetWorldTM();
		Matrix result = DirectX::XMMatrixMultiply(GetLocalTM(), parentWorldTM);

		return result;
	}

	Matrix Transform::GetLocalTM() const
	{
		Matrix result;
		result *= Matrix::CreateScale(_nodeTransform->_scale);
		result *= Matrix::CreateFromQuaternion(_nodeTransform->_rotation);
		result *= Matrix::CreateTranslation(_nodeTransform->_position);

		return result;
	}

	Vector3 Transform::GetForward() const
	{
		Matrix rotMatrix;
		rotMatrix = Matrix::CreateFromQuaternion(GetRotation());

		return Vector3::Transform(Vector3(0.0f, 0.0f, 1.0f), rotMatrix);
	}

	Vector3 Transform::GetUp() const
	{
		Matrix rotMatrix;
		rotMatrix = Matrix::CreateFromQuaternion(GetRotation());

		return Vector3::Transform(Vector3(0.0f, 1.0f, 0.0f), rotMatrix);
	}

	Vector3 Transform::GetRight() const
	{
		Matrix rotMatrix;
		rotMatrix = Matrix::CreateFromQuaternion(GetRotation());

		return Vector3::Transform(Vector3(1.0f, 0.0f, 0.0f), rotMatrix);
	}

	Vector3 Transform::GetNodePosition() const
	{
		return _nodeTransform->_position;
	}

	Quaternion Transform::GetNodeRotation() const
	{
		return _nodeTransform->_rotation;
	}

	Vector3 Transform::GetNodeScale() const
	{
		return _nodeTransform->_scale;
	}

	void Transform::SetWorldTM(const Matrix& worldTM)
	{
		DirectX::XMVECTOR scale;
		DirectX::XMVECTOR rotate;
		DirectX::XMVECTOR translate;
		DirectX::XMMatrixDecompose(&scale, &rotate, &translate, worldTM);
		SetPosition(translate.m128_f32[0], translate.m128_f32[1], translate.m128_f32[2]);
		SetRotation(rotate.m128_f32[0], rotate.m128_f32[1], rotate.m128_f32[2], rotate.m128_f32[3]);
		SetScale(scale.m128_f32[0], scale.m128_f32[1], scale.m128_f32[2]);
	}

	void Transform::SetLocalTM(const Matrix& localTM)
	{
		DirectX::XMVECTOR scale;
		DirectX::XMVECTOR rotate;
		DirectX::XMVECTOR translate;
		DirectX::XMMatrixDecompose(&scale, &rotate, &translate, localTM);
		SetLocalPosition(translate.m128_f32[0], translate.m128_f32[1], translate.m128_f32[2]);
		SetLocalRotation(rotate.m128_f32[0], rotate.m128_f32[1], rotate.m128_f32[2], rotate.m128_f32[3]);
		SetLocalScale(scale.m128_f32[0], scale.m128_f32[1], scale.m128_f32[2]);
	}

	void Transform::SetPosition(const Vector3& position)
	{
		SetPosition(position.x, position.y, position.z);
	}

	void Transform::SetPosition(float x, float y, float z)
	{
		Vector3 result = { x,y,z };
		GameObject* parent = GetGameObject()->GetParentGameObject();

		if (parent)
		{
			result = Vector3::Transform(result, parent->GetTransform()->GetWorldTM().Invert());
		}

		_nodeTransform->_position = result;

		auto bodyCol = GetGameObject()->GetComponent<HDData::DynamicCapsuleCollider>();
		if (bodyCol != nullptr)
		{
			bodyCol->SetColliderPosition(Vector3(x,y,z));
		}

		// 		else
// 		{
// 			// world position = position;
// 			// 월드 포지션을 통해 로컬 포지션 갱신
// 			// 부모 월드 역행렬에 내 월드를 곱하면 로컬
// 			Matrix worldTM = 
// 			worldTM *= Matrix::CreateScale(_scale);
// 			worldTM *= Matrix::CreateFromQuaternion(_rotation);
// 			worldTM *= Matrix::CreateTranslation(position);
// 
// 			Matrix invParent = Matrix::Identity;
// 			invParent = GetGameObject()->GetParentGameObject()->GetTransform()->GetWorldTM().Invert();
// 			invParent *= worldTM;
// 
// 			Vector3 temp1;
// 			Quaternion temp2;
// 			invParent.Decompose(temp1, temp2, _position);
// 		}
	}

	void Transform::SetPositionFromPhysics(float x, float y, float z)
	{
		Vector3 result = { x,y,z };
		GameObject* parent = GetGameObject()->GetParentGameObject();

		if (parent)
		{
			result = Vector3::Transform(result, parent->GetTransform()->GetWorldTM().Invert());
		}

		_nodeTransform->_position = result;
	}

	void Transform::SetRotation(const Quaternion& rotation)
	{
		SetRotation(rotation.x, rotation.y, rotation.z, rotation.w);
	}

	void Transform::SetRotation(float x, float y, float z, float w)
	{
		Quaternion result = { x,y,z,w };
		GameObject* parent = GetGameObject()->GetParentGameObject();

		if (parent)
		{
			Quaternion parentRot = parent->GetTransform()->GetRotation();
			parentRot.Conjugate();

			result = Quaternion::Concatenate(result, parentRot);
		}

		_nodeTransform->_rotation = result;

		auto bodyCol = GetGameObject()->GetComponent<HDData::DynamicCapsuleCollider>();
		if (bodyCol != nullptr)
		{
			bodyCol->SetColliderRotation(Quaternion(x,y,z,w));
		}
	}

	void Transform::SetRotationFromPhysics(const Quaternion& rotation)
	{
		Quaternion result = rotation;
		GameObject* parent = GetGameObject()->GetParentGameObject();

		if (parent)
		{
			Quaternion parentRot = parent->GetTransform()->GetRotation();
			parentRot.Conjugate();

			result = Quaternion::Concatenate(result, parentRot);
		}

		_nodeTransform->_rotation = result;
	}

	void Transform::SetScale(const Vector3& scale)
	{
		SetScale(scale.x, scale.y, scale.z);
	}

	void Transform::SetScale(float x, float y, float z)
	{
		Vector3 result = { x,y,z };
		GameObject* parent = GetGameObject()->GetParentGameObject();

		if (parent)
		{
			Vector3 inverseScale = parent->GetTransform()->GetScale();
			inverseScale.x = 1 / inverseScale.x;
			inverseScale.y = 1 / inverseScale.y;
			inverseScale.z = 1 / inverseScale.z;

			result *= inverseScale;
		}

		_nodeTransform->_scale = result;

		// 		else
		// 		{
		// 			Matrix worldTM = GetTransformMatrix(_position, _rotation, scale);
		// 			Matrix invParent = Matrix::Identity;
		// 			invParent = GetGameObject()->GetParentGameObject()->GetTransform()->GetWorldTM().Inverse();
		// 			_scale = GetLocalScaleFromLocalTM(invParent * worldTM);
		// 		}
	}

	void Transform::SetLocalPosition(const Vector3& position)
	{
		_nodeTransform->_position = position;
	}

	void Transform::SetLocalPosition(float x, float y, float z)
	{
		_nodeTransform->_position.x = x;
		_nodeTransform->_position.y = y;
		_nodeTransform->_position.z = z;
	}

	void Transform::SetLocalRotation(const Quaternion& rotation)
	{
		_nodeTransform->_rotation = rotation;
	}

	void Transform::SetLocalRotation(float x, float y, float z, float w)
	{
		_nodeTransform->_rotation.x = x;
		_nodeTransform->_rotation.y = y;
		_nodeTransform->_rotation.z = z;
		_nodeTransform->_rotation.w = w;
	}

	void Transform::SetLocalRotationEuler(Vector3 eulerAngle)
	{
		float radianX = DirectX::XMConvertToRadians(eulerAngle.x);
		float radianY = DirectX::XMConvertToRadians(eulerAngle.y);
		float radianZ = DirectX::XMConvertToRadians(eulerAngle.z);

		// 회전 각도를 반으로 나누어 준비합니다.
		float half_radianX = radianX * 0.5f;
		float half_radianY = radianY * 0.5f;
		float half_radianZ = radianZ * 0.5f;

		// 각 축의 쿼터니언 요소를 계산합니다.
		float sin_half_radianX = sin(half_radianX);
		float sin_half_radianY = sin(half_radianY);
		float sin_half_radianZ = sin(half_radianZ);
		float cos_half_radianX = cos(half_radianX);
		float cos_half_radianY = cos(half_radianY);
		float cos_half_radianZ = cos(half_radianZ);

		_nodeTransform->_rotation.w = cos_half_radianX * cos_half_radianY * cos_half_radianZ - sin_half_radianX * sin_half_radianY * sin_half_radianZ;
		_nodeTransform->_rotation.x = sin_half_radianX * cos_half_radianY * cos_half_radianZ + cos_half_radianX * sin_half_radianY * sin_half_radianZ;
		_nodeTransform->_rotation.y = cos_half_radianX * sin_half_radianY * cos_half_radianZ - sin_half_radianX * cos_half_radianY * sin_half_radianZ;
		_nodeTransform->_rotation.z = cos_half_radianX * cos_half_radianY * sin_half_radianZ + sin_half_radianX * sin_half_radianY * cos_half_radianZ;

	}

	void Transform::SetLocalScale(const Vector3& scale)
	{
		_nodeTransform->_scale = scale;
	}

	void Transform::SetLocalScale(float x, float y, float z)
	{
		_nodeTransform->_scale.x = x;
		_nodeTransform->_scale.y = y;
		_nodeTransform->_scale.z = z;
	}

	void Transform::Translate(const Vector3& position)
	{
		_nodeTransform->_position.x += position.x;
		_nodeTransform->_position.y += position.y;
		_nodeTransform->_position.z += position.z;
	}

	void Transform::Translate(float x, float y, float z)
	{
		Vector3 delta;
		delta.x = GetRight().x * x + GetForward().x * z + GetUp().x * y;
		delta.z = GetForward().z * z + GetRight().z * x + GetUp().z * y;
		delta.y = GetUp().y * y + GetForward().y * z + GetRight().y * x;

		_nodeTransform->_position.x = delta.x;
		_nodeTransform->_position.y = delta.y;
		_nodeTransform->_position.z = delta.z;
	}

	void Transform::Rotate(float angleX, float angleY, float angleZ)
	{
		float x = DirectX::XMConvertToRadians(angleX);
		float y = DirectX::XMConvertToRadians(angleY);
		float z = DirectX::XMConvertToRadians(angleZ);

		Quaternion rotQuat = Quaternion::CreateFromYawPitchRoll({ x,y,z });

		_nodeTransform->_rotation = Quaternion::Concatenate(_nodeTransform->_rotation, rotQuat);
		// 
		// 		float radianX = HDMath::ToRadian(angleX);
		// 		float radianY = HDMath::ToRadian(angleY);
		// 		float radianZ = HDMath::ToRadian(angleZ);
		// 
		// 		// 회전 각도를 반으로 나누어 준비합니다.
		// 		float half_radianX = radianX * 0.5f;
		// 		float half_radianY = radianY * 0.5f;
		// 		float half_radianZ = radianZ * 0.5f;
		// 
		// 		// 각 축의 쿼터니언 요소를 계산합니다.
		// 		float sin_half_radianX = sin(half_radianX);
		// 		float sin_half_radianY = sin(half_radianY);
		// 		float sin_half_radianZ = sin(half_radianZ);
		// 		float cos_half_radianX = cos(half_radianX);
		// 		float cos_half_radianY = cos(half_radianY);
		// 		float cos_half_radianZ = cos(half_radianZ);
		// 
		// 		// 쿼터니언 요소를 계산합니다.
		// 		Vector4 rotQuat;
		// 		rotQuat.x = sin_half_radianX * cos_half_radianY * cos_half_radianZ + cos_half_radianX * sin_half_radianY * sin_half_radianZ;
		// 		rotQuat.y = cos_half_radianX * sin_half_radianY * cos_half_radianZ - sin_half_radianX * cos_half_radianY * sin_half_radianZ;
		// 		rotQuat.z = cos_half_radianX * cos_half_radianY * sin_half_radianZ + sin_half_radianX * sin_half_radianY * cos_half_radianZ;
		// 		rotQuat.w = cos_half_radianX * cos_half_radianY * cos_half_radianZ - sin_half_radianX * sin_half_radianY * sin_half_radianZ;
		// 
		// 		// 원본 쿼터니언과 회전 쿼터니언의 곱으로 결과 쿼터니언을 계산합니다.
		// 		Quaternion result;
		// 		result.x = _rotation.w * rotQuat.x + _rotation.x * rotQuat.w + _rotation.y * rotQuat.z - _rotation.z * rotQuat.y;
		// 		result.y = _rotation.w * rotQuat.y - _rotation.x * rotQuat.z + _rotation.y * rotQuat.w + _rotation.z * rotQuat.x;
		// 		result.z = _rotation.w * rotQuat.z + _rotation.x * rotQuat.y - _rotation.y * rotQuat.x + _rotation.z * rotQuat.w;
		// 		result.w = _rotation.w * rotQuat.w - _rotation.x * rotQuat.x - _rotation.y * rotQuat.y - _rotation.z * rotQuat.z;
		// 
		// 		// 결과를 저장합니다.
		// 		_rotation = result;
	}

	void Transform::Rotate(const Quaternion& quaternion)
	{
		_nodeTransform->_rotation = Quaternion::Concatenate(_nodeTransform->_rotation, quaternion);
	}

}
