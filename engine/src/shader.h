#pragma once

#include "matrix.h"
#include "exceptions.h"

#include <GL/glew.h>

namespace engine
{
	class ShaderProgram
	{
	public:
		ShaderProgram() = default;
		~ShaderProgram();

		void init(const char*, const char*);
		void bindAttribLocation(GLuint, const char*);
		void link();
		GLuint getUniformLocation(const char*);
		void setUniform(const char*, const Vector3&);
		void setUniform(const char*, const Vector4&);
		void setUniform(const char*, const Matrix3&);
		void setUniform(const char*, const Matrix4&);
		void setUniform(const char*, int);
		void setUniformBlockBinding(const char* name, GLuint uboBP);

		void use();
		void unuse();
	private:
		GLuint programId;
		GLuint vertexShaderId, fragmentShaderId;
	};

}

