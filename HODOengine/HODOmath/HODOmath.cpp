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

}
