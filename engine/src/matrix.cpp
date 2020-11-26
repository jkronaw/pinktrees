#include "matrix.h"

namespace engine
{
	/* ------ constructors ------*/
	Matrix2::Matrix2(float a00, float a01, float a10, float a11)
	{
		data[0] = a00;
		data[1] = a10;
		data[2] = a01;
		data[3] = a11;

	}
	Matrix2::Matrix2(const Vector2& row0, const Vector2& row1)
	{
		data[0] = row0.x;
		data[1] = row1.x;
		data[2] = row0.y;
		data[3] = row1.y;
	}

	Matrix3::Matrix3(float a00, float a01, float a02,
		float a10, float a11, float a12,
		float a20, float a21, float a22)
	{
		data[0] = a00;
		data[1] = a10;
		data[2] = a20;
		data[3] = a01;
		data[4] = a11;
		data[5] = a21;
		data[6] = a02;
		data[7] = a12;
		data[8] = a22;
	}
	Matrix3::Matrix3(const Vector3& row0, const Vector3& row1, const Vector3& row2)
	{
		data[0] = row0.x;
		data[1] = row1.x;
		data[2] = row2.x;
		data[3] = row0.y;
		data[4] = row1.y;
		data[5] = row2.y;
		data[6] = row0.z;
		data[7] = row1.z;
		data[8] = row2.z;
	}
	Matrix3::Matrix3(const Matrix4& mat)
	{
		rows[0] = Vector3(mat.rows[0].x, mat.rows[0].y, mat.rows[0].z);
		rows[1] = Vector3(mat.rows[1].x, mat.rows[1].y, mat.rows[1].z);
		rows[2] = Vector3(mat.rows[2].x, mat.rows[2].y, mat.rows[2].z);
	}

	Matrix4::Matrix4(float a1, float a2, float a3, float a4,
		float a5, float a6, float a7, float a8,
		float a9, float a10, float a11, float a12,
		float a13, float a14, float a15, float a16)
	{
		rows[0] = Vector4(a1, a2, a3, a4);
		rows[1] = Vector4(a5, a6, a7, a8);
		rows[2] = Vector4(a9, a10, a11, a12);
		rows[3] = Vector4(a13, a14, a15, a16);
	}
	Matrix4::Matrix4(const Vector4& v1, const Vector4& v2, const Vector4& v3, const Vector4& v4)
	{
		rows[0] = v1;
		rows[1] = v2;
		rows[2] = v3;
		rows[3] = v4;
	}
	Matrix4::Matrix4(const Matrix3& mat)
	{
		rows[0] = Vector4(mat.rows[0], 0);
		rows[1] = Vector4(mat.rows[1], 0);
		rows[2] = Vector4(mat.rows[2], 0);
		rows[3] = Vector4(0, 0, 0, 1);
	}

	/* ------ scalar operators ------*/
	Matrix2 Matrix2::operator+(float scalar) const
	{
		return Matrix2(rows[0] + scalar, rows[1] + scalar);
	}
	Matrix2 Matrix2::operator-(float scalar) const
	{
		return Matrix2(rows[0] - scalar, rows[1] - scalar);
	}
	Matrix2 Matrix2::operator*(float scalar) const
	{
		return Matrix2(rows[0] * scalar, rows[1] * scalar);
	}
	Matrix2 Matrix2::operator/(float scalar) const
	{
		return Matrix2(rows[0] / scalar, rows[1] / scalar);
	}

	Matrix3 Matrix3::operator+(float scalar) const
	{
		return Matrix3(rows[0] + scalar,
			rows[1] + scalar,
			rows[2] + scalar);
	}
	Matrix3 Matrix3::operator-(float scalar) const
	{
		return Matrix3(rows[0] - scalar,
			rows[1] - scalar,
			rows[2] - scalar);
	}
	Matrix3 Matrix3::operator*(float scalar) const
	{
		return Matrix3(rows[0] * scalar,
			rows[1] * scalar,
			rows[2] * scalar);
	}
	Matrix3 Matrix3::operator/(float scalar) const
	{
		return Matrix3(rows[0] / scalar,
			rows[1] / scalar,
			rows[2] / scalar);
	}

	Matrix4 Matrix4::operator+(float scalar) const
	{
		return Matrix4(rows[0] + scalar,
			rows[1] + scalar,
			rows[2] + scalar,
			rows[3] + scalar);
	}
	Matrix4 Matrix4::operator-(float scalar) const
	{
		return Matrix4(rows[0] - scalar,
			rows[1] - scalar,
			rows[2] - scalar,
			rows[3] - scalar);
	}
	Matrix4 Matrix4::operator*(float scalar) const
	{
		return Matrix4(rows[0] * scalar,
			rows[1] * scalar,
			rows[2] * scalar,
			rows[3] * scalar);
	}
	Matrix4 Matrix4::operator/(float scalar) const
	{
		return Matrix4(rows[0] / scalar,
			rows[1] / scalar,
			rows[2] / scalar,
			rows[3] / scalar);
	}

	Matrix2 operator+(float scalar, const Matrix2& mat)
	{
		return mat + scalar;
	}
	Matrix2 operator-(float scalar, const Matrix2& mat)
	{
		return mat - scalar;
	}
	Matrix2 operator*(float scalar, const Matrix2& mat)
	{
		return mat * scalar;
	}
	Matrix2 operator/(float scalar, const Matrix2& mat)
	{
		return mat / scalar;
	}

	Matrix3 operator+(float scalar, const Matrix3& mat)
	{
		return mat + scalar;
	}
	Matrix3 operator-(float scalar, const Matrix3& mat)
	{
		return mat - scalar;
	}
	Matrix3 operator*(float scalar, const Matrix3& mat)
	{
		return mat * scalar;
	}
	Matrix3 operator/(float scalar, const Matrix3& mat)
	{
		return mat / scalar;
	}

	Matrix4 operator+(float scalar, const Matrix4& mat)
	{
		return mat + scalar;
	}
	Matrix4 operator-(float scalar, const Matrix4& mat)
	{
		return mat - scalar;
	}
	Matrix4 operator*(float scalar, const Matrix4& mat)
	{
		return mat * scalar;
	}
	Matrix4 operator/(float scalar, const Matrix4& mat)
	{
		return mat / scalar;
	}

	/* ------ assignment operators ------*/
	Matrix2& Matrix2::operator+=(const Matrix2& mat)
	{
		rows[0] += mat.rows[0];
		rows[1] += mat.rows[1];
		return *this;
	}
	Matrix2& Matrix2::operator-=(const Matrix2& mat)
	{
		rows[0] -= mat.rows[0];
		rows[1] -= mat.rows[1];
		return *this;
	}
	Matrix2& Matrix2::operator+=(float scalar)
	{
		rows[0] += scalar;
		rows[1] += scalar;
		return *this;
	}
	Matrix2& Matrix2::operator-=(float scalar)
	{
		rows[0] -= scalar;
		rows[1] -= scalar;
		return *this;
	}
	Matrix2& Matrix2::operator*=(float scalar)
	{
		rows[0] *= scalar;
		rows[1] *= scalar;
		return *this;
	}
	Matrix2& Matrix2::operator/=(float scalar)
	{
		rows[0] /= scalar;
		rows[1] /= scalar;
		return *this;
	}

	Matrix3& Matrix3::operator+=(const Matrix3& mat)
	{
		rows[0] += mat.rows[0];
		rows[1] += mat.rows[1];
		rows[2] += mat.rows[2];
		return *this;
	}
	Matrix3& Matrix3::operator-=(const Matrix3& mat)
	{
		rows[0] -= mat.rows[0];
		rows[1] -= mat.rows[1];
		rows[2] -= mat.rows[2];
		return *this;
	}
	Matrix3& Matrix3::operator+=(float scalar)
	{
		rows[0] += scalar;
		rows[1] += scalar;
		rows[2] += scalar;
		return *this;
	}
	Matrix3& Matrix3::operator-=(float scalar)
	{
		rows[0] -= scalar;
		rows[1] -= scalar;
		rows[2] -= scalar;
		return *this;
	}
	Matrix3& Matrix3::operator*=(float scalar)
	{
		rows[0] *= scalar;
		rows[1] *= scalar;
		rows[2] *= scalar;
		return *this;
	}
	Matrix3& Matrix3::operator/=(float scalar)
	{
		rows[0] /= scalar;
		rows[1] /= scalar;
		rows[2] /= scalar;
		return *this;
	}

	Matrix4& Matrix4::operator+=(const Matrix4& mat)
	{
		rows[0] += mat.rows[0];
		rows[1] += mat.rows[1];
		rows[2] += mat.rows[2];
		rows[3] += mat.rows[3];
		return *this;
	}
	Matrix4& Matrix4::operator-=(const Matrix4& mat)
	{
		rows[0] -= mat.rows[0];
		rows[1] -= mat.rows[1];
		rows[2] -= mat.rows[2];
		rows[3] -= mat.rows[3];
		return *this;
	}
	Matrix4& Matrix4::operator+=(float scalar)
	{
		rows[0] += scalar;
		rows[1] += scalar;
		rows[2] += scalar;
		rows[3] += scalar;

		return *this;
	}
	Matrix4& Matrix4::operator-=(float scalar)
	{
		rows[0] -= scalar;
		rows[1] -= scalar;
		rows[2] -= scalar;
		rows[3] -= scalar;
		return *this;
	}
	Matrix4& Matrix4::operator*=(float scalar)
	{
		rows[0] *= scalar;
		rows[1] *= scalar;
		rows[2] *= scalar;
		rows[3] *= scalar;
		return *this;
	}
	Matrix4& Matrix4::operator/=(float scalar)
	{
		rows[0] /= scalar;
		rows[1] /= scalar;
		rows[2] /= scalar;
		rows[3] /= scalar;
		return *this;
	}

	/* ------ comparison operators ------*/
	bool Matrix2::operator==(const Matrix2& mat) const
	{
		return rows[0] == mat.rows[0] && rows[1] == mat.rows[1];
	}
	bool Matrix2::operator!=(const Matrix2& mat) const
	{
		return !(*this == mat);
	}

	bool Matrix3::operator==(const Matrix3& mat) const
	{
		return rows[0] == mat.rows[0] && rows[1] == mat.rows[1] && mat.rows[2] == rows[2];
	}
	bool Matrix3::operator!=(const Matrix3& mat) const
	{
		return !(*this == mat);
	}

	bool Matrix4::operator==(const Matrix4& mat) const
	{
		return rows[0] == mat.rows[0] && rows[1] == mat.rows[1] && mat.rows[2] == rows[2] && mat.rows[3] == rows[3];
	}
	bool Matrix4::operator!=(const Matrix4& mat) const
	{
		return !(*this == mat);
	}

	/* ------ matrix operations ------*/
	Matrix2 Matrix2::operator+(const Matrix2& mat) const
	{
		return Matrix2(rows[0] + mat.rows[0], rows[1] + mat.rows[1]);
	}
	Matrix2 Matrix2::operator-(const Matrix2& mat) const
	{
		return Matrix2(rows[0] - mat.rows[0], rows[1] - mat.rows[1]);
	}

	Matrix3 Matrix3::operator+(const Matrix3& mat) const
	{
		return Matrix3(rows[0] + mat.rows[0],
			rows[1] + mat.rows[1],
			rows[2] + mat.rows[2]);
	}
	Matrix3 Matrix3::operator-(const Matrix3& mat) const
	{
		return Matrix3(rows[0] - mat.rows[0],
			rows[1] - mat.rows[1],
			rows[2] - mat.rows[2]);
	}

	Matrix4 Matrix4::operator+(const Matrix4& mat) const
	{
		return Matrix4(rows[0] + mat.rows[0],
			rows[1] + mat.rows[1],
			rows[2] + mat.rows[2],
			rows[3] + mat.rows[3]);
	}
	Matrix4 Matrix4::operator-(const Matrix4& mat) const
	{
		return Matrix4(rows[0] - mat.rows[0],
			rows[1] - mat.rows[1],
			rows[2] - mat.rows[2],
			rows[3] - mat.rows[3]);
	}

	Vector2 Matrix2::multiply(const Vector2& vec) const
	{
		return Vector2(rows[0].dot(vec), rows[1].dot(vec));
	}
	Matrix2 Matrix2::multiply(const Matrix2& mat) const
	{
		Matrix2 t = mat.transposed();
		return Matrix2(rows[0].dot(t.rows[0]),
			rows[0].dot(t.rows[1]),
			rows[1].dot(t.rows[0]),
			rows[1].dot(t.rows[1]));
	}

	Vector3 Matrix3::multiply(const Vector3& vec) const
	{
		return Vector3(rows[0].dot(vec),
			rows[1].dot(vec),
			rows[2].dot(vec));
	}
	Matrix3 Matrix3::multiply(const Matrix3& mat) const
	{
		Matrix3 t = mat.transposed();
		return Matrix3(rows[0].dot(t.rows[0]),
			rows[0].dot(t.rows[1]),
			rows[0].dot(t.rows[2]),
			rows[1].dot(t.rows[0]),
			rows[1].dot(t.rows[1]),
			rows[1].dot(t.rows[2]),
			rows[2].dot(t.rows[0]),
			rows[2].dot(t.rows[1]),
			rows[2].dot(t.rows[2]));
	}

	Vector4 Matrix4::multiply(const Vector4& vec) const
	{
		return Vector4(rows[0].dot(vec),
			rows[1].dot(vec),
			rows[2].dot(vec),
			rows[3].dot(vec));
	}
	Matrix4 Matrix4::multiply(const Matrix4& mat) const
	{
		Matrix4 t = mat.transposed();
		return Matrix4(rows[0].dot(t.rows[0]),
			rows[0].dot(t.rows[1]),
			rows[0].dot(t.rows[2]),
			rows[0].dot(t.rows[3]),
			rows[1].dot(t.rows[0]),
			rows[1].dot(t.rows[1]),
			rows[1].dot(t.rows[2]),
			rows[1].dot(t.rows[3]),
			rows[2].dot(t.rows[0]),
			rows[2].dot(t.rows[1]),
			rows[2].dot(t.rows[2]),
			rows[2].dot(t.rows[3]),
			rows[3].dot(t.rows[0]),
			rows[3].dot(t.rows[1]),
			rows[3].dot(t.rows[2]),
			rows[3].dot(t.rows[3]));
	}

	float Matrix2::determinant() const
	{
		return rows[0].x * rows[1].y - rows[0].y * rows[1].x;
	}
	float Matrix3::determinant() const
	{
		return rows[0].x * Matrix2(rows[1].y, rows[1].z, rows[2].y, rows[2].z).determinant()
			- rows[0].y * Matrix2(rows[1].x, rows[1].z, rows[2].x, rows[2].z).determinant()
			+ rows[0].z * Matrix2(rows[1].x, rows[1].y, rows[2].x, rows[2].y).determinant();
	}

	Matrix2 Matrix2::inversed() const
	{
		float d = determinant();
		if (d != 0)
		{
			return Matrix2(rows[1].y, -rows[0].y, -rows[1].x, rows[0].x) / d;
		}
		else
		{
			throw "Matrix is not invertible";
		}
	}
	Matrix3 Matrix3::inversed() const
	{
		float de = determinant();
		if (de != 0)
		{
			float c1 = Matrix2(rows[1].y, rows[1].z, rows[2].y, rows[2].z).determinant();
			float c2 = Matrix2(rows[1].x, rows[1].z, rows[2].x, rows[2].z).determinant();
			float c3 = Matrix2(rows[1].x, rows[1].y, rows[2].x, rows[2].y).determinant();
			float c4 = Matrix2(rows[0].y, rows[0].z, rows[2].y, rows[2].z).determinant();
			float c5 = Matrix2(rows[0].x, rows[0].z, rows[2].x, rows[2].z).determinant();
			float c6 = Matrix2(rows[0].x, rows[0].y, rows[2].x, rows[2].y).determinant();
			float c7 = Matrix2(rows[0].y, rows[0].z, rows[1].y, rows[1].z).determinant();
			float c8 = Matrix2(rows[0].x, rows[0].z, rows[1].x, rows[1].z).determinant();
			float c9 = Matrix2(rows[0].x, rows[0].y, rows[1].x, rows[1].y).determinant();

			Matrix3 adjugate(c1, -c2, c3,
				-c4, c5, -c6,
				c7, -c8, c9);

			return adjugate.transposed() / de;
		}
		else
		{
			throw "Matrix is not invertible";
		}
	}

	void Matrix2::inverse() { (*this) = inversed(); }
	void Matrix3::inverse() { (*this) = inversed(); }

	Matrix2 Matrix2::transposed() const
	{
		return Matrix2(rows[0].x, rows[1].x,
			rows[0].y, rows[1].y);
	}
	Matrix3 Matrix3::transposed() const
	{
		return Matrix3(rows[0].x, rows[1].x, rows[2].x,
			rows[0].y, rows[1].y, rows[2].y,
			rows[0].z, rows[1].z, rows[2].z);
	}
	Matrix4 Matrix4::transposed() const
	{
		return Matrix4(rows[0].x, rows[1].x, rows[2].x, rows[3].x,
			rows[0].y, rows[1].y, rows[2].y, rows[3].y,
			rows[0].z, rows[1].z, rows[2].z, rows[3].z,
			rows[0].w, rows[1].w, rows[2].w, rows[3].w);
	}

	void Matrix2::transpose() { (*this) = transposed(); }
	void Matrix3::transpose() { (*this) = transposed(); }
	void Matrix4::transpose() { (*this) = transposed(); }

	/* ------ special matrices ------*/
	Matrix2 Matrix2::CreateIdentity()
	{
		return Matrix2(1.0, 0.0, 0.0, 1.0);
	}
	Matrix3 Matrix3::CreateIdentity()
	{
		return Matrix3(
			1.0, 0.0, 0.0,
			0.0, 1.0, 0.0,
			0.0, 0.0, 1.0);
	}
	Matrix4 Matrix4::CreateIdentity()
	{
		return Matrix4(
			1.0, 0.0, 0.0, 0.0,
			0.0, 1.0, 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			0.0, 0.0, 0.0, 1.0);
	}

	Matrix3 Matrix3::CreateDual(const Vector3& vec)
	{
		return Matrix3(
			0.0, -vec.z, vec.y,
			vec.z, 0.0, -vec.x,
			-vec.y, vec.x, 0.0);
	}

	Matrix4 Matrix4::CreateScale(float sx, float sy, float sz)
	{
		return Matrix4(
			sx, 0.0, 0.0, 0.0,
			0.0, sy, 0.0, 0.0,
			0.0, 0.0, sz, 0.0,
			0.0, 0.0, 0.0, 1.0);
	}
	Matrix4 Matrix4::CreateTranslation(float tx, float ty, float tz)
	{
		return Matrix4(
			1.0, 0.0, 0.0, tx,
			0.0, 1.0, 0.0, ty,
			0.0, 0.0, 1.0, tz,
			0.0, 0.0, 0.0, 1.0);
	}
	Matrix4 Matrix4::CreateTranslation(const Vector3& vec)
	{
		return Matrix4(
			1.0, 0.0, 0.0, vec.x,
			0.0, 1.0, 0.0, vec.y,
			0.0, 0.0, 1.0, vec.z,
			0.0, 0.0, 0.0, 1.0);
	}
	Matrix4 Matrix4::CreateRotationX(float theta)
	{
		return Matrix4(
			1.0, 0.0, 0.0, 0.0,
			0.0, cos(theta), -sin(theta), 0.0,
			0.0, sin(theta), cos(theta), 0.0,
			0.0, 0.0, 0.0, 1.0);
	}
	Matrix4 Matrix4::CreateRotationY(float theta)
	{
		return Matrix4(
			cos(theta), 0.0, sin(theta), 0.0,
			0.0, 1.0, 0.0, 0.0,
			-sin(theta), 0.0, cos(theta), 0.0,
			0.0, 0.0, 0.0, 1.0);
	}
	Matrix4 Matrix4::CreateRotationZ(float theta)
	{
		return Matrix4(
			cos(theta), -sin(theta), 0.0, 0.0,
			sin(theta), cos(theta), 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			0.0, 0.0, 0.0, 1.0);
	}

	Matrix4 Matrix4::CreateRotation(float theta, const Vector3& axis)
	{
		Vector3 k = axis.normalized();
		Matrix3 dualMatrix = Matrix3::CreateDual(k);
		Matrix3 rotation = Matrix3::CreateIdentity() + sin(theta) * dualMatrix + (1 - cos(theta)) * dualMatrix * dualMatrix;
		return Matrix4(rotation);
	}

	/* ------ IO ------ */
	std::istream& operator>>(std::istream& is, Matrix2& mat)
	{
		is >> mat.rows[0] >> mat.rows[1];
		return is;
	}
	std::istream& operator>>(std::istream& is, Matrix3& mat)
	{
		is >> mat.rows[0] >> mat.rows[1] >> mat.rows[2];
		return is;
	}
	std::istream& operator>>(std::istream& is, Matrix4& mat)
	{
		is >> mat.rows[0] >> mat.rows[1] >> mat.rows[2] >> mat.rows[3];
		return is;
	}

	std::ostream& operator<<(std::ostream& os, const Matrix2& mat)
	{
		os << "[" << mat.rows[0] << std::endl;
		os << " " << mat.rows[1] << "]";
		return os;
	}
	std::ostream& operator<<(std::ostream& os, const Matrix4& mat)
	{
		os << "[" << mat.rows[0] << std::endl;
		os << " " << mat.rows[1] << std::endl;
		os << " " << mat.rows[2] << std::endl;
		os << " " << mat.rows[3] << "]" << std::endl;
		return os;
	}
	std::ostream& operator<<(std::ostream& os, const Matrix3& mat)
	{
		os << "[" << mat.rows[0] << std::endl;
		os << " " << mat.rows[1] << std::endl;
		os << " " << mat.rows[2] << "]" << std::endl;
		return os;
	}
}
