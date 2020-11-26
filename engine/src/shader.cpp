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
		std::string vertexShaderSource = readStringFromFile(vertexShaderFilename);
		std::string fragmentShaderSource = readStringFromFile(fragmentShaderFilename);

		const char* vertexShaderSourceC = vertexShaderSource.c_str();
		const char* fragmentShaderSourceC = fragmentShaderSource.c_str();

		vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShaderId, 1, &vertexShaderSourceC, 0);
		glCompileShader(vertexShaderId);

		fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShaderId, 1, &fragmentShaderSourceC, 0);
		glCompileShader(fragmentShaderId);

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
		glLinkProgram(programId);
		glDetachShader(programId, vertexShaderId);
		glDeleteShader(vertexShaderId);
		glDetachShader(programId, fragmentShaderId);
		glDeleteShader(fragmentShaderId);
	}

	GLuint ShaderProgram::getUniformLocation(const char* name)
	{
		return glGetUniformLocation(programId, name);
	}

	void ShaderProgram::setUniform(const char* name, const Vector3& vector)
	{
		GLuint id = glGetUniformLocation(programId, name);
		glUniform3fv(id, 1, (GLfloat*)&vector);
	}

	void ShaderProgram::setUniform(const char* name, const Vector4& vector)
	{
		GLuint id = glGetUniformLocation(programId, name);
		glUniform4fv(id, 1, (GLfloat*)&vector);
	}

	void ShaderProgram::setUniform(const char* name, const Matrix4& matrix)
	{
		GLuint id = glGetUniformLocation(programId, name);
		float tmp[16];
		matrix.convertToOpenGL(tmp);
		glUniformMatrix4fv(id, 1, GL_FALSE, tmp);
	}

	GLuint ShaderProgram::getUniformBlockBinding(const char* name, GLuint bp)
	{
		GLuint uboId = glGetUniformBlockIndex(programId, "SharedMatrices");
		glUniformBlockBinding(programId, uboId, bp);
		return uboId;
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
