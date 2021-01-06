#pragma once

#include "GL/glew.h"

namespace engine {
	class Quad2D {

	public:
		Quad2D();
		~Quad2D();

		void draw();

		// vertex shader layout
		static const GLuint POSITIONS = 0;
	private:
		GLuint vaoId = -1;
		GLuint vboId = -1;
	};
}