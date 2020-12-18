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
			GB_NUMBER_OF_TEXTURES
		};

		GBuffer();
		~GBuffer();

		void initialize(unsigned int windowWidth, unsigned int windowHeight);

		void bindWrite();
		void bindRead();
		void bindReadDebug();
		void setBufferToRead(GB_TEX_TYPE texType);

	private:
		GLuint m_fbo;
		GLuint m_textures[GB_NUMBER_OF_TEXTURES];
		GLuint m_depthTexture;
	};
}