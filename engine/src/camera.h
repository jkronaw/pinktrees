#pragma once

#include "shader.h"
#include "vector.h"
#include "matrix.h"

namespace engine
{
	const float CAMERA_MOUSE_SENSITIVITY = 0.002f;
	const float CAMERA_SPEED = 2.5f;

	class Camera
	{
	public:
		Camera(GLuint uboBP);
		~Camera();

		GLuint getUboBP() const;

		// projection matrix
		Matrix4 getProjectionMatrix() const;
		void setOrtho(float l, float r, float b, float t, float n, float f);
		void setPerspective(float fov, float aspect, float n, float f);

		// view matrix
		Matrix4 getViewMatrix() const;
		void lookAt(Vector3 eye, Vector3 center);

		// camera properties
		float getPitch();
		float getYaw();
		Vector3 getPosition();

		void update(double elapsedSecs, Vector2 cursorDiff);

	private:
		GLuint uboId;
		GLuint uboBP;

		Matrix4 projectionMatrix;
		Matrix4 viewMatrix;

		float pitch = 0;
		float yaw = 0;
		Vector3 position;
	};
}