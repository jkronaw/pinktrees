#pragma once

#include <GL/glew.h>

namespace engine {

	class GBuffer 
	{
	public:
		enum GB_TEX_TYPE 
		{
			GB_POSITION,
			GB_ALBEDO,
			GB_NORMAL,
			GB_METALLIC_ROUGHNESS_AO,
			GB_TEXCOORD,
			GB_NUMBER_OF_TEXTURES		// Not a texture type, but used to count textures 
		};

		GBuffer();
		~GBuffer();

		void initialize(unsigned int windowWidth, unsigned int windowHeight);
		void deleteBufferData();

		void setBufferToRead(GB_TEX_TYPE texType);

		GLuint fboGeo;
		GLuint fboShaded;
		GLuint fboBloom;
		GLuint fboPingPong[2];
		GLuint texturesGeo[GB_NUMBER_OF_TEXTURES];
		GLuint textureShaded;
		GLuint textureBloom;
		GLuint depthTexture;
		GLuint shadedDepthTexture;
		GLuint texturesPingPong[2];
	};
}