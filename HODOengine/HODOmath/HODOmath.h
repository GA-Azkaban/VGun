#pragma once
#include <functional>

/// <summary>
/// 23.10.31 오수안
/// 공용 수학 라이브러리
/// RocketMath와 Overload의 OvMath 참고
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

		// 기본 생성자
		HDFLOAT2(float x = 0.0f, float y = 0.0f);

		// 복사 생성자
		HDFLOAT2(const HDFLOAT2& val);

		// 이동 생성자
		HDFLOAT2(HDFLOAT2&& val) noexcept = default;

		// 연산자 오버로딩
		HDFLOAT2 operator-() const; // 음수 변환
		HDFLOAT2 operator=(const HDFLOAT2& other); // 대입 연산자
		HDFLOAT2 operator+(const HDFLOAT2& other) const; // 더하기
		HDFLOAT2& operator+=(const HDFLOAT2& other); // 더해서 대입
		HDFLOAT2 operator-(const HDFLOAT2& other) const; // 빼기
		HDFLOAT2& operator-=(const HDFLOAT2& other); // 빼서 대입

		// 벡터와 스칼라 연산
		HDFLOAT2 operator* (float scalar); // 벡터와 스칼라 곱
		HDFLOAT2& operator*=(float scalar); // 벡터와 스칼라 곱 대입
		HDFLOAT2 operator/ (float scalar); // 벡터와 스칼라 나눗셈
		HDFLOAT2& operator/=(float scalar); // 벡터와 스칼라 나누고 대입

		// 비교연산자
		bool operator==(const HDFLOAT2& other);
		bool operator!=(const HDFLOAT2& other);

		// 수학 연산
		static HDFLOAT2 Add(const HDFLOAT2& left, const HDFLOAT2& right);
		static HDFLOAT2 Substract(const HDFLOAT2& left, const HDFLOAT2& right);
		static HDFLOAT2 Multiply(const HDFLOAT2& val, float scalar);
		static HDFLOAT2 Divide(const HDFLOAT2& val, float scalar);
		static float Dot(const HDFLOAT2& left, const HDFLOAT2& right); // 내적
		static HDFLOAT2 Normalize(const HDFLOAT2& val); // 정규화

	};

	struct HDFLOAT3
	{
		float x;
		float y;
		float z;

		// 기본 생성자
		HDFLOAT3(float x = 0.0f, float y = 0.0f, float z = 0.0f);

		// 복사 생성자
		HDFLOAT3(const HDFLOAT3& val);

		// 이동 생성자
		HDFLOAT3(HDFLOAT3&& val) noexcept = default;

		// 연산자 오버로딩
		HDFLOAT3 operator-() const; // 음수 변환
		HDFLOAT3 operator=(const HDFLOAT3& other); // 대입 연산자
		HDFLOAT3 operator+(const HDFLOAT3& other) const; // 더하기
		HDFLOAT3 operator*(const HDFLOAT3& other) const; // 곱하기
		HDFLOAT3& operator+=(const HDFLOAT3& other); // 더해서 대입
		HDFLOAT3 operator-(const HDFLOAT3& other) const; // 빼기
		HDFLOAT3& operator-=(const HDFLOAT3& other); // 빼서 대입

		// 벡터와 스칼라 연산
		HDFLOAT3 operator* (float scalar); // 벡터와 스칼라 곱
		HDFLOAT3& operator*=(float scalar); // 벡터와 스칼라 곱 대입
		HDFLOAT3 operator/ (float scalar); // 벡터와 스칼라 나눗셈
		HDFLOAT3& operator/=(float scalar); // 벡터와 스칼라 나누고 대입

		// 비교연산자
		bool operator==(const HDFLOAT3& other);
		bool operator!=(const HDFLOAT3& other);
		bool operator<(const HDFLOAT3& other);
		bool operator<=(const HDFLOAT3& other);
		bool operator>(const HDFLOAT3& other);
		bool operator>=(const HDFLOAT3& other);

		// 수학 연산
		static HDFLOAT3 Add(const HDFLOAT3& left, const HDFLOAT3& right);
		static HDFLOAT3 Substract(const HDFLOAT3& left, const HDFLOAT3& right);
		static HDFLOAT3 Multiply(const HDFLOAT3& val, float scalar);
		static HDFLOAT3 Divide(const HDFLOAT3& val, float scalar);
		static float Dot(const HDFLOAT3& left, const HDFLOAT3& right); // 내적
		static HDFLOAT3 Cross(const HDFLOAT3& a, const HDFLOAT3& b); // 외적
		static HDFLOAT3 Normalize(const HDFLOAT3& val); // 정규화

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
		
		// 기본 생성자
		HDFLOAT4(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f);

		// 복사 생성자
		HDFLOAT4(const HDFLOAT4& val);

		// 이동 생성자
		HDFLOAT4(HDFLOAT4&& val) noexcept = default;

		// 연산자 오버로딩
		HDFLOAT4 operator-() const; // 음수 변환
		HDFLOAT4 operator=(const HDFLOAT4& other); // 대입 연산자
		HDFLOAT4 operator+(const HDFLOAT4& other) const; // 더하기
		HDFLOAT4 operator*(const HDFLOAT4& other) const; // 곱하기
		HDFLOAT4& operator+=(const HDFLOAT4& other); // 더해서 대입
		HDFLOAT4 operator-(const HDFLOAT4& other) const; // 빼기
		HDFLOAT4& operator-=(const HDFLOAT4& other); // 빼서 대입

		// 벡터와 스칼라 연산
		HDFLOAT4 operator* (float scalar); // 벡터와 스칼라 곱
		HDFLOAT4& operator*=(float scalar); // 벡터와 스칼라 곱 대입
		HDFLOAT4 operator/ (float scalar); // 벡터와 스칼라 나눗셈
		HDFLOAT4& operator/=(float scalar); // 벡터와 스칼라 나누고 대입

		// 비교연산자
		bool operator==(const HDFLOAT4& other);
		bool operator!=(const HDFLOAT4& other);
		bool operator<(const HDFLOAT4& other);
		bool operator<=(const HDFLOAT4& other);
		bool operator>(const HDFLOAT4& other);
		bool operator>=(const HDFLOAT4& other);

		// 수학 연산
		static HDFLOAT4 Add(const HDFLOAT4& left, const HDFLOAT4& right);
		static HDFLOAT4 Substract(const HDFLOAT4& left, const HDFLOAT4& right);
		static HDFLOAT4 Multiply(const HDFLOAT4& val, float scalar);
		static HDFLOAT4 Divide(const HDFLOAT4& val, float scalar);
		static HDFLOAT4 Normalize(const HDFLOAT4& val); // 정규화
	};

	struct HDQuaternion
	{
		float w;
		float x;
		float y;
		float z;

		// 기본 생성자 (마지막 요소가 1인)
		HDQuaternion();

		// 기본 생성자
		HDQuaternion(float w, float x, float y, float z);

		// 매트릭스를 쿼터니언으로 변환하는 생성자
		HDQuaternion(const HDFLOAT3X3& matrix);
		
		
		// 복사 생성자
		HDQuaternion(const HDQuaternion& val);

		// 이동 생성자
		HDQuaternion(HDQuaternion&& val) noexcept = default;

		// 연산자 오버로딩
		HDQuaternion operator=(const HDQuaternion& other); // 대입 연산자
		HDQuaternion operator*(const HDQuaternion& other); // 곱하기

		HDFLOAT3 operator*(const HDFLOAT3& other) const;

		// 비교 연산자
		bool operator==(const HDQuaternion& other);
		bool operator!=(const HDQuaternion& other);


		static const HDQuaternion Identity;

		// 수학 연산
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

		// 기본 생성자
		HDFLOAT3X3();
		HDFLOAT3X3(	float val11, float val12, float val13,
					float val21, float val22, float val23,
					float val31, float val32, float val33 );

		// 복사 생성자
		HDFLOAT3X3(const HDFLOAT3X3& val);

		// 대입 연산자 (깊은 복사)
		HDFLOAT3X3& operator=(const HDFLOAT3X3& val);

		// 연산자 오버로딩
		HDFLOAT3X3 operator*(const HDFLOAT3X3& val);
		HDFLOAT3X3& operator*=(const HDFLOAT3X3& val);

		// 수학 연산
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

		// 기본 생성자
		HDFLOAT4X4();
		HDFLOAT4X4(float val11, float val12, float val13, float val14,
			float val21, float val22, float val23, float val24,
			float val31, float val32, float val33, float val34,
			float val41, float val42, float val43, float val44);

		// 복사 생성자
		HDFLOAT4X4(const HDFLOAT3X3& val);


		// 대입 연산자
		HDFLOAT4X4& operator=(const HDFLOAT4X4& val);

		// 연산자 오버로딩 (역행렬, 행렬곱)
		HDFLOAT4X4 operator*(const HDFLOAT4X4& val);
		HDFLOAT4X4& operator*=(const HDFLOAT4X4& val);
		

		// 수학 연산
		HDFLOAT4X4 Multiply		(const HDFLOAT4X4& left, const HDFLOAT4X4& right);
		HDFLOAT4X4 Inverse		() const;
		HDFLOAT3X3 ToMatrix3x3	() const;
	};

	// 기타 수학 연산
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


