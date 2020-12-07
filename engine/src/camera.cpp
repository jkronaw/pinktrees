#include "camera.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <math.h>

namespace engine
{
	Camera::Camera(GLuint uboBP) : uboBP(uboBP)
	{
		glGenBuffers(1, &uboId);
		glBindBuffer(GL_UNIFORM_BUFFER, uboId);
		{
			glBufferData(GL_UNIFORM_BUFFER, sizeof(Matrix4) * 2, 0, GL_STREAM_DRAW);
			glBindBufferBase(GL_UNIFORM_BUFFER, uboBP, uboId);
		}
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	Camera::~Camera()
	{
		glDeleteBuffers(1, &uboId);
	}

	GLuint Camera::getUboBP()
	{
		return uboBP;
	}

	void Camera::bind()
	{
		glBindBuffer(GL_UNIFORM_BUFFER, uboId);
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
		viewMatrix = Matrix4::CreateLookAt(eye, center, up);
		viewMatrixInversedUpToDate = false;
	}

	void Camera::setOrtho(float l, float r, float b, float t, float n, float f)
	{
		projectionMatrix = Matrix4::CreateOrthographicProjection(l, r, b, t, n, f);
	}

	void Camera::setPerspective(float fov, float aspect, float n, float f)
	{
		projectionMatrix = Matrix4::CreatePerspectiveProjection(fov, aspect, n, f);
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
