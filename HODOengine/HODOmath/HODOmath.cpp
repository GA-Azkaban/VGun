#include "HODOmath.h"

#include <cmath>

namespace HDMaths
{
	/// Vector2

	HDFLOAT2::HDFLOAT2(float x, float y)
		: x(x), y(y)
	{

	}

	HDFLOAT2::HDFLOAT2(const HDFLOAT2& val)
		: x(val.x), y(val.y)
	{

	}

	HDFLOAT2 HDFLOAT2::operator-() const
	{
		return Multiply(*this, -1);
	}

	HDFLOAT2 HDFLOAT2::operator=(const HDFLOAT2& other)
	{
		this->x = other.x;
		this->y = other.y;

		return *this;
	}

	HDFLOAT2 HDFLOAT2::operator+(const HDFLOAT2& other) const
	{
		return Add(*this, other);
	}

	HDMaths::HDFLOAT2& HDFLOAT2::operator+=(const HDFLOAT2& other)
	{
		*this = Add(*this, other);
		return *this;
	}

	HDFLOAT2 HDFLOAT2::operator-(const HDFLOAT2& other) const
	{
		return Substract(*this, other);
	}

	HDFLOAT2& HDFLOAT2::operator-=(const HDFLOAT2& other)
	{
		*this = Substract(*this, other);
		return *this;
	}

	HDFLOAT2 HDFLOAT2::operator*(float scalar)
	{
		return Multiply(*this, scalar);
	}

	HDFLOAT2& HDFLOAT2::operator*=(float scalar)
	{
		*this = Multiply(*this, scalar);
		return *this;
	}

	HDFLOAT2 HDFLOAT2::operator/(float scalar)
	{
		return Divide(*this, scalar);
	}

	HDMaths::HDFLOAT2& HDFLOAT2::operator/=(float scalar)
	{
		*this = Divide(*this, scalar);
		return *this;
	}

	bool HDFLOAT2::operator==(const HDFLOAT2& other)
	{
		return (x == other.x && y == other.y);
	}

	bool HDFLOAT2::operator!=(const HDFLOAT2& other)
	{
		return (x != other.x || y != other.y);
	}

	HDFLOAT2 HDFLOAT2::Add(const HDFLOAT2& left, const HDFLOAT2& right)
	{
		return HDFLOAT2
		{
			left.x + right.x,
			left.y + right.y
		};
	}

	HDFLOAT2 HDFLOAT2::Substract(const HDFLOAT2& left, const HDFLOAT2& right)
	{
		return HDFLOAT2
		{
			left.x - right.x,
			left.y - right.y
		};
	}

	HDFLOAT2 HDFLOAT2::Multiply(const HDFLOAT2& val, float scalar)
	{
		return HDFLOAT2
		{
			val.x * scalar,
			val.y * scalar
		};
	}

	HDFLOAT2 HDFLOAT2::Divide(const HDFLOAT2& val, float scalar)
	{
		return HDFLOAT2
		{
			val.x / scalar,
			val.y / scalar
		};
	}

	float HDFLOAT2::Dot(const HDFLOAT2& left, const HDFLOAT2& right)
	{
		
		return (left.x * right.x + left.y * right.y);
		
	}

	HDFLOAT2 HDFLOAT2::Normalize(const HDFLOAT2& val)
	{
		float length = std::sqrt(val.x * val.x + val.y * val.y);

		if (length != 0.0f)
		{
			// 벡터 크기의 역수를 구하고
			float invLength = 1.0f / length;
			// 각 성분에 곱하여 크기를 모두 1로 만든다
			return HDFLOAT2(val.x * invLength, val.y * invLength);
		}
		else
		{
			// 벡터가 원점에 있는 경우
			return HDFLOAT2(0.f, 0.f);
		}
	}

	/// Vector3

	HDFLOAT3::HDFLOAT3(float x, float y, float z)
		: x(x), y(y), z(z)
	{

	}

	HDFLOAT3::HDFLOAT3(const HDFLOAT3& val)
		: x(val.x), y(val.y), z(val.z)
	{

	}

	HDFLOAT3 HDFLOAT3::operator-() const
	{
		return Multiply(*this, -1);
	}

	HDFLOAT3 HDFLOAT3::operator=(const HDFLOAT3& other)
	{
		this->x = other.x;
		this->y = other.y;
		this->z = other.z;

		return *this;
	}

	HDFLOAT3 HDFLOAT3::operator+(const HDFLOAT3& other) const
	{
		return Add(*this, other);
	}

	HDFLOAT3& HDFLOAT3::operator+=(const HDFLOAT3& other)
	{
		*this = Add(*this, other);
		return *this;
	}

	HDFLOAT3 HDFLOAT3::operator-(const HDFLOAT3& other) const
	{
		return Substract(*this, other);
	}

	HDFLOAT3& HDFLOAT3::operator-=(const HDFLOAT3& other)
	{
		*this = Substract(*this, other);
		return *this;
	}

	HDFLOAT3 HDFLOAT3::operator*(float scalar)
	{
		return Multiply(*this, scalar);
	}

	HDFLOAT3& HDFLOAT3::operator*=(float scalar)
	{
		*this = Multiply(*this, scalar);
		return *this;
	}

	HDFLOAT3 HDFLOAT3::operator/(float scalar)
	{
		return Divide(*this, scalar);
	}

	HDMaths::HDFLOAT3& HDFLOAT3::operator/=(float scalar)
	{
		*this = Divide(*this, scalar);
		return *this;
	}

	bool HDFLOAT3::operator==(const HDFLOAT3& other)
	{
		return (x == other.x && y == other.y && z == other.z);
	}

	bool HDFLOAT3::operator!=(const HDFLOAT3& other)
	{
		return !(*this == other);
	}

	bool HDFLOAT3::operator<(const HDFLOAT3& other)
	{
		return
		{
			x < other.x
			&& y < other.y
			&& z < other.z
		};
	}

	bool HDFLOAT3::operator<=(const HDFLOAT3& other)
	{
		return
		{
			x <= other.x
			&& y <= other.y
			&& z <= other.z
		};
	}

	bool HDFLOAT3::operator>(const HDFLOAT3& other)
	{
		return
		{
			x > other.x
			&& y > other.y
			&& z > other.z
		};
	}

	bool HDFLOAT3::operator>=(const HDFLOAT3& other)
	{
		return
		{
			x >= other.x
			&& y >= other.y
			&& z >= other.z
		};
	}

	HDFLOAT3 HDFLOAT3::Add(const HDFLOAT3& left, const HDFLOAT3& right)
	{
		return HDFLOAT3
		{
			left.x + right.x,
			left.y + right.y,
			left.z + right.z
		};
	}

	HDFLOAT3 HDFLOAT3::Substract(const HDFLOAT3& left, const HDFLOAT3& right)
	{
		return HDFLOAT3
		{
			left.x - right.x,
			left.y - right.y,
			left.z - right.z
		};
	}

	HDFLOAT3 HDFLOAT3::Multiply(const HDFLOAT3& val, float scalar)
	{
		return HDFLOAT3
		{
			val.x * scalar,
			val.y * scalar,
			val.z * scalar
		};
	}

	HDFLOAT3 HDFLOAT3::Divide(const HDFLOAT3& val, float scalar)
	{
		return HDFLOAT3
		{
			val.x / scalar,
			val.y / scalar,
			val.z / scalar
		};
	}

	float HDFLOAT3::Dot(const HDFLOAT3& left, const HDFLOAT3& right)
	{
		return (left.x * right.x + left.y * right.y + left.z * right.z);
	}

	HDFLOAT3 HDFLOAT3::Cross(const HDFLOAT3& a, const HDFLOAT3& b)
	{
		float resultX = a.y * b.z - a.z * b.y;
		float resultY = a.z * b.x - a.x * b.z;
		float resultZ = a.x * b.y - a.y * b.x;
		
		return HDFLOAT3
		{ 
			resultX, resultY, resultZ
		};
	}

	HDFLOAT3 HDFLOAT3::Normalize(const HDFLOAT3& val)
	{
		float length = std::sqrt(val.x * val.x + val.y * val.y + val.z * val.z);
		if (length != 0.0f) 
		{
			float invLength = 1.0f / length;
			return HDFLOAT3(val.x * invLength, val.y * invLength, val.z * invLength);
		}
		else 
		{
			return HDFLOAT3(0.0f, 0.0f, 0.0f);
		}
	}

	HDFLOAT4::HDFLOAT4(float x, float y, float z, float w)
		: x(x), y(y), z(z), w(w)
	{

	}

	HDFLOAT4::HDFLOAT4(const HDFLOAT4& val)
		: x(val.x), y(val.y), z(val.z), w(val.w)
	{

	}

	HDFLOAT4 HDFLOAT4::operator-() const
	{
		return Multiply(*this, -1);
	}

	HDFLOAT4 HDFLOAT4::operator=(const HDFLOAT4& other)
	{
		this->x = other.x;
		this->y = other.y;
		this->z = other.z;

		return *this;
	}

	HDFLOAT4 HDFLOAT4::operator+(const HDFLOAT4& other) const
	{
		return Add(*this, other);
	}

	HDMaths::HDFLOAT4& HDFLOAT4::operator+=(const HDFLOAT4& other)
	{
		*this = Add(*this, other);
		return *this;
	}

	HDFLOAT4 HDFLOAT4::operator-(const HDFLOAT4& other) const
	{
		return Substract(*this, other);
	}

	HDFLOAT4& HDFLOAT4::operator-=(const HDFLOAT4& other)
	{
		*this = Substract(*this, other);
		return *this;
	}

	HDFLOAT4 HDFLOAT4::operator*(float scalar)
	{
		return Multiply(*this, scalar);
	}

	HDFLOAT4& HDFLOAT4::operator*=(float scalar)
	{
		*this = Multiply(*this, scalar);
		return *this;
	}

	HDFLOAT4 HDFLOAT4::operator/(float scalar)
	{
		return Divide(*this, scalar);
	}

	HDFLOAT4& HDFLOAT4::operator/=(float scalar)
	{
		*this = Divide(*this, scalar);
		return *this;
	}

	bool HDFLOAT4::operator==(const HDFLOAT4& other)
	{
		return (x == other.x && y == other.y && z == other.z && w == other.w);
	}

	bool HDFLOAT4::operator!=(const HDFLOAT4& other)
	{
		return !(*this == other);
	}

	bool HDFLOAT4::operator<(const HDFLOAT4& other)
	{
		return
		{
			x < other.x
			&& y < other.y
			&& z < other.z
		};
	}

	bool HDFLOAT4::operator<=(const HDFLOAT4& other)
	{
		return
		{
			x <= other.x
			&& y <= other.y
			&& z <= other.z
		};
	}

	bool HDFLOAT4::operator>(const HDFLOAT4& other)
	{
		return
		{
			x > other.x
			&& y > other.y
			&& z > other.z
		};
	}

	bool HDFLOAT4::operator>=(const HDFLOAT4& other)
	{
		return
		{
			x >= other.x
			&& y >= other.y
			&& z >= other.z
		};
	}

	HDFLOAT4 HDFLOAT4::Add(const HDFLOAT4& left, const HDFLOAT4& right)
	{
		return HDFLOAT4
		{
			left.x + right.x,
			left.y + right.y,
			left.z + right.z,
			left.w + right.w
		};
	}

	HDFLOAT4 HDFLOAT4::Substract(const HDFLOAT4& left, const HDFLOAT4& right)
	{
		return HDFLOAT4
		{
			left.x - right.x,
			left.y - right.y,
			left.z - right.z,
			left.w - right.w
		};
	}

	HDFLOAT4 HDFLOAT4::Multiply(const HDFLOAT4& val, float scalar)
	{
		return HDFLOAT4
		{
			val.x * scalar,
			val.y * scalar,
			val.z * scalar,
			val.w * scalar
		};
	}

	HDMaths::HDFLOAT4 HDFLOAT4::Divide(const HDFLOAT4& val, float scalar)
	{
		return HDFLOAT4
		{
			val.x / scalar,
			val.y / scalar,
			val.z / scalar,
			val.w / scalar
		};
	}

	HDFLOAT4 HDFLOAT4::Normalize(const HDFLOAT4& val)
	{
		float length = 
			std::sqrt(val.x * val.x + val.y * val.y + val.z * val.z + val.w * val.w);
		if (length != 0.0f) 
		{
			float invLength = 1.0f / length;
			return HDFLOAT4(val.x * invLength, 
							val.y * invLength, 
							val.z * invLength, 
							val.w * invLength);
		}
		else 
		{
			return HDFLOAT4();
		}
	}

	HDQuternion::HDQuternion()
		: w(0.f), x(0.f), y(0.f), z(1.0)
	{

	}

	HDQuternion::HDQuternion(float w, float x, float y, float z)
		: w(w), x(x), y(y), z(z)
	{

	}

	HDQuternion::HDQuternion(const HDQuternion& val)
		: w(val.w), x(val.x), y(val.y), z(val.z)
	{

	}

	HDQuternion HDQuternion::conjugate() const
	{
		return HDQuternion(w, -x, -y, -z);
	}

	HDQuternion HDQuternion::Normalize(const HDQuternion& other)
	{
		float length = std::sqrt(other.w * other.w + other.x * other.x + other.y * other.y + other.z * other.z);
		return { other.w / length, other.x / length, other.y / length, other.z / length };
	}

	HDMaths::HDQuternion HDQuternion::operator=(const HDQuternion& other)
	{
		this->w = other.w;
		this->x = other.x;
		this->y = other.y;
		this->z = other.z;

		return *this;
	}

	bool HDQuternion::operator==(const HDQuternion& other)
	{
		return (w == other.w && x == other.x && y == other.y && z == other.z);
	}

	bool HDQuternion::operator!=(const HDQuternion& other)
	{
		return !(*this == other);
	}

	const HDFLOAT3X3 HDFLOAT3X3::Identitiy = 
		HDFLOAT3X3(	1.f, 0.f, 0.f,
					0.f, 1.f, 0.f,
					0.f, 0.f, 1.f);

	HDFLOAT3X3::HDFLOAT3X3()
		: _11(0.f), _12(0.f), _13(0.f), 
		_21(0.f), _22(0.f), _23(0.f),
		_31(0.f), _32(0.f), _33(0.f)
	{

	}


	HDFLOAT3X3::HDFLOAT3X3(float val11, float val12, float val13, float val21, float val22, float val23, float val31, float val32, float val33)
		: _11(val11), _12(val12), _13(val13),
		_21(val21), _22(val22), _23(val23),
		_31(val31), _32(val32), _33(val33)
	{

	}


	HDFLOAT3X3::HDFLOAT3X3(const HDFLOAT3X3& val)
	{
		*this = val;
	}


	HDFLOAT3X3& HDFLOAT3X3::operator=(const HDFLOAT3X3& val)
	{
		memcpy(this->element, val.element, 9 * sizeof(float));
		return *this;
	}

	HDFLOAT3X3 HDFLOAT3X3::operator*(const HDFLOAT3X3& val)
	{
		return Multiply(*this, val);
	}

	HDFLOAT3X3& HDFLOAT3X3::operator*=(const HDFLOAT3X3& val)
	{
		*this = Multiply(*this, val);
		return *this;
	}

	HDFLOAT3X3 HDFLOAT3X3::Multiply(const HDFLOAT3X3& left, const HDFLOAT3X3& right)
	{
		HDFLOAT3X3 result;

		for (int row = 0; row < 3; row++)
		{
			for (int col = 0; col < 3; col++)
			{
				result.element[row][col] =
					left.element[row][0] * right.element[0][col] +
					left.element[row][1] * right.element[1][col] +
					left.element[row][2] * right.element[2][col];
			}
		}
		
		return result;
	}

	HDFLOAT3X3 HDFLOAT3X3::Inverse() const
	{
		// 3x3 행렬의 행렬식 계산
		float det = element[0][0] * (element[1][1] * element[2][2] - element[1][2] * element[2][1]) -
			element[0][1] * (element[1][0] * element[2][2] - element[1][2] * element[2][0]) +
			element[0][2] * (element[1][0] * element[2][1] - element[1][1] * element[2][0]);

		// 행렬식이 0인 경우 역행렬이 존재하지 않음
		if (det == 0)
		{
			return this->Identitiy;
		}

		HDFLOAT3X3 inv;

		// 역행렬 계산
		float invDet = 1.0f / det;
		inv.element[0][0] = (element[1][1] * element[2][2] - element[1][2] * element[2][1]) * invDet;
		inv.element[0][1] = (element[0][2] * element[2][1] - element[0][1] * element[2][2]) * invDet;
		inv.element[0][2] = (element[0][1] * element[1][2] - element[0][2] * element[1][1]) * invDet;
		inv.element[1][0] = (element[1][2] * element[2][0] - element[1][0] * element[2][2]) * invDet;
		inv.element[1][1] = (element[0][0] * element[2][2] - element[0][2] * element[2][0]) * invDet;
		inv.element[1][2] = (element[0][2] * element[1][0] - element[0][0] * element[1][2]) * invDet;
		inv.element[2][0] = (element[1][0] * element[2][1] - element[1][1] * element[2][0]) * invDet;
		inv.element[2][1] = (element[0][1] * element[2][0] - element[0][0] * element[2][1]) * invDet;
		inv.element[2][2] = (element[0][0] * element[1][1] - element[0][1] * element[1][0]) * invDet;

		return inv;
	}

	const HDFLOAT4X4 HDFLOAT4X4::Identitiy =
		HDFLOAT4X4(1.f, 0.f, 0.f, 0.f,
			0.f, 1.f, 0.f, 0.f,
			0.f, 0.f, 1.f, 0.f,
			0.f, 0.f, 0.f, 1.f);

	HDFLOAT4X4::HDFLOAT4X4()
		: _11(0.f), _12(0.f), _13(0.f), _14(0.f),
		_21(0.f), _22(0.f), _23(0.f), _24(0.f),
		_31(0.f), _32(0.f), _33(0.f), _34(0.f),
		_41(0.f), _42(0.f), _43(0.f), _44(0.f)
	{

	}

	HDFLOAT4X4::HDFLOAT4X4(const HDFLOAT3X3& val)
	{
		*this = val;
	}

	HDFLOAT4X4::HDFLOAT4X4(float val11, float val12, float val13, float val14,
		float val21, float val22, float val23, float val24,
		float val31, float val32, float val33, float val34,
		float val41, float val42, float val43, float val44)
		: _11(val11), _12(val12), _13(val13), _14(val14),
		_21(val21), _22(val22), _23(val23), _24(val24),
		_31(val31), _32(val32), _33(val33), _34(val34),
		_41(val41), _42(val32), _43(val33), _44(val34)
	{

	}

	HDFLOAT4X4& HDFLOAT4X4::operator=(const HDFLOAT4X4& val)
	{
		return *this;
	}

	HDFLOAT4X4 HDFLOAT4X4::operator*(const HDFLOAT4X4& val)
	{
		return Multiply(*this, val);
	}

	HDFLOAT4X4& HDFLOAT4X4::operator*=(const HDFLOAT4X4& val)
	{
		*this = Multiply(*this, val);
		return *this;
	}

	HDFLOAT4X4 HDFLOAT4X4::Multiply(const HDFLOAT4X4& left, const HDFLOAT4X4& right)
	{
		HDFLOAT4X4 result;

		for (int row = 0; row < 4; row++)
		{
			for (int col = 0; col < 4; col++)
			{
				result.element[row][col] =
					left.element[row][0] * right.element[0][col] +
					left.element[row][1] * right.element[1][col] +
					left.element[row][2] * right.element[2][col] +
					left.element[row][3] * right.element[3][col];
			}
		}
		return result;
	}

	HDFLOAT4X4 HDFLOAT4X4::Inverse() const
	{
		HDFLOAT4X4 inv;

		float det =
			_11 * (_22 * (_33 * _44 - _34 * _43) - _23 * (_32 * _44 - _34 * _42) + _24 * (_32 * _43 - _33 * _42)) -
			_12 * (_21 * (_33 * _44 - _34 * _43) - _23 * (_31 * _44 - _34 * _41) + _24 * (_31 * _43 - _33 * _41)) +
			_13 * (_21 * (_32 * _44 - _34 * _42) - _22 * (_31 * _44 - _34 * _41) + _24 * (_31 * _42 - _32 * _41)) -
			_14 * (_21 * (_32 * _43 - _33 * _42) - _22 * (_31 * _43 - _33 * _41) + _23 * (_31 * _42 - _32 * _41));

		if (std::abs(det) < 1e-8)
		{
			// 행렬식이 0에 가까우면 역행렬이 존재하지 않습니다.
			return Identitiy;
		}

		float invDet = 1.0f / det;

		inv._11 = (_22 * (_33 * _44 - _34 * _43) - _23 * (_32 * _44 - _34 * _42) + _24 * (_32 * _43 - _33 * _42)) * invDet;
		inv._12 = -(_12 * (_33 * _44 - _34 * _43) - _13 * (_32 * _44 - _34 * _42) + _14 * (_32 * _43 - _33 * _42)) * invDet;
		inv._13 = (_12 * (_23 * _44 - _24 * _43) - _13 * (_22 * _44 - _24 * _42) + _14 * (_22 * _43 - _23 * _42)) * invDet;
		inv._14 = -(_12 * (_23 * _34 - _24 * _33) - _13 * (_22 * _34 - _24 * _32) + _14 * (_22 * _33 - _23 * _32)) * invDet;
		inv._21 = -(_21 * (_33 * _44 - _34 * _43) - _23 * (_31 * _44 - _34 * _41) + _24 * (_31 * _43 - _33 * _41)) * invDet;
		inv._22 = (_11 * (_33 * _44 - _34 * _43) - _13 * (_31 * _44 - _34 * _41) + _14 * (_31 * _43 - _33 * _41)) * invDet;
		inv._23 = -(_11 * (_23 * _44 - _24 * _43) - _13 * (_21 * _44 - _24 * _41) + _14 * (_21 * _43 - _23 * _41)) * invDet;
		inv._24 = (_11 * (_23 * _34 - _24 * _33) - _13 * (_21 * _34 - _24 * _31) + _14 * (_21 * _33 - _23 * _31)) * invDet;
		inv._31 = (_21 * (_32 * _44 - _34 * _42) - _22 * (_31 * _44 - _34 * _41) + _24 * (_31 * _42 - _32 * _41)) * invDet;
		inv._32 = -(_11 * (_32 * _44 - _34 * _42) - _12 * (_31 * _44 - _34 * _41) + _14 * (_31 * _42 - _32 * _41)) * invDet;
		inv._33 = (_11 * (_22 * _44 - _24 * _42) - _12 * (_21 * _44 - _24 * _41) + _14 * (_21 * _42 - _22 * _41)) * invDet;
		inv._34 = -(_11 * (_22 * _34 - _24 * _32) - _12 * (_21 * _34 - _24 * _31) + _14 * (_21 * _32 - _22 * _31)) * invDet;
		inv._41 = -(_21 * (_32 * _43 - _33 * _42) - _22 * (_31 * _43 - _33 * _41) + _23 * (_31 * _42 - _32 * _41)) * invDet;
		inv._42 = (_11 * (_32 * _43 - _33 * _42) - _12 * (_31 * _43 - _33 * _41) + _13 * (_31 * _42 - _32 * _41)) * invDet;
		inv._43 = -(_11 * (_22 * _43 - _23 * _42) - _12 * (_21 * _43 - _23 * _41) + _13 * (_21 * _42 - _22 * _41)) * invDet;
		inv._44 = (_11 * (_22 * _33 - _23 * _32) - _12 * (_21 * _33 - _23 * _31) + _13 * (_21 * _32 - _22 * _31)) * invDet;

		return inv;
	}

	HDFLOAT4 HDFloat4MultiplyMatrix(const HDFLOAT4& left, const HDFLOAT4X4& right)
	{
		HDFLOAT4 result;

		result.x =
			left.x * right.element[0][0] +
			left.y * right.element[1][0] +
			left.z * right.element[2][0] +
			left.w * right.element[3][0];

		result.y =
			left.x * right.element[0][1] +
			left.y * right.element[1][1] +
			left.z * right.element[2][1] +
			left.w * right.element[3][1];

		result.z =
			left.x * right.element[0][2] +
			left.y * right.element[1][2] +
			left.z * right.element[2][2] +
			left.w * right.element[3][2];

		result.w =
			left.x * right.element[0][3] +
			left.y * right.element[1][3] +
			left.z * right.element[2][3] +
			left.w * right.element[3][3];

		return result;
	}

	HDFLOAT4 HDFloat3MultiplyMatrix(const HDFLOAT3& left, const HDFLOAT4X4& right)
	{
		HDFLOAT4 result = HDFloat4MultiplyMatrix({ left.x, left.y, left.z, 1.0f }, right);
		return { result.x ,result.y, result.z };
	}

	HDFLOAT4 QuaternionToFloat4(const HDQuternion& val)
	{
		return { val.x, val.y, val.z, val.w };
	}

	HDQuternion Float4ToQuaternion(const HDFLOAT4& val)
	{
		return { val.w, val.x, val.y, val.z };
	}

}
