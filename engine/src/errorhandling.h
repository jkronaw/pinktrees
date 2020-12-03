#pragma once

#include <stdlib.h>
#include <string>
#include <GL/glew.h>

namespace engine
{
#define ERROR_CALLBACK
#ifdef  ERROR_CALLBACK
	static const std::string errorSource(GLenum source);
	static const std::string errorType(GLenum type);
	static const std::string errorSeverity(GLenum severity);
	static void error(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
	void setupErrorCallback();
#else
	static const std::string errorString(GLenum error);
	static bool isOpenGLError();
	static void checkOpenGLError(std::string error);
#endif
}

