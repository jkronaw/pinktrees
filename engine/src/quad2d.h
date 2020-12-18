#pragma once

namespace engine {

	class Quad2D {
	private:
		unsigned int m_vao;
		unsigned int m_vbo[2];

		void destroy();

	public:
		Quad2D();
		~Quad2D();
		void setupQuad();
		void drawQuad();
	};
}