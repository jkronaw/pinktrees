#pragma once

#include <iostream>
#include "vector.h"

namespace engine
{
	struct Matrix2;
	struct Matrix3;
	struct Matrix4;

	struct Matrix2
	{
		Matrix2() = default;
		Matrix2(float, float, float, float);
		Matrix2(const Vector2& row1, const Vector2& row2);

		// scalar operators
		friend Matrix2 operator+(const Matrix2&, float);
		friend Matrix2 operator-(const Matrix2&, float);
		friend Matrix2 operator*(const Matrix2&, float);
		friend Matrix2 operator/(const Matrix2&, float);

		friend Matrix2 operator+(float, const Matrix2&);
		friend Matrix2 operator-(float, const Matrix2&);
		friend Matrix2 operator*(float, const Matrix2&);
		friend Matrix2 operator/(float, const Matrix2&);

		// assignment operators
		Matrix2& operator+=(float);
		Matrix2& operator-=(float);
		Matrix2& operator*=(float);
		Matrix2& operator/=(float);

		Matrix2& operator+=(const Matrix2&);
		Matrix2& operator-=(const Matrix2&);

		// comparison operators
		bool operator==(const Matrix2&) const;
		bool operator!=(const Matrix2&) const;

		// matrix multiplication
		Matrix2 operator*(const Matrix2&) const;
		Vector2 operator*(const Vector2&) const;

		// matrix operations
		Matrix2 operator+(const Matrix2&) const;
		Matrix2 operator-(const Matrix2&) const;

		// subscript operator
		float operator[](int index);

		float determinant() const;

		Matrix2 inversed() const;
		void inverse();

		Matrix2 transposed() const;
		void transpose();

		// IO
		friend std::istream& operator>>(std::istream&, Matrix2&);
		friend std::ostream& operator<<(std::ostream&, const Matrix2&);

		// special matrices
		static Matrix2 CreateIdentity();

		// fields
		float data[4] = { 0 };
	};

	struct Matrix3
	{
		Matrix3() = default;
		Matrix3(float, float, float,
			    float, float, float,
			    float, float, float);
		Matrix3(const Vector3&, const Vector3&, const Vector3&);
		explicit Matrix3(const Matrix4&);

		// scalar operators
		friend Matrix3 operator+(const Matrix3&, float);
		friend Matrix3 operator-(const Matrix3&, float);
		friend Matrix3 operator*(const Matrix3&, float);
		friend Matrix3 operator/(const Matrix3&, float);

		friend Matrix3 operator+(float, const Matrix3&);
		friend Matrix3 operator-(float, const Matrix3&);
		friend Matrix3 operator*(float, const Matrix3&);
		friend Matrix3 operator/(float, const Matrix3&);

		// assignment operators
		Matrix3& operator+=(float);
		Matrix3& operator-=(float);
		Matrix3& operator*=(float);
		Matrix3& operator/=(float);

		Matrix3& operator+=(const Matrix3&);
		Matrix3& operator-=(const Matrix3&);

		// comparison operators
		bool operator==(const Matrix3&) const;
		bool operator!=(const Matrix3&) const;

		// matrix multiplication
		Matrix3 operator*(const Matrix3&) const;
		Vector3 operator*(const Vector3&) const;

		// matrix operations
		Matrix3 operator+(const Matrix3&) const;
		Matrix3 operator-(const Matrix3&) const;

		// subscript operator
		float operator[](int index);

		float determinant() const;

		Matrix3 inversed() const;
		void inverse();

		Matrix3 transposed() const;
		void transpose();

		// IO
		friend std::istream& operator>>(std::istream&, Matrix3&);
		friend std::ostream& operator<<(std::ostream&, const Matrix3&);

		// special matrices
		static Matrix3 CreateIdentity();
		static Matrix3 CreateDual(const Vector3&);

		// fields
		float data[9] = { 0 };
	};

	struct Matrix4
	{
		Matrix4() = default;
		Matrix4(float, float, float, float,
			    float, float, float, float,
			    float, float, float, float,
			    float, float, float, float);
		Matrix4(const Vector4&, const Vector4&, const Vector4&, const Vector4&);
		explicit Matrix4(const Matrix3&);

		// scalar operators
		friend Matrix4 operator+(const Matrix4&, float);
		friend Matrix4 operator-(const Matrix4&, float);
		friend Matrix4 operator*(const Matrix4&, float);
		friend Matrix4 operator/(const Matrix4&, float);

		friend Matrix4 operator+(float, const Matrix4&);
		friend Matrix4 operator-(float, const Matrix4&);
		friend Matrix4 operator*(float, const Matrix4&);
		friend Matrix4 operator/(float, const Matrix4&);

		// assignment operators
		Matrix4& operator+=(float);
		Matrix4& operator-=(float);
		Matrix4& operator*=(float);
		Matrix4& operator/=(float);

		Matrix4& operator+=(const Matrix4&);
		Matrix4& operator-=(const Matrix4&);

		// comparison operators
		bool operator==(const Matrix4&) const;
		bool operator!=(const Matrix4&) const;

		// matrix multiplication
		Matrix4 operator*(const Matrix4&) const;
		Vector4 operator*(const Vector4&) const;

		// matrix operations
		Matrix4 operator+(const Matrix4&) const;
		Matrix4 operator-(const Matrix4&) const;

		// subscript operator
		float operator[](int index);

		Matrix4 transposed() const;
		void transpose();

		// IO
		friend std::istream& operator>>(std::istream&, Matrix4&);
		friend std::ostream& operator<<(std::ostream&, const Matrix4&);

		// special matrices
		static Matrix4 CreateIdentity();
		static Matrix4 CreateScale(float, float, float);
		static Matrix4 CreateScale(float);
		static Matrix4 CreateScale(const Vector3&);
		static Matrix4 CreateTranslation(float, float, float);
		static Matrix4 CreateTranslation(const Vector3&);
		static Matrix4 CreateRotationX(float);
		static Matrix4 CreateRotationY(float);
		static Matrix4 CreateRotationZ(float);
		static Matrix4 CreateRotation(float, const Vector3&);
		static Matrix4 CreateLookAt(Vector3 eye, Vector3 center, Vector3 up);
		static Matrix4 CreateOrthographicProjection(float left, float right, float bottom, float top, float near, float far);
		static Matrix4 CreatePerspectiveProjection(float fov, float aspect, float near, float far);

		// fields
		float data[16] = { 0 };
	};
}
