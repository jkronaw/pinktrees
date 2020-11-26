#include "quaternion.h"
#include<math.h>

namespace engine
{
	Quaternion::Quaternion(float t, float x, float y, float z) : t(t), x(x), y(y), z(z) {}
	Quaternion::Quaternion(float theta, const Vector3& axis)
	{
		Vector3 normalized = axis.normalized();

		t = cos(theta / 2.0f);
		float s = sin(theta / 2.0f);
		x = normalized.x * s;
		y = normalized.y * s;
		z = normalized.z * s;

		normalize();
	}

	void Quaternion::normalize() { *this = *this / norm(); }
	Quaternion Quaternion::normalized() const
	{
		Quaternion q = *this;
		q.normalize();
		return q;
	}

	Quaternion Quaternion::conjugate() const { return Quaternion(t, -x, -y, -z); }
	Quaternion Quaternion::inverse() const { return conjugate() / quadrance(); }

	float Quaternion::quadrance() const { return t * t + x * x + y * y + z * z; }
	float Quaternion::norm() { return sqrt(quadrance()); }

	Quaternion Quaternion::multiply(const Quaternion& q1) const
	{
		Quaternion q;
		q.t = t * q1.t - x * q1.x - y * q1.y - z * q1.z;
		q.x = t * q1.x + x * q1.t + y * q1.z - z * q1.y;
		q.y = t * q1.y + y * q1.t + z * q1.x - x * q1.z;
		q.z = t * q1.z + z * q1.t + x * q1.y - y * q1.x;
		return q;
	}

	Quaternion Quaternion::operator+(const Quaternion& q1)
	{
		Quaternion q;
		q.t = t + q1.t;
		q.x = x + q1.x;
		q.y = y + q1.y;
		q.z = z + q1.z;
		return q;
	}
	Quaternion Quaternion::operator*(float s) const
	{
		Quaternion q;
		q.t = s * t;
		q.x = s * x;
		q.y = s * y;
		q.z = s * z;
		return q;
	}
	Quaternion Quaternion::operator/(float s) const
	{
		Quaternion q;
		q.t = t / s;
		q.x = x / s;
		q.y = y / s;
		q.z = z / s;
		return q;
	}

	void Quaternion::toAngleAxis(float& theta, Vector3& axis) const
	{
		Quaternion qn = normalized();

		theta = 2.0f * acos(qn.t);
		float s = sqrt(1.0f - qn.t * qn.t);
		if (s < EPSILON)
		{
			axis.x = 1.0f;
			axis.y = 0.0f;
			axis.z = 0.0f;
		}
		else
		{
			float sinv = 1 / s;
			axis.x = qn.x * sinv;
			axis.y = qn.y * sinv;
			axis.z = qn.z * sinv;
		}
	}
	Matrix4 Quaternion::GLRotationMatrix() const
	{
		Quaternion qn = normalized();

		float xx = qn.x * qn.x;
		float xy = qn.x * qn.y;
		float xz = qn.x * qn.z;
		float xt = qn.x * qn.t;
		float yy = qn.y * qn.y;
		float yz = qn.y * qn.z;
		float yt = qn.y * qn.t;
		float zz = qn.z * qn.z;
		float zt = qn.z * qn.t;

		Matrix4 matrix(1.0f - 2.0f * (yy + zz), 2.0f * (xy + zt), 2.0f * (xz - yt), 0.0f,
			2.0f * (xy - zt), 1.0f - 2.0f * (xx + zz), 2.0f * (yz + xt), 0.0f,
			2.0f * (xz + yt), 2.0f * (yz - xt), 1.0f - 2.0f * (xx + yy), 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);

		return matrix;
	}

	bool Quaternion::operator==(const Quaternion& q1) const
	{
		return (fabs(t - q1.t) < EPSILON && fabs(x - q1.x) < EPSILON &&
			fabs(y - q1.y) < EPSILON && fabs(z - q1.z) < EPSILON);
	}
	bool Quaternion::operator!=(const Quaternion& q1) const
	{
		return !((*this) == q1);
	}

	std::ostream& operator<<(std::ostream& os, const Quaternion& q)
	{
		os << "(" << q.t << ", " << q.x << ", " << q.y << ", " << q.z << ")";
		return os;
	}

	Quaternion qLerp(const Quaternion& q0, const Quaternion& q1, float k)
	{
		float cos_angle = q0.x * q1.x + q0.y * q1.y + q0.z * q1.z + q0.t * q1.t;
		float k0 = 1.0f - k;
		float k1 = (cos_angle > 0) ? k : -k;
		Quaternion qi = q0 * k0 + q1 * k1;
		qi.normalize();
		return qi;
	}
	Quaternion qSlerp(const Quaternion& q0, const Quaternion& q1, float k)
	{
		float angle = acos(q0.x * q1.x + q0.y * q1.y + q0.z * q1.z + q0.t * q1.t);
		float k0 = sin((1 - k) * angle) / sin(angle);
		float k1 = sin(k * angle) / sin(angle);
		Quaternion qi = q0 * k0 + q1 * k1;
		qi.normalize();
		return qi;
	}
}
