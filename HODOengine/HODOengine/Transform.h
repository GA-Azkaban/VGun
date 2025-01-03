﻿#pragma once
#include "dllExporter.h"
#include "Component.h"
#include "MathHeader.h"
#include "../HODOCommon/HDTransform.h"

/// <summary>
/// Transform은 모든 GameObject가 공통적으로 가지고 있는 컴포넌트입니다.
/// Transform은 오브젝트의 position, rotation, scale 정보를 가지고 있습니다.
/// </summary>

namespace HDData
{
	class HODO_API Transform : public Component
	{
	public:
		friend class GameObject;
		Transform();
		Vector3 GetPosition() const;			// world 기준
		Vector3& GetPositionRef();
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

		Vector3 GetNodePosition() const;
		Quaternion GetNodeRotation() const;
		Vector3 GetNodeScale() const;

		void SetWorldTM(const Matrix& worldTM);
		void SetLocalTM(const Matrix& localTM);
		void SetPosition(const Vector3& position);
		void SetPosition(float x, float y, float z);
		void SetPositionFromPhysics(float x, float y, float z);
		void SetRotation(const Quaternion& rotation);
		void SetRotationFromPhysics(const Quaternion& rotation);
		void SetRotation(float w, float x, float y, float z);
		void SetScale(const Vector3& scale);
		void SetScale(float x, float y, float z);
		void SetLocalPosition(const Vector3& position);
		void SetLocalPosition(float x, float y, float z);
		void SetLocalRotation(const Quaternion& rotation);
		void SetLocalRotation(float x, float y, float z, float w);
		void SetLocalRotationEuler(Vector3 eulerAngle);
		void SetLocalScale(const Vector3& scale);
		void SetLocalScale(float x, float y, float z);

	public:
		void Translate(const Vector3& position);
		void Translate(float x, float y, float z);
		void Rotate(float angleX, float angleY, float angleZ);
		void Rotate(const Quaternion& quaternion);

	private:
		HDCommon::Transform* _nodeTransform;
	};
}

