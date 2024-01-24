﻿#pragma once
#include "Component.h"
#include "dllExporter.h"
#include "MathHeader.h"
#include "../HODO3DGraphicsInterface/PrimitiveHeader.h"

///
/// ���� ������Ʈ���� ���� ū ���̽� Ŭ����.
/// �ݶ��̴����� Transform�� ���õ� �������� �ٷ��.
/// 

namespace HDData
{
	class HODO_API Collider : public Component
	{
	public:
		Collider();

	public:
		virtual void UpdateToPhysics() abstract;
		virtual void DrawDebug() abstract;

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

	protected:
		int _flag;
	};
}


