#include "geometrybuffer.h"
#include <cstddef>
#include <iostream>

namespace engine {

	GBuffer::GBuffer() {
		m_fbo = 0;
		m_depthTexture = 0;
	}

	GBuffer::~GBuffer() {
		if (m_fbo != 0)
			glDeleteFramebuffers(1, &m_fbo);
		if (m_textures[0] != 0)
			glDeleteTextures(sizeof(m_textures) / sizeof(m_textures[0]), m_textures);
		if (m_depthTexture != 0) {
			glDeleteTextures(1, &m_depthTexture);
		}
	}

	void GBuffer::initialize(unsigned int windowWidth, unsigned int windowHeight) {
		glGenFramebuffers(1, &m_fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
		float texturesSize = sizeof(m_textures) / sizeof(m_textures[0]);
		glGenTextures(texturesSize, m_textures);
		glGenTextures(1, &m_depthTexture);

		for (unsigned int i = 0; i < texturesSize; i++) {
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
	}

	void GBuffer::bindWrite() {
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	}

	void GBuffer::bindReadDebug() {
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);
	}
	void GBuffer::bindRead() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		for (unsigned int i = 0; i < sizeof(m_textures) / sizeof(m_textures[0]); i++) {
			std::cout << GL_TEXTURE0 + i << std::endl;
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, m_textures[GB_POSITION + i]);
		}
	}

	void GBuffer::setBufferToRead(GB_TEX_TYPE texType) {
		glReadBuffer(GL_COLOR_ATTACHMENT0 + texType);
	}

}