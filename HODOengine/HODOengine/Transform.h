#pragma once
#include "dllExporter.h"
#include "Component.h"
#include "MathHeader.h"

/// <summary>
/// Transform은 모든 GameObject가 공통적으로 가지고 있는 컴포넌트입니다.
/// Transform은 오브젝트의 position, rotation, scale 정보를 가지고 있습니다.
/// </summary>

namespace HDData
{
	class GameObject;
	class HODO_API Transform : public Component
	{
	public:		
		Transform();
		Vector3 GetPosition() const;			// world 기준
		Quaternion GetRotation() const;			// world 기준
		Vector3 GetScale() const;				// world 기준
		Vector3 GetLocalPosition() const;
		Quaternion GetLocalRotation() const;
		Vector3 GetLocalScale() const;
		Matrix GetWorldTM() const;
		Matrix GetLocalTM() const;
		Vector3 GetForward() const;				// world 기준
		Vector3 GetUp() const;					// world 기준
		Vector3 GetRight() const;				// world 기준
		void SetPosition(const Vector3& position);
		void SetPosition(float x, float y, float z);
		void SetRotation(const Quaternion& rotation);
		void SetRotation(float w, float x, float y, float z);
		void SetScale(const Vector3& scale);
		void SetScale(float x, float y, float z);
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

