#include "Transform.h"
#include "GameObject.h"

namespace HDData
{
	Transform::Transform()
		:_position(0.0f, 0.0f, 0.0f), _rotation(0.0f, 0.0f, 0.0f, 1.0f), _scale(Vector3::One)
	{

	}

	Vector3 Transform::GetPosition() const
	{
		/// 이득우의 게임수학 p.619 참조
		// 내 로컬 포지션에 부모의 월드 트랜스폼을 반영해 내 월드 포지션을 구한다.
		if (GetGameObject()->GetParentGameObject() == nullptr)
		{
			return _position;
		}

		Matrix parentMatrix = GetGameObject()->GetParentGameObject()->GetTransform()->GetWorldTM();

		return Vector3::Transform(_position, parentMatrix);

		// 		Vector3 parentPosition = GetGameObject()->GetParentGameObject()->GetTransform()->GetWorldPosition();
		// 		Quaternion parentRotation = GetGameObject()->GetParentGameObject()->GetTransform()->GetWorldRotation();
		// 		Vector3 parentScale = GetGameObject()->GetParentGameObject()->GetTransform()->GetWorldScale();
		// 
		// 		return parentPosition + parentRotation * (parentScale * _position);
	}

	Quaternion Transform::GetRotation() const
	{
		if (GetGameObject()->GetParentGameObject() == nullptr)
		{
			return _rotation;
		}

		Quaternion parentQuat = GetGameObject()->GetParentGameObject()->GetTransform()->GetRotation();

		return Quaternion::Concatenate(_rotation, parentQuat);
	}

	Vector3 Transform::GetScale() const
	{
		if (GetGameObject()->GetParentGameObject() == nullptr)
		{
			return _scale;
		}

		Vector3 parentScale = GetGameObject()->GetParentGameObject()->GetTransform()->GetScale();
		Vector3 result = { 1.f, 1.f, 1.f };
		result *= _scale;
		result *= parentScale;

		return result;
	}

	Vector3 Transform::GetLocalPosition() const
	{
		return _position;
	}

	Quaternion Transform::GetLocalRotation() const
	{
		return _rotation;
	}

	Vector3 Transform::GetLocalScale() const
	{
		return _scale;
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
		result *= Matrix::CreateScale(_scale);
		result *= Matrix::CreateFromQuaternion(_rotation);
		result *= Matrix::CreateTranslation(_position);

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

		_position = result;

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

		_rotation = result;
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

		_scale = result;

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
		_position = position;
	}

	void Transform::SetLocalPosition(float x, float y, float z)
	{
		const Vector3 pos { x, y, z };
		SetLocalPosition(pos);
	}

	void Transform::SetLocalRotation(const Quaternion& rotation)
	{
		_rotation = rotation;
	}

	void Transform::SetLocalScale(const Vector3& scale)
	{
		_scale = scale;
	}

	void Transform::Translate(const Vector3& position)
	{
		_position.x += position.x;
		_position.y += position.y;
		_position.z += position.z;
	}

	void Transform::Translate(float x, float y, float z)
	{
		_position.x += x;
		_position.y += y;
		_position.z += z;
	}

	void Transform::Rotate(float angleX, float angleY, float angleZ)
	{
		float x = DirectX::XMConvertToRadians(angleX);
		float y = DirectX::XMConvertToRadians(angleY);
		float z = DirectX::XMConvertToRadians(angleZ);

		Quaternion rotQuat = Quaternion::CreateFromYawPitchRoll({ x,y,z });

		_rotation = Quaternion::Concatenate(_rotation, rotQuat);
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
		_rotation = Quaternion::Concatenate(_rotation, quaternion);
	}

}
