#include "vector.h"

namespace engine
{
	/* ------ constructors ------ */
	Vector2::Vector2(float x, float y) : x(x), y(y) {}
	Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
	Vector4::Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

	Vector3::Vector3(const Vector4& v)
	{
		if (floatEquals(v.z, 0.0))
		{
			y = v.y;
			x = v.x;
			z = v.z;
		}
		else
		{
			y = v.y / v.w;
			x = v.x / v.w;
			z = v.z / v.w;
		}
	}
	Vector4::Vector4(const Vector3& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		w = 1.0;
	}
	Vector4::Vector4(const Vector3& v, float s)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		w = s;
	}

	/* ------ scalar operators ------ */
	Vector2 operator+(const Vector2& v, float s) { return Vector2(v.x + s, v.y + s); }
	Vector2 operator-(const Vector2& v, float s) { return Vector2(v.x - s, v.y - s); }
	Vector2 operator*(const Vector2& v, float s) { return Vector2(v.x * s, v.y * s); }
	Vector2 operator/(const Vector2& v, float s) { return Vector2(v.x / s, v.y / s); }

	Vector3 operator+(const Vector3& v, float s) { return Vector3(v.x + s, v.y + s, v.z + s); }
	Vector3 operator-(const Vector3& v, float s) { return Vector3(v.x - s, v.y - s, v.z - s); }
	Vector3 operator*(const Vector3& v, float s) { return Vector3(v.x * s, v.y * s, v.z * s); }
	Vector3 operator/(const Vector3& v, float s) { return Vector3(v.x / s, v.y / s, v.z / s); }

	Vector4 operator+(const Vector4& v, float s) { return Vector4(v.x + s, v.y + s, v.z + s, v.w + s); }
	Vector4 operator-(const Vector4& v, float s) { return Vector4(v.x - s, v.y - s, v.z - s, v.w - s); }
	Vector4 operator*(const Vector4& v, float s) { return Vector4(v.x * s, v.y * s, v.z * s, v.w * s); }
	Vector4 operator/(const Vector4& v, float s) { return Vector4(v.x / s, v.y / s, v.z / s, v.w / s); }

	Vector2 operator+(float s, const Vector2& v) { return Vector2(s + v.x, s + v.y); }
	Vector2 operator-(float s, const Vector2& v) { return Vector2(s - v.x, s - v.y); }
	Vector2 operator*(float s, const Vector2& v) { return Vector2(s * v.x, s * v.y); }
	Vector2 operator/(float s, const Vector2& v) { return Vector2(s / v.x, s / v.y); }

	Vector3 operator+(float s, const Vector3& v) { return Vector3(s + v.x, s + v.y, s + v.z); }
	Vector3 operator-(float s, const Vector3& v) { return Vector3(s - v.x, s - v.y, s - v.z); }
	Vector3 operator*(float s, const Vector3& v) { return Vector3(s * v.x, s * v.y, s * v.z); }
	Vector3 operator/(float s, const Vector3& v) { return Vector3(s / v.x, s / v.y, s / v.z); }

	Vector4 operator+(float s, const Vector4& v) { return Vector4(s + v.x, s + v.y, s + v.z, s + v.w); }
	Vector4 operator-(float s, const Vector4& v) { return Vector4(s - v.x, s - v.y, s - v.z, s - v.w); }
	Vector4 operator*(float s, const Vector4& v) { return Vector4(s * v.x, s * v.y, s * v.z, s * v.w); }
	Vector4 operator/(float s, const Vector4& v) { return Vector4(s / v.x, s / v.y, s / v.z, s / v.w); }

	/* ------ assignment operators ------ */
	Vector2& Vector2::operator+=(float s)
	{
		x += s;
		y += s;
		return *this;
	}
	Vector2& Vector2::operator-=(float s)
	{
		x -= s;
		y -= s;
		return *this;
	}
	Vector2& Vector2::operator*=(float s)
	{
		x *= s;
		y *= s;
		return *this;
	}
	Vector2& Vector2::operator/=(float s)
	{
		x /= s;
		y /= s;
		return *this;
	}
	Vector2& Vector2::operator+=(const Vector2& v)
	{
		x += v.x;
		y += v.y;
		return *this;
	}
	Vector2& Vector2::operator-=(const Vector2& v)
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}
	Vector2& Vector2::operator*=(const Vector2& v)
	{
		x *= v.x;
		y *= v.y;
		return *this;
	}
	Vector2& Vector2::operator/=(const Vector2& v)
	{
		x /= v.x;
		y /= v.y;
		return *this;
	}

	Vector3& Vector3::operator+=(float s)
	{
		x += s;
		y += s;
		z += s;
		return *this;
	}
	Vector3& Vector3::operator-=(float s)
	{
		x -= s;
		y -= s;
		z -= s;
		return *this;
	}
	Vector3& Vector3::operator*=(float s)
	{
		x *= s;
		y *= s;
		z *= s;
		return *this;
	}
	Vector3& Vector3::operator/=(float s)
	{
		x /= s;
		y /= s;
		z /= s;
		return *this;
	}
	Vector3& Vector3::operator+=(const Vector3& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}
	Vector3& Vector3::operator-=(const Vector3& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}
	Vector3& Vector3::operator*=(const Vector3& v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
		return *this;
	}
	Vector3& Vector3::operator/=(const Vector3& v)
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;
		return *this;
	}

	Vector4& Vector4::operator+=(float s)
	{
		x += s;
		y += s;
		z += s;
		w += s;
		return *this;
	}
	Vector4& Vector4::operator-=(float s)
	{
		x -= s;
		y -= s;
		z -= s;
		w -= s;
		return *this;
	}
	Vector4& Vector4::operator*=(float s)
	{
		x *= s;
		y *= s;
		z *= s;
		w *= s;
		return *this;
	}
	Vector4& Vector4::operator/=(float s)
	{
		x /= s;
		y /= s;
		z /= s;
		w /= s;
		return *this;
	}
	Vector4& Vector4::operator+=(const Vector4& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;
		return *this;
	}
	Vector4& Vector4::operator-=(const Vector4& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		w -= v.w;
		return *this;
	}
	Vector3& Vector3::operator-=(const Vector3& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}
	Vector4& Vector4::operator*=(const Vector4& v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
		w *= v.w;
		return *this;
	}
	Vector4& Vector4::operator/=(const Vector4& v)
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;
		w /= v.w;
		return *this;
	}

	/* ------ comparison operators ------ */
	bool Vector2::operator==(const Vector2& v) const
	{
		return floatEquals(x, v.x) && floatEquals(y, v.y);
	}
	bool Vector2::operator!=(const Vector2& v) const
	{
		return !(*this == v);
	}

	bool Vector3::operator==(const Vector3& v) const
	{
		return floatEquals(x, v.x) && floatEquals(y, v.y) && floatEquals(z, v.z);
	}
	bool Vector3::operator!=(const Vector3& v) const
	{
		return !(*this == v);
	}

	bool Vector4::operator==(const Vector4& v) const
	{
		return floatEquals(x, v.x) && floatEquals(y, v.y) && floatEquals(z, v.z) && floatEquals(w, v.w);
	}
	bool Vector4::operator!=(const Vector4& v) const
	{
		return !(*this == v);
	}

	/* ------ vector operations ------ */
	Vector2 Vector2::operator+(const Vector2& v) const
	{
		return Vector2(x + v.x, y + v.y);
	}
	Vector2 Vector2::operator-(const Vector2& v) const
	{
		return Vector2(x - v.x, y - v.y);
	}
	Vector2 Vector2::operator*(const Vector2& v) const
	{
		return Vector2(x * v.x, y * v.y);
	}
	Vector2 Vector2::operator/(const Vector2& v) const
	{
		return Vector2(x / v.x, y / v.y);
	}

	Vector3 Vector3::operator+(const Vector3& v) const
	{
		return Vector3(x + v.x, y + v.y, z + v.z);
	}
	Vector3 Vector3::operator-(const Vector3& v) const
	{
		return Vector3(x - v.x, y - v.y, z - v.z);
	}
	Vector3 Vector3::operator*(const Vector3& v) const
	{
		return Vector3(x * v.x, y * v.y, z * v.z);
	}
	Vector3 Vector3::operator/(const Vector3& v) const
	{
		return Vector3(x / v.x, y / v.y, z / v.z);
	}

	Vector4 Vector4::operator+(const Vector4& v) const
	{
		return Vector4(x + v.x, y + v.y, z + v.z, w + v.w);
	}
	Vector4 Vector4::operator-(const Vector4& v) const
	{
		return Vector4(x - v.x, y - v.y, z - v.z, w - v.w);
	}
	Vector4 Vector4::operator*(const Vector4& v) const
	{
		return Vector4(x * v.x, y * v.y, z * v.z, w * v.w);
	}
	Vector4 Vector4::operator/(const Vector4& v) const
	{
		return Vector4(x / v.x, y / v.y, z / v.z, w / v.w);
	}

	/* ------ dot product ------ */
	float Vector2::dot(const Vector2& v) const
	{
		return x * v.x + y * v.y;
	}
	float Vector3::dot(const Vector3& v) const
	{
		return x * v.x + y * v.y + z * v.z;
	}
	float Vector4::dot(const Vector4& v) const
	{
		return x * v.x + y * v.y + z * v.z + w * v.w;
	}

	Vector3 Vector3::cross(const Vector3& v) const
	{
		return Vector3(
			y * v.z - z * v.y,
			z * v.x - x * v.z,
			x * v.y - y * v.x
		);
	}

	float Vector2::magnitude() const { return sqrt(x * x + y * y); }
	float Vector3::magnitude() const { return sqrt(x * x + y * y + z * z); }
	float Vector4::magnitude() const { return sqrt(x * x + y * y + z * z + w * w); }

	Vector2 Vector2::normalized() const { return (*this) / magnitude(); }
	Vector3 Vector3::normalized() const { return (*this) / magnitude(); }
	Vector4 Vector4::normalized() const { return (*this) / magnitude(); }

	void Vector2::normalize() { (*this) = (*this) / magnitude(); }
	void Vector3::normalize() { (*this) = (*this) / magnitude(); }
	void Vector4::normalize() { (*this) = (*this) / magnitude(); }

	void Vector2::cleanToZero()
	{
		if (floatEquals(x, 0.0))
		{
			x = 0.0;
		}

		if (floatEquals(y, 0.0))
		{
			y = 0.0;
		}
	}
	void Vector3::cleanToZero()
	{
		if (floatEquals(x, 0.0))
		{
			x = 0.0;
		}

		if (floatEquals(y, 0.0))
		{
			y = 0.0;
		}

		if (floatEquals(z, 0.0))
		{
			z = 0.0;
		}
	}
	void Vector4::cleanToZero()
	{
		if (floatEquals(x, 0.0))
		{
			x = 0.0;
		}

		if (floatEquals(y, 0.0))
		{
			y = 0.0;
		}

		if (floatEquals(z, 0.0))
		{
			z = 0.0;
		}

		if (floatEquals(w, 0.0))
		{
			w = 0.0;
		}
	}

	/* ------ interpolation ------ */
	Vector2 Vector2::lerp(const Vector2& v1, const Vector2& v2, float k)
	{
		return v1 * (1 - k) + v2 * k;
	}
	Vector3 Vector3::lerp(const Vector3& v1, const Vector3& v2, float k)
	{
		return v1 * (1 - k) + v2 * k;
	}
	Vector4 Vector4::lerp(const Vector4& v1, const Vector4& v2, float k)
	{
		return v1 * (1 - k) + v2 * k;
	}

	/* ------ IO ------ */
	std::istream& operator>>(std::istream& is, Vector2& vector)
	{
		is >> vector.x;
		is >> vector.y;
		return is;
	}
	std::istream& operator>>(std::istream& is, Vector3& vector)
	{
		is >> vector.x;
		is >> vector.y;
		is >> vector.z;
		return is;
	}
	std::istream& operator>>(std::istream& is, Vector4& vector)
	{
		is >> vector.x;
		is >> vector.y;
		is >> vector.z;
		is >> vector.w;
		return is;
	}

	std::ostream& operator<<(std::ostream& os, const Vector2& vector)
	{
		os << "[" << vector.x << ", " << vector.y << "]";
		return os;
	}
	std::ostream& operator<<(std::ostream& os, const Vector3& vector)
	{
		os << "[" << vector.x << ", " << vector.y << ", " << vector.z << "]";
		return os;
	}
	std::ostream& operator<<(std::ostream& os, const Vector4& vector)
	{
		os << "[" << vector.x << ", " << vector.y << ", " << vector.z << ", " << vector.w << "]";
		return os;
	}

	/* ------ special vectors ------ */
	Vector3 Vector3::up()
	{
		return Vector3(0, 1, 0);
	}

	/* ------ helper ------ */
	bool floatEquals(float s1, float s2)
	{
		return fabs(s1 - s2) < EPSILON;
	}
}