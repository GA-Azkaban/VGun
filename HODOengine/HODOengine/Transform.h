#pragma once
#include "dllExporter.h"
#include "Component.h"
#include "MathHeader.h"

/// <summary>
/// Transform�� ��� GameObject�� ���������� ������ �ִ� ������Ʈ�Դϴ�.
/// Transform�� ������Ʈ�� position, rotation, scale ������ ������ �ֽ��ϴ�.
/// </summary>

namespace HDData
{
	class GameObject;
	class HODO_API Transform : public Component
	{
	public:		
		Transform();
		Vector3 GetWorldPosition() const;
		Quaternion GetWorldRotation() const;
		Vector3 GetWorldScale() const;
		Vector3 GetLocalPosition() const;
		Quaternion GetLocalRotation() const;
		Vector3 GetLocalScale() const;
		Matrix GetWorldTM() const;
		Matrix GetLocalTM() const;
		Vector3 GetForward() const;
		Vector3 GetUp() const;
		Vector3 GetRight() const;
		void SetWorldPosition(const Vector3& position);
		void SetWorldPosition(float x, float y, float z);
		void SetWorldRotation(const Quaternion& rotation);
		void SetWorldRotation(float w, float x, float y, float z);
		void SetWorldScale(const Vector3& scale);
		void SetLocalPosition(const Vector3& position);
		void SetLocalRotation(const Quaternion& rotation);
		void SetLocalScale(const Vector3& scale);

	public:
		void Translate(const Vector3& position);
		void Translate(float x, float y, float z);
		void Rotate(float angleX, float angleY, float angleZ);
		void Rotate(const Quaternion& quaternion);

	private:
		Vector3 _position;	// Local position of gameobject
		Quaternion _rotation;	// Local rotation of gameobject
		Vector3 _scale;	// Local scale of gameobject
	};
}

