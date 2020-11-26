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
	Matrix3::Matrix3(const Matrix4& m)
	{
		data[0] = m.data[0];
		data[1] = m.data[1];
		data[2] = m.data[2];
		data[3] = m.data[4];
		data[4] = m.data[5];
		data[5] = m.data[6];
		data[6] = m.data[8];
		data[7] = m.data[9];
		data[8] = m.data[10];
	}

	Matrix4::Matrix4(float a00, float a01, float a02, float a03,
		float a10, float a11, float a12, float a13,
		float a20, float a21, float a22, float a23,
		float a30, float a31, float a32, float a33)
	{
		data[0] = a00;
		data[1] = a10;
		data[2] = a20;
		data[3] = a30;
		data[4] = a01;
		data[5] = a11;
		data[6] = a21;
		data[7] = a31;
		data[8] = a02;
		data[9] = a12;
		data[10] = a22;
		data[11] = a32;
		data[12] = a03;
		data[13] = a13;
		data[14] = a23;
		data[15] = a33;
	}
	Matrix4::Matrix4(const Vector4& row0, const Vector4& row1, const Vector4& row2, const Vector4& row3)
	{
		data[0] = row0.x;
		data[1] = row1.x;
		data[2] = row2.x;
		data[3] = row3.x;
		data[4] = row0.y;
		data[5] = row1.y;
		data[6] = row2.y;
		data[7] = row3.y;
		data[8] = row0.z;
		data[9] = row1.z;
		data[10] = row2.z;
		data[11] = row3.z;
		data[12] = row0.w;
		data[13] = row1.w;
		data[14] = row2.w;
		data[15] = row3.w;
	}
	Matrix4::Matrix4(const Matrix3& m)
	{
		data[0] = m.data[0];
		data[1] = m.data[1];
		data[2] = m.data[2];
		data[3] = 0;
		data[4] = m.data[3];
		data[5] = m.data[4];
		data[6] = m.data[5];
		data[7] = 0;
		data[8] = m.data[6];
		data[9] = m.data[7];
		data[10] = m.data[8];
		data[11] = 0;
		data[12] = 0;
		data[13] = 0;
		data[14] = 0;
		data[15] = 1;
	}

	/* ------ scalar operators ------*/
	Matrix2 operator+(const Matrix2& m, float s)
	{
		return Matrix2(
			m.data[0] + s,
			m.data[1] + s,
			m.data[2] + s,
			m.data[3] + s
		);
	}
	Matrix2 operator-(const Matrix2& m, float s)
	{
		return Matrix2(
			m.data[0] - s,
			m.data[1] - s,
			m.data[2] - s,
			m.data[3] - s
		);
	}
	Matrix2 operator*(const Matrix2& m, float s)
	{
		return Matrix2(
			m.data[0] * s,
			m.data[1] * s,
			m.data[2] * s,
			m.data[3] * s
		);
	}
	Matrix2 operator/(const Matrix2& m, float s)
	{
		return Matrix2(
			m.data[0] / s,
			m.data[1] / s,
			m.data[2] / s,
			m.data[3] / s
		);
	}

	Matrix3 operator+(const Matrix3& m, float s)
	{
		return Matrix3(
			m.data[0] + s,
			m.data[1] + s,
			m.data[2] + s,
			m.data[3] + s,
			m.data[4] + s,
			m.data[5] + s,
			m.data[6] + s,
			m.data[7] + s,
			m.data[8] + s
		);
	}
	Matrix3 operator-(const Matrix3& m, float s)
	{
		return Matrix3(
			m.data[0] - s,
			m.data[1] - s,
			m.data[2] - s,
			m.data[3] - s,
			m.data[4] - s,
			m.data[5] - s,
			m.data[6] - s,
			m.data[7] - s,
			m.data[8] - s
		);
	}
	Matrix3 operator*(const Matrix3& m, float s)
	{
		return Matrix3(
			m.data[0] * s,
			m.data[1] * s,
			m.data[2] * s,
			m.data[3] * s,
			m.data[4] * s,
			m.data[5] * s,
			m.data[6] * s,
			m.data[7] * s,
			m.data[8] * s
		);
	}
	Matrix3 operator/(const Matrix3& m, float s)
	{
		return Matrix3(
			m.data[0] / s,
			m.data[1] / s,
			m.data[2] / s,
			m.data[3] / s,
			m.data[4] / s,
			m.data[5] / s,
			m.data[6] / s,
			m.data[7] / s,
			m.data[8] / s
		);
	}

	Matrix4 operator+(const Matrix4& m, float s)
	{
		return Matrix4(
			m.data[0] + s,
			m.data[1] + s,
			m.data[2] + s,
			m.data[3] + s,
			m.data[4] + s,
			m.data[5] + s,
			m.data[6] + s,
			m.data[7] + s,
			m.data[8] + s,
			m.data[9] + s,
			m.data[10] + s,
			m.data[11] + s,
			m.data[12] + s,
			m.data[13] + s,
			m.data[14] + s,
			m.data[15] + s
		);
	}
	Matrix4 operator-(const Matrix4& m, float s)
	{
		return Matrix4(
			m.data[0] - s,
			m.data[1] - s,
			m.data[2] - s,
			m.data[3] - s,
			m.data[4] - s,
			m.data[5] - s,
			m.data[6] - s,
			m.data[7] - s,
			m.data[8] - s,
			m.data[9] - s,
			m.data[10] - s,
			m.data[11] - s,
			m.data[12] - s,
			m.data[13] - s,
			m.data[14] - s,
			m.data[15] - s
		);
	}
	Matrix4 operator*(const Matrix4& m, float s)
	{
		return Matrix4(
			m.data[0] * s,
			m.data[1] * s,
			m.data[2] * s,
			m.data[3] * s,
			m.data[4] * s,
			m.data[5] * s,
			m.data[6] * s,
			m.data[7] * s,
			m.data[8] * s,
			m.data[9] * s,
			m.data[10] * s,
			m.data[11] * s,
			m.data[12] * s,
			m.data[13] * s,
			m.data[14] * s,
			m.data[15] * s
		);
	}
	Matrix4 operator/(const Matrix4& m, float s)
	{
		return Matrix4(
			m.data[0] / s,
			m.data[1] / s,
			m.data[2] / s,
			m.data[3] / s,
			m.data[4] / s,
			m.data[5] / s,
			m.data[6] / s,
			m.data[7] / s,
			m.data[8] / s,
			m.data[9] / s,
			m.data[10] / s,
			m.data[11] / s,
			m.data[12] / s,
			m.data[13] / s,
			m.data[14] / s,
			m.data[15] / s
		);
	}

	Matrix2 operator+(float s, const Matrix2& m)
	{
		return Matrix2(
			s + m.data[0],
			s + m.data[1],
			s + m.data[2],
			s + m.data[3]
		);
	}
	Matrix2 operator-(float s, const Matrix2& m)
	{
		return Matrix2(
			s - m.data[0],
			s - m.data[1],
			s - m.data[2],
			s - m.data[3]
		);
	}
	Matrix2 operator*(float s, const Matrix2& m)
	{
		return Matrix2(
			s * m.data[0],
			s * m.data[1],
			s * m.data[2],
			s * m.data[3]
		);
	}
	Matrix2 operator/(float s, const Matrix2& m)
	{
		return Matrix2(
			s / m.data[0],
			s / m.data[1],
			s / m.data[2],
			s / m.data[3]
		);
	}

	Matrix3 operator+(float s, const Matrix3& m)
	{
		return Matrix3(
			s + m.data[0],
			s + m.data[1],
			s + m.data[2],
			s + m.data[3],
			s + m.data[4],
			s + m.data[5],
			s + m.data[6],
			s + m.data[7],
			s + m.data[8]
		);
	}
	Matrix3 operator-(float s, const Matrix3& m)
	{
		return Matrix3(
			s - m.data[0],
			s - m.data[1],
			s - m.data[2],
			s - m.data[3],
			s - m.data[4],
			s - m.data[5],
			s - m.data[6],
			s - m.data[7],
			s - m.data[8]
		);
	}
	Matrix3 operator*(float s, const Matrix3& m)
	{
		return Matrix3(
			s * m.data[0],
			s * m.data[1],
			s * m.data[2],
			s * m.data[3],
			s * m.data[4],
			s * m.data[5],
			s * m.data[6],
			s * m.data[7],
			s * m.data[8]
		);
	}
	Matrix3 operator/(float s, const Matrix3& m)
	{
		return Matrix3(
			s / m.data[0],
			s / m.data[1],
			s / m.data[2],
			s / m.data[3],
			s / m.data[4],
			s / m.data[5],
			s / m.data[6],
			s / m.data[7],
			s / m.data[8]
		);
	}

	Matrix4 operator+(float s, const Matrix4& m)
	{
		return Matrix4(
			s + m.data[0],
			s + m.data[1],
			s + m.data[2],
			s + m.data[3],
			s + m.data[4],
			s + m.data[5],
			s + m.data[6],
			s + m.data[7],
			s + m.data[8],
			s + m.data[9],
			s + m.data[10],
			s + m.data[11],
			s + m.data[12],
			s + m.data[13],
			s + m.data[14],
			s + m.data[15]
		);
	}
	Matrix4 operator-(float s, const Matrix4& m)
	{
		return Matrix4(
			s - m.data[0],
			s - m.data[1],
			s - m.data[2],
			s - m.data[3],
			s - m.data[4],
			s - m.data[5],
			s - m.data[6],
			s - m.data[7],
			s - m.data[8],
			s - m.data[9],
			s - m.data[10],
			s - m.data[11],
			s - m.data[12],
			s - m.data[13],
			s - m.data[14],
			s - m.data[15]
		);
	}
	Matrix4 operator*(float s, const Matrix4& m)
	{
		return Matrix4(
			s * m.data[0],
			s * m.data[1],
			s * m.data[2],
			s * m.data[3],
			s * m.data[4],
			s * m.data[5],
			s * m.data[6],
			s * m.data[7],
			s * m.data[8],
			s * m.data[9],
			s * m.data[10],
			s * m.data[11],
			s * m.data[12],
			s * m.data[13],
			s * m.data[14],
			s * m.data[15]
		);
	}
	Matrix4 operator/(float s, const Matrix4& m)
	{
		return Matrix4(
			s / m.data[0],
			s / m.data[1],
			s / m.data[2],
			s / m.data[3],
			s / m.data[4],
			s / m.data[5],
			s / m.data[6],
			s / m.data[7],
			s / m.data[8],
			s / m.data[9],
			s / m.data[10],
			s / m.data[11],
			s / m.data[12],
			s / m.data[13],
			s / m.data[14],
			s / m.data[15]
		);
	}

	/* ------ assignment operators ------*/
	Matrix2& Matrix2::operator+=(const Matrix2& m)
	{
		data[0] += m.data[0];
		data[1] += m.data[1];
		data[2] += m.data[2];
		data[3] += m.data[3];
		return *this;
	}
	Matrix2& Matrix2::operator-=(const Matrix2& m)
	{
		data[0] -= m.data[0];
		data[1] -= m.data[1];
		data[2] -= m.data[2];
		data[3] -= m.data[3];
		return *this;
	}
	Matrix2& Matrix2::operator+=(float s)
	{
		data[0] += s;
		data[1] += s;
		data[2] += s;
		data[3] += s;
		return *this;
	}
	Matrix2& Matrix2::operator-=(float s)
	{
		data[0] -= s;
		data[1] -= s;
		data[2] -= s;
		data[3] -= s;
		return *this;
	}
	Matrix2& Matrix2::operator*=(float s)
	{
		data[0] *= s;
		data[1] *= s;
		data[2] *= s;
		data[3] *= s;
		return *this;
	}
	Matrix2& Matrix2::operator/=(float s)
	{
		data[0] /= s;
		data[1] /= s;
		data[2] /= s;
		data[3] /= s;
		return *this;
	}

	Matrix3& Matrix3::operator+=(const Matrix3& m)
	{
		data[0] += m.data[0];
		data[1] += m.data[1];
		data[2] += m.data[2];
		data[3] += m.data[3];
		data[4] += m.data[4];
		data[5] += m.data[5];
		data[6] += m.data[6];
		data[7] += m.data[7];
		data[8] += m.data[8];
		return *this;
	}
	Matrix3& Matrix3::operator-=(const Matrix3& m)
	{
		data[0] -= m.data[0];
		data[1] -= m.data[1];
		data[2] -= m.data[2];
		data[3] -= m.data[3];
		data[4] -= m.data[4];
		data[5] -= m.data[5];
		data[6] -= m.data[6];
		data[7] -= m.data[7];
		data[8] -= m.data[8];
		return *this;
	}
	Matrix3& Matrix3::operator+=(float s)
	{
		data[0] += s;
		data[1] += s;
		data[2] += s;
		data[3] += s;
		data[4] += s;
		data[5] += s;
		data[6] += s;
		data[7] += s;
		data[8] += s;
		return *this;
	}
	Matrix3& Matrix3::operator-=(float s)
	{
		data[0] -= s;
		data[1] -= s;
		data[2] -= s;
		data[3] -= s;
		data[4] -= s;
		data[5] -= s;
		data[6] -= s;
		data[7] -= s;
		data[8] -= s;
		return *this;
	}
	Matrix3& Matrix3::operator*=(float s)
	{
		data[0] *= s;
		data[1] *= s;
		data[2] *= s;
		data[3] *= s;
		data[4] *= s;
		data[5] *= s;
		data[6] *= s;
		data[7] *= s;
		data[8] *= s;
		return *this;
	}
	Matrix3& Matrix3::operator/=(float s)
	{
		data[0] /= s;
		data[1] /= s;
		data[2] /= s;
		data[3] /= s;
		data[4] /= s;
		data[5] /= s;
		data[6] /= s;
		data[7] /= s;
		data[8] /= s;
		return *this;
	}

	Matrix4& Matrix4::operator+=(const Matrix4& m)
	{
		data[0] += m.data[0];
		data[1] += m.data[1];
		data[2] += m.data[2];
		data[3] += m.data[3];
		data[4] += m.data[4];
		data[5] += m.data[5];
		data[6] += m.data[6];
		data[7] += m.data[7];
		data[8] += m.data[8];
		data[9] += m.data[9];
		data[10] += m.data[10];
		data[11] += m.data[11];
		data[12] += m.data[12];
		data[13] += m.data[13];
		data[14] += m.data[14];
		data[15] += m.data[15];
		return *this;
	}
	Matrix4& Matrix4::operator-=(const Matrix4& m)
	{
		data[0] -= m.data[0];
		data[1] -= m.data[1];
		data[2] -= m.data[2];
		data[3] -= m.data[3];
		data[4] -= m.data[4];
		data[5] -= m.data[5];
		data[6] -= m.data[6];
		data[7] -= m.data[7];
		data[8] -= m.data[8];
		data[9] -= m.data[9];
		data[10] -= m.data[10];
		data[11] -= m.data[11];
		data[12] -= m.data[12];
		data[13] -= m.data[13];
		data[14] -= m.data[14];
		data[15] -= m.data[15];
		return *this;
	}
	Matrix4& Matrix4::operator+=(float s)
	{
		data[0] += s;
		data[1] += s;
		data[2] += s;
		data[3] += s;
		data[4] += s;
		data[5] += s;
		data[6] += s;
		data[7] += s;
		data[8] += s;
		data[9] += s;
		data[10] += s;
		data[11] += s;
		data[12] += s;
		data[13] += s;
		data[14] += s;
		data[15] += s;
		return *this;
	}
	Matrix4& Matrix4::operator-=(float s)
	{
		data[0] -= s;
		data[1] -= s;
		data[2] -= s;
		data[3] -= s;
		data[4] -= s;
		data[5] -= s;
		data[6] -= s;
		data[7] -= s;
		data[8] -= s;
		data[9] -= s;
		data[10] -= s;
		data[11] -= s;
		data[12] -= s;
		data[13] -= s;
		data[14] -= s;
		data[15] -= s;
		return *this;
	}
	Matrix4& Matrix4::operator*=(float s)
	{
		data[0] *= s;
		data[1] *= s;
		data[2] *= s;
		data[3] *= s;
		data[4] *= s;
		data[5] *= s;
		data[6] *= s;
		data[7] *= s;
		data[8] *= s;
		data[9] *= s;
		data[10] *= s;
		data[11] *= s;
		data[12] *= s;
		data[13] *= s;
		data[14] *= s;
		data[15] *= s;
		return *this;
	}
	Matrix4& Matrix4::operator/=(float s)
	{
		data[0] /= s;
		data[1] /= s;
		data[2] /= s;
		data[3] /= s;
		data[4] /= s;
		data[5] /= s;
		data[6] /= s;
		data[7] /= s;
		data[8] /= s;
		data[9] /= s;
		data[10] /= s;
		data[11] /= s;
		data[12] /= s;
		data[13] /= s;
		data[14] /= s;
		data[15] /= s;
		return *this;
	}

	/* ------ comparison operators ------*/
	bool Matrix2::operator==(const Matrix2& m) const
	{
		return floatEquals(data[0], m.data[0])
			&& floatEquals(data[1], m.data[1])
			&& floatEquals(data[2], m.data[2])
			&& floatEquals(data[3], m.data[3]);
	}
	bool Matrix2::operator!=(const Matrix2& m) const
	{
		return !(*this == m);
	}

	bool Matrix3::operator==(const Matrix3& m) const
	{
		return floatEquals(data[0], m.data[0])
			&& floatEquals(data[1], m.data[1])
			&& floatEquals(data[2], m.data[2])
			&& floatEquals(data[3], m.data[3])
			&& floatEquals(data[4], m.data[4])
			&& floatEquals(data[5], m.data[5])
			&& floatEquals(data[6], m.data[6])
			&& floatEquals(data[7], m.data[7])
			&& floatEquals(data[8], m.data[8]);
	}
	bool Matrix3::operator!=(const Matrix3& m) const
	{
		return !(*this == m);
	}

	bool Matrix4::operator==(const Matrix4& m) const
	{
		return floatEquals(data[0], m.data[0])
			&& floatEquals(data[1], m.data[1])
			&& floatEquals(data[2], m.data[2])
			&& floatEquals(data[3], m.data[3])
			&& floatEquals(data[4], m.data[4])
			&& floatEquals(data[5], m.data[5])
			&& floatEquals(data[6], m.data[6])
			&& floatEquals(data[7], m.data[7])
			&& floatEquals(data[8], m.data[8])
			&& floatEquals(data[9], m.data[9])
			&& floatEquals(data[10], m.data[10])
			&& floatEquals(data[11], m.data[11])
			&& floatEquals(data[12], m.data[12])
			&& floatEquals(data[13], m.data[13])
			&& floatEquals(data[14], m.data[14])
			&& floatEquals(data[15], m.data[15]);
	}
	bool Matrix4::operator!=(const Matrix4& m) const
	{
		return !(*this == m);
	}

	/* ------ matrix multiplication ------*/
	Vector2 Matrix2::operator*(const Vector2& v) const
	{
		return Vector2(
			data[0] * v.x + data[2] * v.y,
			data[1] * v.x + data[3] * v.y
		);
	}
	Matrix2 Matrix2::operator*(const Matrix2& m) const
	{
		return Matrix2(
			data[0] * m.data[0] + data[2] * m.data[1],
			data[0] * m.data[2] + data[2] * m.data[3],
			data[1] * m.data[2] + data[3] * m.data[3],
			data[1] * m.data[2] + data[3] * m.data[3]);
	}

	Vector3 Matrix3::operator*(const Vector3& v) const
	{
		return Vector3(rows[0].dot(vec),
			rows[1].dot(vec),
			rows[2].dot(vec));
	}
	Matrix3 Matrix3::operator*(const Matrix3& m) const
	{
		Matrix3 t = m.transposed();
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

	Vector4 Matrix4::operator*(const Vector4& v) const
	{
		return Vector4(rows[0].dot(vec),
			rows[1].dot(vec),
			rows[2].dot(vec),
			rows[3].dot(vec));
	}
	Matrix4 Matrix4::operator*(const Matrix4& m) const
	{
		Matrix4 t = m.transposed();
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

	/* ------ matrix operations ------*/
	Matrix2 Matrix2::operator+(const Matrix2& m) const
	{
		return Matrix2(
			data[0] + m.data[0],
			data[1] + m.data[1],
			data[2] + m.data[2],
			data[3] + m.data[3]
		);
	}
	Matrix2 Matrix2::operator-(const Matrix2& m) const
	{
		return Matrix2(
			data[0] - m.data[0],
			data[1] - m.data[1],
			data[2] - m.data[2],
			data[3] - m.data[3]
		);
	}

	Matrix3 Matrix3::operator+(const Matrix3& m) const
	{
		return Matrix3(
			data[0] + m.data[0],
			data[1] + m.data[1],
			data[2] + m.data[2],
			data[3] + m.data[3],
			data[4] + m.data[4],
			data[5] + m.data[5],
			data[6] + m.data[6],
			data[7] + m.data[7],
			data[8] + m.data[8]
		);
	}
	Matrix3 Matrix3::operator-(const Matrix3& m) const
	{
		return Matrix3(
			data[0] - m.data[0],
			data[1] - m.data[1],
			data[2] - m.data[2],
			data[3] - m.data[3],
			data[4] - m.data[4],
			data[5] - m.data[5],
			data[6] - m.data[6],
			data[7] - m.data[7],
			data[8] - m.data[8]
		);
	}

	Matrix4 Matrix4::operator+(const Matrix4& m) const
	{
		return Matrix4(
			data[0] + m.data[0],
			data[1] + m.data[1],
			data[2] + m.data[2],
			data[3] + m.data[3],
			data[4] + m.data[4],
			data[5] + m.data[5],
			data[6] + m.data[6],
			data[7] + m.data[7],
			data[8] + m.data[8],
			data[9] + m.data[9],
			data[10] + m.data[10],
			data[11] + m.data[11],
			data[12] + m.data[12],
			data[13] + m.data[13],
			data[14] + m.data[14],
			data[15] + m.data[15]
		);
	}
	Matrix4 Matrix4::operator-(const Matrix4& m) const
	{
		return Matrix4(
			data[0] - m.data[0],
			data[1] - m.data[1],
			data[2] - m.data[2],
			data[3] - m.data[3],
			data[4] - m.data[4],
			data[5] - m.data[5],
			data[6] - m.data[6],
			data[7] - m.data[7],
			data[8] - m.data[8],
			data[9] - m.data[9],
			data[10] - m.data[10],
			data[11] - m.data[11],
			data[12] - m.data[12],
			data[13] - m.data[13],
			data[14] - m.data[14],
			data[15] - m.data[15]
		);
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

	/* ------ IO ------ */
	std::istream& operator>>(std::istream& is, Matrix2& m)
	{
		is >> m.rows[0] >> m.rows[1];
		return is;
	}
	std::istream& operator>>(std::istream& is, Matrix3& m)
	{
		is >> m.rows[0] >> m.rows[1] >> m.rows[2];
		return is;
	}
	std::istream& operator>>(std::istream& is, Matrix4& m)
	{
		is >> m.rows[0] >> m.rows[1] >> m.rows[2] >> m.rows[3];
		return is;
	}

	std::ostream& operator<<(std::ostream& os, const Matrix2& m)
	{
		os << "[" << m.rows[0] << std::endl;
		os << " " << m.rows[1] << "]";
		return os;
	}
	std::ostream& operator<<(std::ostream& os, const Matrix4& m)
	{
		os << "[" << m.rows[0] << std::endl;
		os << " " << m.rows[1] << std::endl;
		os << " " << m.rows[2] << std::endl;
		os << " " << m.rows[3] << "]" << std::endl;
		return os;
	}
	std::ostream& operator<<(std::ostream& os, const Matrix3& m)
	{
		os << "[" << m.rows[0] << std::endl;
		os << " " << m.rows[1] << std::endl;
		os << " " << m.rows[2] << "]" << std::endl;
		return os;
	}

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

	Matrix3 Matrix3::CreateDual(const Vector3& v)
	{
		return Matrix3(
			0.0, -v.z, v.y,
			v.z, 0.0, -v.x,
			-v.y, v.x, 0.0);
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
	Matrix4 Matrix4::CreateTranslation(const Vector3& v)
	{
		return Matrix4(
			1.0, 0.0, 0.0, v.x,
			0.0, 1.0, 0.0, v.y,
			0.0, 0.0, 1.0, v.z,
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
}
