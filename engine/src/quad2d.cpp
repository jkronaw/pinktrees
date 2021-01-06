#include "quad2d.h"

#include "vector.h"

namespace engine {

	const Vector3 QUAD_VERTICES[4] = {
			Vector3( 1.0f, -1.0f, 0.0f),
			Vector3( 1.0f,  1.0f, 0.0f),
			Vector3(-1.0f, -1.0f, 0.0f),
			Vector3(-1.0f,  1.0f, 0.0f)
	};

	Quad2D::Quad2D()
	{
		glGenVertexArrays(1, &vaoId);
		glBindVertexArray(vaoId);
		{
			glGenBuffers(1, &vboId);
			glBindBuffer(GL_ARRAY_BUFFER, vboId);
			{
				glBufferData(GL_ARRAY_BUFFER, sizeof(QUAD_VERTICES), QUAD_VERTICES, GL_STATIC_DRAW);
				glEnableVertexAttribArray(POSITIONS);
				glVertexAttribPointer(POSITIONS, 3, GL_FLOAT, GL_FALSE, 0, 0);
			}
		}

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	Quad2D::~Quad2D() {
		glDeleteBuffers(1, &vboId);
		glDeleteVertexArrays(1, &vaoId);
	}

	void Quad2D::draw() {
		glBindVertexArray(vaoId);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
	}
}