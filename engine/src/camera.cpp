#include "camera.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <math.h>

namespace engine
{
	GLuint vboId;
	void Camera::init(ShaderProgram* program)
	{
		GLuint uboId = program->getUniformBlockBinding("SharedMatrices", UBO_BP);

		glGenBuffers(1, &vboId);
		glBindBuffer(GL_UNIFORM_BUFFER, vboId);
		{
			glBufferData(GL_UNIFORM_BUFFER, sizeof(Matrix4) * 2, 0, GL_STREAM_DRAW);
			glBindBufferBase(GL_UNIFORM_BUFFER, UBO_BP, vboId);
		}
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	void Camera::convert()
	{
		glBindBuffer(GL_UNIFORM_BUFFER, vboId);
		{
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Matrix4), &viewMatrix);
			glBufferSubData(GL_UNIFORM_BUFFER, sizeof(Matrix4), sizeof(Matrix4), &projectionMatrix);
		}
	}

	void Camera::setViewMatrix(Matrix4 viewMatrix)
	{
		this->viewMatrix = viewMatrix;
		this->viewMatrixInversedUpToDate = false;
	}

	void Camera::lookAt(Vector3 eye, Vector3 center, Vector3 up)
	{
		Vector3 v = center - eye;
		v.normalize();

		Vector3 s = v.cross(up);
		s.normalize();

		Vector3 u = s.cross(v);
		u.normalize();

		viewMatrix = Matrix4(
			s.x, s.y, s.z, -s.dot(eye),
			u.x, u.y, u.z, -u.dot(eye),
			-v.x, -v.y, -v.z, v.dot(eye),
			0, 0, 0, 1
		);

		viewMatrixInversedUpToDate = false;
	}

	void Camera::setOrtho(float l, float r, float b, float t, float n, float f)
	{
		projectionMatrix = Matrix4(
			2 / (r - l), 0, 0, (l + r) / (l - r),
			0, 2 / (t - b), 0, (b + t) / (b - t),
			0, 0, 2 / (n - f), (n + f) / (n - f),
			0, 0, 0, 1
		);
	}

	void Camera::setPerspective(float fov, float aspect, float n, float f)
	{
		float d = 1 / tan(fov / 2);
		projectionMatrix = Matrix4(
			d / aspect, 0, 0, 0,
			0, d, 0, 0,
			0, 0, (n + f) / (n - f), 2 * n * f / (n - f),
			0, 0, -1, 0
		);
	}

	Matrix4 Camera::getViewMatrix() { return viewMatrix; }

	Matrix3 Camera::getViewMatrixInversed()
	{
		if (!viewMatrixInversedUpToDate)
		{
			Matrix3 mat3 = Matrix3(getViewMatrix());
			viewMatrixInversed = mat3.inversed();

			viewMatrixInversedUpToDate = true;
		}

		return viewMatrixInversed;
	}

	Matrix4 Camera::getProjectionMatrix() { return projectionMatrix; }
}
