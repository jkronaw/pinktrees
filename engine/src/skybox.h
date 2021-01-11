#pragma once

#include "camera.h"
#include "mesh.h"
#include "texture.h"

namespace engine {
	class Skybox
	{
	public:
		Skybox(const Camera* camera);
		~Skybox();

		void enableToneMapping();
		void disableToneMapping();

		TextureCubemap* getCubemap() const;
		void setCubemap(TextureCubemap* cubemap);

		void loadCubemapFromDiskSingleFiles(const std::string& directoryName);
		void loadCubemapFromDiskHDR(const std::string& filename);
		void draw() const;

		// vertex attributes
		static const GLuint POSITIONS = 0;
	private:
		Mesh* cube = nullptr;
		TextureInfo* textureInfo = nullptr;
	};
}

