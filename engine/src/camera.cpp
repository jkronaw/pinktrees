#include "camera.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <math.h>

#include "engine.h"

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
	Camera::~Camera() { glDeleteBuffers(1, &uboId); }

	GLuint Camera::getUboBP() const { return uboBP; }

	Matrix4 Camera::getProjectionMatrix() const { return projectionMatrix; }
	void Camera::setOrtho(float l, float r, float b, float t, float n, float f)
	{
		projectionMatrix = Matrix4::CreateOrthographicProjection(l, r, b, t, n, f);
	}
	void Camera::setPerspective(float fov, float aspect, float n, float f)
	{
		projectionMatrix = Matrix4::CreatePerspectiveProjection(fov, aspect, n, f);
	}

	Matrix4 Camera::getViewMatrix() const { return viewMatrix; }
	void Camera::lookAt(Vector3 eye, Vector3 center)
	{
		Vector3 up = Vector3::up();
		Matrix4 matrix = Matrix4::CreateLookAt(eye, center, up);

		Vector3 view = center - eye;
		pitch = atan(view.y);
		yaw = atan2(view.x, view.z);
		position = eye;
	}

	float Camera::getPitch() { return pitch; }
	float Camera::getYaw() { return yaw; }
	Vector3 Camera::getPosition() { return position; }

	void Camera::update(double elapsedSecs, Vector2 cursorDiff)
	{
		Engine& engine = Engine::getInstance();

		// handle mouse
		pitch += -cursorDiff.y * CAMERA_MOUSE_SENSITIVITY;
		yaw += cursorDiff.x * CAMERA_MOUSE_SENSITIVITY;

		// handle keyboard
		bool forwardPressed = engine.getKey(GLFW_KEY_W) == GLFW_PRESS;
		bool backwardsPressed = engine.getKey(GLFW_KEY_S) == GLFW_PRESS;

		bool leftPressed = engine.getKey(GLFW_KEY_A) == GLFW_PRESS;
		bool rightPressed = engine.getKey(GLFW_KEY_D) == GLFW_PRESS;

		bool ascendPressed = engine.getKey(GLFW_KEY_SPACE) == GLFW_PRESS;
		bool descendPressed = engine.getKey(GLFW_KEY_C) == GLFW_PRESS;

		Vector3 cameraVelocity = Vector3(0, 0, 0);
		if (forwardPressed && !backwardsPressed) cameraVelocity.z = CAMERA_SPEED;
		if (!forwardPressed && backwardsPressed) cameraVelocity.z = -CAMERA_SPEED;

		if (leftPressed && !rightPressed) cameraVelocity.x = -CAMERA_SPEED;
		if (!leftPressed && rightPressed) cameraVelocity.x = CAMERA_SPEED;

		if (ascendPressed && !descendPressed) cameraVelocity.y = CAMERA_SPEED;
		if (!ascendPressed && descendPressed) cameraVelocity.y = -CAMERA_SPEED;

		// calculate view matrix
		Vector3 front = Vector3(cos(yaw) * cos(pitch), sin(pitch), sin(yaw) * cos(pitch));
		Vector3 right = front.cross(Vector3::up()).normalized();
		Vector3 up = right.cross(front).normalized();

		position += cameraVelocity.x * CAMERA_SPEED * elapsedSecs * right;
		position += cameraVelocity.y * CAMERA_SPEED * elapsedSecs * up;
		position += cameraVelocity.z * CAMERA_SPEED * elapsedSecs * front;

		viewMatrix = Matrix4(
			right.x, right.y, right.z, -right.dot(position),
			up.x, up.y, up.z, -up.dot(position),
			-front.x, -front.y, -front.z, front.dot(position),
			0, 0, 0, 1
		);

		// write to uniform buffer object
		glBindBuffer(GL_UNIFORM_BUFFER, uboId);
		{
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Matrix4), &viewMatrix);
			glBufferSubData(GL_UNIFORM_BUFFER, sizeof(Matrix4), sizeof(Matrix4), &projectionMatrix);
		}
	}
}
