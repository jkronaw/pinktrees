#include "shader.h"

#include <fstream>

#include <stdexcept>

namespace engine
{
	std::string readStringFromFile(const char* filename)
	{
		std::ifstream in(filename);
		if (in.fail())
		{
			throw FileCouldNotBeOpenedException(filename);
		}
		std::string content((std::istreambuf_iterator<char>(in)), (std::istreambuf_iterator<char>()));
		return content;
	}

	void ShaderProgram::init(const char* vertexShaderFilename, const char* fragmentShaderFilename)
	{
		int  success;
		char infoLog[512];

		std::string vertexShaderSource = readStringFromFile(vertexShaderFilename);
		std::string fragmentShaderSource = readStringFromFile(fragmentShaderFilename);

		const char* vertexShaderSourceC = vertexShaderSource.c_str();
		const char* fragmentShaderSourceC = fragmentShaderSource.c_str();

		vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShaderId, 1, &vertexShaderSourceC, 0);
		glCompileShader(vertexShaderId);

		glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexShaderId, 512, NULL, infoLog);
			throw ShaderCompilationException(infoLog);
		}

		fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShaderId, 1, &fragmentShaderSourceC, 0);
		glCompileShader(fragmentShaderId);

		glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShaderId, 512, NULL, infoLog);
			throw ShaderCompilationException(infoLog);
		}

		programId = glCreateProgram();
		glAttachShader(programId, vertexShaderId);
		glAttachShader(programId, fragmentShaderId);
	}

	void ShaderProgram::bindAttribLocation(GLuint id, const char* str)
	{
		glBindAttribLocation(programId, id, str);
	}

	void ShaderProgram::link()
	{
		int  success;
		char infoLog[512];

		glLinkProgram(programId);

		glDetachShader(programId, vertexShaderId);
		glDeleteShader(vertexShaderId);
		glDetachShader(programId, fragmentShaderId);
		glDeleteShader(fragmentShaderId);

		glGetProgramiv(programId, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(programId, 512, NULL, infoLog);
			throw ShaderProgramLinkageException(infoLog);
		}
	}

	GLuint ShaderProgram::getUniformLocation(const char* name)
	{
		return glGetUniformLocation(programId, name);
	}

	void ShaderProgram::setUniform(const char* name, const Vector2& vector)
	{
		GLuint location = glGetUniformLocation(programId, name);
		glUniform2fv(location, 1, (GLfloat*)&vector);
	}

	void ShaderProgram::setUniform(const char* name, const Vector3& vector)
	{
		GLuint location = glGetUniformLocation(programId, name);
		glUniform3fv(location, 1, (GLfloat*)&vector);
	}

	void ShaderProgram::setUniform(const char* name, const std::vector<Vector3> vectors)
	{
		GLuint location = glGetUniformLocation(programId, name);
		glUniform3fv(location, (GLsizei)vectors.size(), (GLfloat*)&vectors[0]);
	}

	void ShaderProgram::setUniform(const char* name, const Vector4& vector)
	{
		GLuint location = glGetUniformLocation(programId, name);
		glUniform4fv(location, 1, (GLfloat*)&vector);
	}

	void ShaderProgram::setUniform(const char* name, const Matrix3& matrix)
	{
		GLuint location = glGetUniformLocation(programId, name);
		glUniformMatrix3fv(location, 1, GL_FALSE, (GLfloat*)&matrix);
	}

	void ShaderProgram::setUniform(const char* name, const Matrix4& matrix)
	{
		GLuint location = glGetUniformLocation(programId, name);
		glUniformMatrix4fv(location, 1, GL_FALSE, (GLfloat*)&matrix);
	}

	void ShaderProgram::setUniform(const char* name, int value)
	{
		GLuint location = glGetUniformLocation(programId, name);
		glUniform1i(location, value);
	}

	void ShaderProgram::setUniform(const char* name, float value)
	{
		GLuint location = glGetUniformLocation(programId, name);
		glUniform1f(location, value);
	}

	void ShaderProgram::setUniformBlockBinding(const char* name, GLuint uboBP)
	{
		GLuint uboId = glGetUniformBlockIndex(programId, name);
		glUniformBlockBinding(programId, uboId, uboBP);
	}

	void ShaderProgram::use()
	{
		glUseProgram(programId);
	}

	void ShaderProgram::unuse()
	{
		glUseProgram(0);
	}

	ShaderProgram::~ShaderProgram()
	{
		unuse();
		glDeleteProgram(programId);
	}
}
