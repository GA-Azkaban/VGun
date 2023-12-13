#pragma once
#include <functional>

/// <summary>
/// 23.10.31 ������
/// ���� ���� ���̺귯��
/// RocketMath�� Overload�� OvMath ����
/// </summary>

namespace HDMath
{

	struct HDFLOAT3X3;

	constexpr float HD_PI = 3.141592654f;
	constexpr float HD_2PI = 6.283185307f;
	constexpr float HD_1DIVPI = 0.318309886f;
	constexpr float HD_1DIV2PI = 0.159154943f;
	constexpr float HD_PIDIV2 = 1.570796327f;
	constexpr float HD_PIDIV4 = 0.785398163f;
	
	constexpr float ToRadian(float degree)
	{
		return degree * HD_PI / 180.f;
	}
	constexpr float ToDegree(float rad)
	{
		return rad * 180.f / HD_PI;
	}

	struct HDFLOAT2
	{
		float x;
		float y;

		// �⺻ ������
		HDFLOAT2(float x = 0.0f, float y = 0.0f);

		// ���� ������
		HDFLOAT2(const HDFLOAT2& val);

		// �̵� ������
		HDFLOAT2(HDFLOAT2&& val) noexcept = default;

		// ������ �����ε�
		HDFLOAT2 operator-() const; // ���� ��ȯ
		HDFLOAT2 operator=(const HDFLOAT2& other); // ���� ������
		HDFLOAT2 operator+(const HDFLOAT2& other) const; // ���ϱ�
		HDFLOAT2& operator+=(const HDFLOAT2& other); // ���ؼ� ����
		HDFLOAT2 operator-(const HDFLOAT2& other) const; // ����
		HDFLOAT2& operator-=(const HDFLOAT2& other); // ���� ����

		// ���Ϳ� ��Į�� ����
		HDFLOAT2 operator* (float scalar); // ���Ϳ� ��Į�� ��
		HDFLOAT2& operator*=(float scalar); // ���Ϳ� ��Į�� �� ����
		HDFLOAT2 operator/ (float scalar); // ���Ϳ� ��Į�� ������
		HDFLOAT2& operator/=(float scalar); // ���Ϳ� ��Į�� ������ ����

		// �񱳿�����
		bool operator==(const HDFLOAT2& other);
		bool operator!=(const HDFLOAT2& other);

		// ���� ����
		static HDFLOAT2 Add(const HDFLOAT2& left, const HDFLOAT2& right);
		static HDFLOAT2 Substract(const HDFLOAT2& left, const HDFLOAT2& right);
		static HDFLOAT2 Multiply(const HDFLOAT2& val, float scalar);
		static HDFLOAT2 Divide(const HDFLOAT2& val, float scalar);
		static float Dot(const HDFLOAT2& left, const HDFLOAT2& right); // ����
		static HDFLOAT2 Normalize(const HDFLOAT2& val); // ����ȭ

	};

	struct HDFLOAT3
	{
		float x;
		float y;
		float z;

		// �⺻ ������
		HDFLOAT3(float x = 0.0f, float y = 0.0f, float z = 0.0f);

		// ���� ������
		HDFLOAT3(const HDFLOAT3& val);

		// �̵� ������
		HDFLOAT3(HDFLOAT3&& val) noexcept = default;

		// ������ �����ε�
		HDFLOAT3 operator-() const; // ���� ��ȯ
		HDFLOAT3 operator=(const HDFLOAT3& other); // ���� ������
		HDFLOAT3 operator+(const HDFLOAT3& other) const; // ���ϱ�
		HDFLOAT3 operator*(const HDFLOAT3& other) const; // ���ϱ�
		HDFLOAT3& operator+=(const HDFLOAT3& other); // ���ؼ� ����
		HDFLOAT3 operator-(const HDFLOAT3& other) const; // ����
		HDFLOAT3& operator-=(const HDFLOAT3& other); // ���� ����

		// ���Ϳ� ��Į�� ����
		HDFLOAT3 operator* (float scalar); // ���Ϳ� ��Į�� ��
		HDFLOAT3& operator*=(float scalar); // ���Ϳ� ��Į�� �� ����
		HDFLOAT3 operator/ (float scalar); // ���Ϳ� ��Į�� ������
		HDFLOAT3& operator/=(float scalar); // ���Ϳ� ��Į�� ������ ����

		// �񱳿�����
		bool operator==(const HDFLOAT3& other);
		bool operator!=(const HDFLOAT3& other);
		bool operator<(const HDFLOAT3& other);
		bool operator<=(const HDFLOAT3& other);
		bool operator>(const HDFLOAT3& other);
		bool operator>=(const HDFLOAT3& other);

		// ���� ����
		static HDFLOAT3 Add(const HDFLOAT3& left, const HDFLOAT3& right);
		static HDFLOAT3 Substract(const HDFLOAT3& left, const HDFLOAT3& right);
		static HDFLOAT3 Multiply(const HDFLOAT3& val, float scalar);
		static HDFLOAT3 Divide(const HDFLOAT3& val, float scalar);
		static float Dot(const HDFLOAT3& left, const HDFLOAT3& right); // ����
		static HDFLOAT3 Cross(const HDFLOAT3& a, const HDFLOAT3& b); // ����
		static HDFLOAT3 Normalize(const HDFLOAT3& val); // ����ȭ

		static const HDFLOAT3 zero;
		static const HDFLOAT3 one;
		static const HDFLOAT3 forward;
		static const HDFLOAT3 up;
		static const HDFLOAT3 right;
	};

	struct HDFLOAT4
	{
		union 
		{
			struct
			{
				float x;
				float y;
				float z;
				float w;
			};

			struct
			{
				float r;
				float g;
				float b;
				float a;
			};

		};
		
		// �⺻ ������
		HDFLOAT4(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f);

		// ���� ������
		HDFLOAT4(const HDFLOAT4& val);

		// �̵� ������
		HDFLOAT4(HDFLOAT4&& val) noexcept = default;

		// ������ �����ε�
		HDFLOAT4 operator-() const; // ���� ��ȯ
		HDFLOAT4 operator=(const HDFLOAT4& other); // ���� ������
		HDFLOAT4 operator+(const HDFLOAT4& other) const; // ���ϱ�
		HDFLOAT4 operator*(const HDFLOAT4& other) const; // ���ϱ�
		HDFLOAT4& operator+=(const HDFLOAT4& other); // ���ؼ� ����
		HDFLOAT4 operator-(const HDFLOAT4& other) const; // ����
		HDFLOAT4& operator-=(const HDFLOAT4& other); // ���� ����

		// ���Ϳ� ��Į�� ����
		HDFLOAT4 operator* (float scalar); // ���Ϳ� ��Į�� ��
		HDFLOAT4& operator*=(float scalar); // ���Ϳ� ��Į�� �� ����
		HDFLOAT4 operator/ (float scalar); // ���Ϳ� ��Į�� ������
		HDFLOAT4& operator/=(float scalar); // ���Ϳ� ��Į�� ������ ����

		// �񱳿�����
		bool operator==(const HDFLOAT4& other);
		bool operator!=(const HDFLOAT4& other);
		bool operator<(const HDFLOAT4& other);
		bool operator<=(const HDFLOAT4& other);
		bool operator>(const HDFLOAT4& other);
		bool operator>=(const HDFLOAT4& other);

		// ���� ����
		static HDFLOAT4 Add(const HDFLOAT4& left, const HDFLOAT4& right);
		static HDFLOAT4 Substract(const HDFLOAT4& left, const HDFLOAT4& right);
		static HDFLOAT4 Multiply(const HDFLOAT4& val, float scalar);
		static HDFLOAT4 Divide(const HDFLOAT4& val, float scalar);
		static HDFLOAT4 Normalize(const HDFLOAT4& val); // ����ȭ
	};

	struct HDQuaternion
	{
		float w;
		float x;
		float y;
		float z;

		// �⺻ ������ (������ ��Ұ� 1��)
		HDQuaternion();

		// �⺻ ������
		HDQuaternion(float w, float x, float y, float z);

		// ��Ʈ������ ���ʹϾ����� ��ȯ�ϴ� ������
		HDQuaternion(const HDFLOAT3X3& matrix);
		
		
		// ���� ������
		HDQuaternion(const HDQuaternion& val);

		// �̵� ������
		HDQuaternion(HDQuaternion&& val) noexcept = default;

		// ������ �����ε�
		HDQuaternion operator=(const HDQuaternion& other); // ���� ������
		HDQuaternion operator*(const HDQuaternion& other); // ���ϱ�

		HDFLOAT3 operator*(const HDFLOAT3& other) const;

		// �� ������
		bool operator==(const HDQuaternion& other);
		bool operator!=(const HDQuaternion& other);


		static const HDQuaternion Identity;

		// ���� ����
		HDQuaternion conjugate() const;
		HDFLOAT3 Right() const;
		HDFLOAT3 Up() const;
		HDFLOAT3 Forward() const;
		static HDQuaternion Normalize(const HDQuaternion& other);
		static HDQuaternion FromLocalAxes(HDFLOAT3 right, HDFLOAT3 up, HDFLOAT3 forward);
	};

	struct HDFLOAT3X3
	{
		union 
		{
			struct
			{
				float _11, _12, _13;
				float _21, _22, _23;
				float _31, _32, _33;
			};

			float element[3][3];
		};

		static const HDFLOAT3X3 Identity;

		// �⺻ ������
		HDFLOAT3X3();
		HDFLOAT3X3(	float val11, float val12, float val13,
					float val21, float val22, float val23,
					float val31, float val32, float val33 );

		// ���� ������
		HDFLOAT3X3(const HDFLOAT3X3& val);

		// ���� ������ (���� ����)
		HDFLOAT3X3& operator=(const HDFLOAT3X3& val);

		// ������ �����ε�
		HDFLOAT3X3 operator*(const HDFLOAT3X3& val);
		HDFLOAT3X3& operator*=(const HDFLOAT3X3& val);

		// ���� ����
		HDFLOAT3X3 Multiply(const HDFLOAT3X3& left, const HDFLOAT3X3& right);
		HDFLOAT3X3 Inverse() const;

	};

	struct HDFLOAT4X4
	{
		union 
		{
			struct 
			{
				float _11, _12, _13, _14;
				float _21, _22, _23, _24;
				float _31, _32, _33, _34;
				float _41, _42, _43, _44;
			};

			float element[4][4];
		};

		static const HDFLOAT4X4 Identity;

		// �⺻ ������
		HDFLOAT4X4();
		HDFLOAT4X4(float val11, float val12, float val13, float val14,
			float val21, float val22, float val23, float val24,
			float val31, float val32, float val33, float val34,
			float val41, float val42, float val43, float val44);

		// ���� ������
		HDFLOAT4X4(const HDFLOAT3X3& val);


		// ���� ������
		HDFLOAT4X4& operator=(const HDFLOAT4X4& val);

		// ������ �����ε� (�����, ��İ�)
		HDFLOAT4X4 operator*(const HDFLOAT4X4& val);
		HDFLOAT4X4& operator*=(const HDFLOAT4X4& val);
		

		// ���� ����
		HDFLOAT4X4 Multiply		(const HDFLOAT4X4& left, const HDFLOAT4X4& right);
		HDFLOAT4X4 Inverse		() const;
		HDFLOAT3X3 ToMatrix3x3	() const;
	};

	// ��Ÿ ���� ����
	HDFLOAT4		HDFloat4MultiplyMatrix		(const HDFLOAT4& left, const HDFLOAT4X4& right);
	HDFLOAT4		HDFloat3MultiplyMatrix		(const HDFLOAT3& left, const HDFLOAT4X4& right);
	HDFLOAT4		QuaternionToFloat4			(const HDQuaternion& val);
	HDQuaternion	Float4ToQuaternion			(const HDFLOAT4& val);
	HDFLOAT4X4		GetTransformMatrix			(const HDFLOAT3& position, const HDQuaternion& rotation, const HDFLOAT3& scale);
	HDFLOAT3		GetLocalPositionFromLocalTM	(const HDFLOAT4X4& localTM);
	HDQuaternion	GetLocalRotationFromLocalTM	(const HDFLOAT4X4& localTM);
	HDFLOAT3		GetLocalScaleFromLocalTM	(const HDFLOAT4X4& localTM);
	HDQuaternion	HDRotateQuaternion			(const HDQuaternion& quaternion, const HDFLOAT3& axis, float radian);
	HDQuaternion	HDQuaternionMultiply		(const HDQuaternion& lhs, const HDQuaternion& rhs);
};


