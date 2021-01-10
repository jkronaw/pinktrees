#pragma once

#include "drawable.h"

#include "texture.h"

namespace engine {
	class Skybox : public IDrawable
	{
	public:
		Skybox();
		~Skybox();

		void setCubemap(TextureCubemap* cubemap);
		void loadCubemapFromDiskSingleFiles(const std::string& directoryName);
		void loadCubemapFromDiskHDR(const std::string& filename);
		void draw(ShaderProgram* program) const;

		// vertex shader layout
		static const GLuint POSITIONS = 0;
	private:
		GLuint vaoId = -1;
		GLuint vboId = -1;

		TextureInfo* textureInfo = nullptr;
	};
}

