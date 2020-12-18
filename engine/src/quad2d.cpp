#include "quad2d.h"
#include "GL/glew.h"

namespace engine {

	GLfloat data[] = {
			1.0f, -1.0f,
			1.0f, 1.0f,
			-1.0f, -1.0f,
			-1.0f, 1.0f
	};

	Quad2D::Quad2D()
		: m_vao(-1)//, m_vbo(-1)
	{}

	Quad2D::~Quad2D() {
		destroy();
	}

	void Quad2D::setupQuad() {
		/*
		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);

		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL)
		*/
		typedef struct {
			GLfloat XYZ[3];
		} Vertex;

		const Vertex Vertices[] = {
			-1, -1, 0,
			-1,	 1, 0,
			 1,	 1, 0,
			 1, -1, 0 };

		const GLubyte Indices[] = { 0, 2, 1, 0, 3, 2 };

		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);
		{
			glGenBuffers(2, m_vbo);
			glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
			{
				glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
			}
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbo[1]);
			{
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
			}
		}
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		
	}

	void Quad2D::drawQuad() {
		glBindVertexArray(m_vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (GLvoid*)0);
		glBindVertexArray(0);
	}

	void Quad2D::destroy() {
		glDeleteBuffers(2, m_vbo);
		glDeleteVertexArrays(1, &m_vao);
	}

}