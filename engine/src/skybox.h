#pragma once

#include "camera.h"
#include "texture.h"

namespace engine {
	class Skybox
	{
	public:
		Skybox(const Camera* camera);
		~Skybox();

		void enableToneMapping();
		void disableToneMapping();

		void setCubemap(TextureCubemap* cubemap);
		void loadCubemapFromDiskSingleFiles(const std::string& directoryName);
		void loadCubemapFromDiskHDR(const std::string& filename);
		void draw() const;

		// vertex attributes
		static const GLuint POSITIONS = 0;
	private:
		GLuint vaoId = -1;
		GLuint vboId = -1;

		TextureInfo* textureInfo = nullptr;
	};
}

