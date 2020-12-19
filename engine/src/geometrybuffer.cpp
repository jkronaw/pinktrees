#include "geometrybuffer.h"
#include <cstddef>
#include <iostream>

namespace engine {

	GBuffer::GBuffer() {
		m_fbo = 0;
		m_fboPostProcess = 0;
		m_depthTexture = 0;
	}

	GBuffer::~GBuffer() {
		if (m_fbo != 0)
			glDeleteFramebuffers(1, &m_fbo);
		if (m_textures[0] != 0)
			glDeleteTextures(GBuffer::GB_NUMBER_OF_TEXTURES, m_textures);
		if (m_depthTexture != 0) {
			glDeleteTextures(1, &m_depthTexture);
		}
		if (m_fboPostProcess != 0)
			glDeleteFramebuffers(1, &m_fboPostProcess);
		if (m_postProcessTextures[0] != 0)
			glDeleteTextures(GBuffer::GB_NUMBER_OF_PP_TEXTURES, m_postProcessTextures);
		if (m_fboPingPong != 0)
			glDeleteFramebuffers(2, m_fboPingPong);
		if (m_pingPongTextures[0] != 0)
			glDeleteTextures(2, m_pingPongTextures);
	}

	void GBuffer::initialize(unsigned int windowWidth, unsigned int windowHeight) {
		
		// Geometry FBO

		glGenFramebuffers(1, &m_fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
		float texturesSize = sizeof(m_textures) / sizeof(m_textures[0]);
		glGenTextures(GBuffer::GB_NUMBER_OF_TEXTURES, m_textures);
		glGenTextures(1, &m_depthTexture);

		for (unsigned int i = 0; i < GBuffer::GB_NUMBER_OF_TEXTURES; i++) {
			glBindTexture(GL_TEXTURE_2D, m_textures[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, windowWidth, windowHeight, 0, GL_RGBA, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_textures[i], 0);
		}

		glBindTexture(GL_TEXTURE_2D, m_depthTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, windowWidth, windowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture, 0);

		GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4 };
		float drawBuffersSize = sizeof(DrawBuffers) / sizeof(DrawBuffers[0]);
		glDrawBuffers(drawBuffersSize, DrawBuffers);

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

		// Shade and Postprocess FBO
		glGenFramebuffers(1, &m_fboPostProcess);
		glGenTextures(GBuffer::GB_NUMBER_OF_PP_TEXTURES, m_postProcessTextures);
		glBindFramebuffer(GL_FRAMEBUFFER, m_fboPostProcess);
		for (unsigned int i = 0; i < GBuffer::GB_NUMBER_OF_PP_TEXTURES; i++) {
			glBindTexture(GL_TEXTURE_2D, m_postProcessTextures[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, windowWidth, windowHeight, 0, GL_RGBA, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_postProcessTextures[i], 0);
		}
		GLenum DrawBuffersPostProcess[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
		float drawBuffersPostProcessSize = sizeof(DrawBuffersPostProcess) / sizeof(DrawBuffersPostProcess[0]);
		glDrawBuffers(drawBuffersPostProcessSize, DrawBuffersPostProcess);

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		
		// Bloom Ping Pong FBO
		glGenFramebuffers(2, m_fboPingPong);
		glGenTextures(2, m_pingPongTextures);
		for (unsigned int i = 0; i < 2; i++)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, m_fboPingPong[i]);
			glBindTexture(GL_TEXTURE_2D, m_pingPongTextures[i]);
			glTexImage2D(
				GL_TEXTURE_2D, 0, GL_RGBA16F, windowWidth, windowHeight, 0, GL_RGBA, GL_FLOAT, NULL
			);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glFramebufferTexture2D(
				GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_pingPongTextures[i], 0
			);
		}
	}

	void GBuffer::bindWrite() {
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	}

	void GBuffer::bindWritePostProcess() {
		bindRead();
		glBindFramebuffer(GL_FRAMEBUFFER, m_fboPostProcess);
	}

	void GBuffer::bindReadDebug() {
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);
	}

	void GBuffer::bindRead() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		for (unsigned int i = 0; i < GBuffer::GB_NUMBER_OF_TEXTURES; i++) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, m_textures[GB_POSITION + i]);
		}
	}

	void GBuffer::bindReadPostProcess() {
		bindRead();
		for (unsigned int i = 0; i < GBuffer::GB_NUMBER_OF_PP_TEXTURES; i++) {
			glActiveTexture(GL_TEXTURE0 + GBuffer::GB_NUMBER_OF_TEXTURES + i);
			glBindTexture(GL_TEXTURE_2D, m_postProcessTextures[GB_SHADED + i]);
		}
	}

	void GBuffer::setBufferToRead(GB_TEX_TYPE texType) {
		glReadBuffer(GL_COLOR_ATTACHMENT0 + texType);

	}

	void GBuffer::bindPingFirstIteration() {
		glBindFramebuffer(GL_FRAMEBUFFER, m_fboPingPong[0]);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_postProcessTextures[GB_BLOOM]);
	}

	void GBuffer::bindPing() {
		glBindFramebuffer(GL_FRAMEBUFFER, m_fboPingPong[0]);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_pingPongTextures[1]);
		
	}

	void GBuffer::bindPong() {
		glBindFramebuffer(GL_FRAMEBUFFER, m_fboPingPong[1]);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_pingPongTextures[0]);

	}

	void GBuffer::bindBloom() {
		glBindFramebuffer(GL_FRAMEBUFFER, m_fboPostProcess);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_postProcessTextures[GB_SHADED]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_pingPongTextures[1]);
	}
}