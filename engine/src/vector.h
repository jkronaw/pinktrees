#pragma once

#include <iostream>

namespace engine
{
	const float EPSILON = 0.00001f;

	struct Vector2;
	struct Vector3;
	struct Vector4;

	struct Vector2
	{
		Vector2() = default;
		Vector2(float, float);

		// scalar operators
		friend Vector2 operator+(const Vector2&, float);
		friend Vector2 operator-(const Vector2&, float);
		friend Vector2 operator*(const Vector2&, float);
		friend Vector2 operator/(const Vector2&, float);

		friend Vector2 operator+(float, const Vector2&);
		friend Vector2 operator-(float, const Vector2&);
		friend Vector2 operator*(float, const Vector2&);
		friend Vector2 operator/(float, const Vector2&);

		// assignment operators
		Vector2& operator+=(float);
		Vector2& operator-=(float);
		Vector2& operator*=(float);
		Vector2& operator/=(float);

		Vector2& operator+=(const Vector2&);
		Vector2& operator-=(const Vector2&);
		Vector2& operator*=(const Vector2&); // element-wise
		Vector2& operator/=(const Vector2&); // element-wise

		// comparison operators
		bool operator==(const Vector2&) const;
		bool operator!=(const Vector2&) const;

		// vector operations
		Vector2 operator+(const Vector2&) const;
		Vector2 operator-(const Vector2&) const;
		Vector2 operator*(const Vector2&) const; // element-wise
		Vector2 operator/(const Vector2&) const; // element-wise

		float dot(const Vector2&) const;

		float magnitude() const;
		Vector2 normalized() const;
		void normalize();

		void cleanToZero();

		// interpolation
		static Vector2 lerp(const Vector2&, const Vector2&, float);

		// IO
		friend std::istream& operator>>(std::istream&, Vector2&);
		friend std::ostream& operator<<(std::ostream&, const Vector2&);

		// fields
		union
		{
			float x, u = 0.0;
		};
		union
		{
			float y, v = 0.0;
		};
	};

	struct Vector3
	{
		Vector3() = default;
		Vector3(float, float, float);
		explicit Vector3(const Vector4&);

		// scalar operators
		friend Vector3 operator+(const Vector3&, float);
		friend Vector3 operator-(const Vector3&, float);
		friend Vector3 operator*(const Vector3&, float);
		friend Vector3 operator/(const Vector3&, float);

		friend Vector3 operator+(float, const Vector3&);
		friend Vector3 operator-(float, const Vector3&);
		friend Vector3 operator*(float, const Vector3&);
		friend Vector3 operator/(float, const Vector3&);

		// assignment operators
		Vector3& operator+=(float);
		Vector3& operator-=(float);
		Vector3& operator*=(float);
		Vector3& operator/=(float);

		Vector3& operator+=(const Vector3&);
		Vector3& operator-=(const Vector3&);
		Vector3& operator*=(const Vector3&); // element-wise
		Vector3& operator/=(const Vector3&); // element-wise

		// comparison operators
		bool operator==(const Vector3&) const;
		bool operator!=(const Vector3&) const;

		// vector operations
		Vector3 operator+(const Vector3&) const;
		Vector3 operator-(const Vector3&) const;
		Vector3 operator*(const Vector3&) const; // element-wise
		Vector3 operator/(const Vector3&) const; // element-wise

		float dot(const Vector3&) const;
		Vector3 cross(const Vector3&) const;

		float magnitude() const;
		Vector3 normalized() const;
		void normalize();

		void cleanToZero();

		// interpolation
		static Vector3 lerp(const Vector3&, const Vector3&, float);

		// IO
		friend std::istream& operator>>(std::istream&, Vector3&);
		friend std::ostream& operator<<(std::ostream&, const Vector3&);

		// special vectors
		static Vector3 up();

		// fields
		union
		{
			float x, r = 0.0;
		};
		union
		{
			float y, g = 0.0;
		};
		union
		{
			float z, b = 0.0;
		};
	};

	struct Vector4
	{
		Vector4() = default;
		Vector4(float, float, float, float);
		Vector4(const Vector3&, float);
		explicit Vector4(const Vector3&);

		// scalar operators
		friend Vector4 operator+(const Vector4&, float);
		friend Vector4 operator-(const Vector4&, float);
		friend Vector4 operator*(const Vector4&, float);
		friend Vector4 operator/(const Vector4&, float);

		friend Vector4 operator+(float, const Vector4&);
		friend Vector4 operator-(float, const Vector4&);
		friend Vector4 operator*(float, const Vector4&);
		friend Vector4 operator/(float, const Vector4&);

		// assignment operators
		Vector4& operator+=(float);
		Vector4& operator-=(float);
		Vector4& operator*=(float);
		Vector4& operator/=(float);

		Vector4& operator+=(const Vector4&);
		Vector4& operator-=(const Vector4&);
		Vector4& operator*=(const Vector4&); // element-wise
		Vector4& operator/=(const Vector4&); // element-wise

		// comparison operators
		bool operator==(const Vector4&) const;
		bool operator!=(const Vector4&) const;

		// vector operations
		Vector4 operator+(const Vector4&) const;
		Vector4 operator-(const Vector4&) const;
		Vector4 operator*(const Vector4&) const; // element-wise
		Vector4 operator/(const Vector4&) const; // element-wise

		float dot(const Vector4&) const;

		float magnitude() const;
		Vector4 normalized() const;
		void normalize();

		void cleanToZero();

		// interpolation
		static Vector4 lerp(const Vector4&, const Vector4&, float);

		// IO
		friend std::istream& operator>>(std::istream&, Vector4&);
		friend std::ostream& operator<<(std::ostream&, const Vector4&);

		// fields
		union
		{
			float x, r = 0.0;
		};
		union
		{
			float y, g = 0.0;
		};
		union
		{
			float z, b = 0.0;
		};
		union
		{
			float w, a = 0.0;
		};
	};

	bool floatEquals(float s1, float s2);
}
