#pragma once

#include "vector.h"
#include "matrix.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <cassert>

namespace engine
{
	struct Quaternion
	{
		Quaternion() = default;
		Quaternion(float, float, float, float);
		Quaternion(float, const Vector3&);
		Quaternion(const Quaternion&) = default;
		~Quaternion() = default;

		// quaternion operations
		void normalize();
		Quaternion normalized() const;

		Quaternion conjugate() const;
		Quaternion inverse() const;

		float norm();
		float quadrance() const;

		// quaternion multiplication
		Quaternion multiply(const Quaternion&) const;

		// operators
		Quaternion operator+(const Quaternion&);
		Quaternion operator*(float) const;
		Quaternion operator/(float) const;

		// equality
		bool operator==(const Quaternion&) const;
		bool operator!=(const Quaternion&) const;

		void toAngleAxis(float&, Vector3&) const;
		Matrix4 GLRotationMatrix() const;

		// fields
		float t = 0.0;
		float x = 0.0;
		float y = 0.0;
		float z = 0.0;
	};

	// output
	std::ostream& operator<<(std::ostream&, const Quaternion&);

	Quaternion qLerp(const Quaternion& q0, const Quaternion& q1, float k);
	Quaternion qSlerp(const Quaternion& q0, const Quaternion& q1, float k);
}
