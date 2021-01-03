#pragma once

#include "drawable.h"

#include "texture.h"

namespace engine {
	class Skybox : public IDrawable
	{
	public:
		Skybox();
		~Skybox();

		void load(const std::string& directoryName);
		void draw(ShaderProgram* program) const;

		// vertex shader layout
		static const GLuint POSITIONS = 0;
	private:
		GLuint vaoId = -1;
		TextureInfo* textureInfo = nullptr;
	};
}

