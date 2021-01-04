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

		enum GB_PP_TEX_TYPE
		{
			GB_SHADED,
			GB_BLOOM,
			GB_NUMBER_OF_PP_TEXTURES	// Not a texture type, but used to count textures
		};

		GBuffer();
		~GBuffer();

		void initialize(unsigned int windowWidth, unsigned int windowHeight);
		void deleteBufferData();
		void bind();
		void bindWritePostProcess();
		void bindReadGbufferTextures();
		void bindReadPostProcess();
		void bindRead();
		void setBufferToRead(GB_TEX_TYPE texType);

		void bindPingFirstIteration();
		void bindPing();
		void bindPong();

		void bindBloom();

	private:
		GLuint m_fbo;
		GLuint m_fboPostProcess;
		GLuint m_fboPingPong[2];
		GLuint m_textures[GB_NUMBER_OF_TEXTURES];
		GLuint m_postProcessTextures[GB_NUMBER_OF_PP_TEXTURES];
		GLuint m_depthTexture;
		GLuint m_pingPongTextures[2];
	};
}