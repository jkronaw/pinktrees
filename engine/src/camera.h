#pragma once

#include "shader.h"
#include "vector.h"
#include "matrix.h"

namespace engine
{
	class Camera
	{
	public:
		Camera(GLuint uboBP);
		~Camera();

		GLuint getUboBP();

		void bind();

		void lookAt(Vector3 eye, Vector3 center, Vector3 up);
		void setOrtho(float l, float r, float b, float t, float n, float f);
		void setPerspective(float fov, float aspect, float n, float f);

		Matrix4 getViewMatrix();
		void setViewMatrix(Matrix4);

		Matrix3 getViewMatrixInversed();
		Matrix4 getProjectionMatrix();

	private:
		Matrix4 viewMatrix;
		Matrix3 viewMatrixInversed;

		Matrix4 projectionMatrix;

		bool viewMatrixInversedUpToDate = false;

		GLuint uboId;
		GLuint uboBP;
	};
}