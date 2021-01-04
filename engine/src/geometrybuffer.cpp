#include "geometrybuffer.h"
#include <cstddef>
#include <iostream>

namespace engine {

	GBuffer::GBuffer() {
		fboGeo = 0;
		fboShaded = 0;
		fboBloom = 0;
		depthTexture = 0;
	}

	GBuffer::~GBuffer() {
		deleteBufferData();
	}

	void GBuffer::deleteBufferData() {
		if (fboGeo != 0)
			glDeleteFramebuffers(1, &fboGeo);
		if (texturesGeo[0] != 0)
			glDeleteTextures(GBuffer::GB_NUMBER_OF_TEXTURES, texturesGeo);
		if (depthTexture != 0) 
			glDeleteTextures(1, &depthTexture);
		if (fboShaded != 0)
			glDeleteFramebuffers(1, &fboShaded);
		if (textureShaded != 0)
			glDeleteTextures(1, &textureShaded);
		if (fboBloom != 0)
			glDeleteFramebuffers(1, &fboBloom);
		if (textureBloom != 0)
			glDeleteTextures(1, &textureBloom);
		if (fboPingPong != 0)
			glDeleteFramebuffers(2, fboPingPong);
		if (texturesPingPong[0] != 0)
			glDeleteTextures(2, texturesPingPong);
	}

	void GBuffer::initialize(unsigned int windowWidth, unsigned int windowHeight) {
		
		// Geometry FBO
		glGenFramebuffers(1, &fboGeo);
		glBindFramebuffer(GL_FRAMEBUFFER, fboGeo);
		float texturesSize = sizeof(texturesGeo) / sizeof(texturesGeo[0]);
		glGenTextures(GBuffer::GB_NUMBER_OF_TEXTURES, texturesGeo);
		glGenTextures(1, &depthTexture);

		for (unsigned int i = 0; i < GBuffer::GB_NUMBER_OF_TEXTURES; i++) {
			glBindTexture(GL_TEXTURE_2D, texturesGeo[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, windowWidth, windowHeight, 0, GL_RGBA, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, texturesGeo[i], 0);
		}

		glBindTexture(GL_TEXTURE_2D, depthTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, windowWidth, windowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);

		GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4 };
		float drawBuffersSize = sizeof(DrawBuffers) / sizeof(DrawBuffers[0]);
		glDrawBuffers(drawBuffersSize, DrawBuffers);
		//glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);


		// Shade FBO
		glGenFramebuffers(1, &fboShaded);
		glGenTextures(1, &textureShaded);
		glBindFramebuffer(GL_FRAMEBUFFER, fboShaded);
		glBindTexture(GL_TEXTURE_2D, textureShaded);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, windowWidth, windowHeight, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureShaded, 0);
		glGenTextures(1, &shadedDepthTexture);
		glBindTexture(GL_TEXTURE_2D, shadedDepthTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, windowWidth, windowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadedDepthTexture, 0);
		GLenum DrawBuffersShade[] = { GL_COLOR_ATTACHMENT0};
		glDrawBuffers(1, DrawBuffersShade);
		//glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

		// Bloom FBO
		glGenFramebuffers(1, &fboBloom);
		glGenTextures(1, &textureBloom);
		glBindFramebuffer(GL_FRAMEBUFFER, fboBloom);
		glBindTexture(GL_TEXTURE_2D, textureBloom);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, windowWidth, windowHeight, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureBloom, 0);
		GLenum DrawBuffersBloom[] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, DrawBuffersBloom);
		//glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

		// Ping Pong FBO
		glGenFramebuffers(2, fboPingPong);
		glGenTextures(2, texturesPingPong);
		for (unsigned int i = 0; i < 2; i++)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, fboPingPong[i]);
			glBindTexture(GL_TEXTURE_2D, texturesPingPong[i]);
			glTexImage2D(
				GL_TEXTURE_2D, 0, GL_RGBA16F, windowWidth, windowHeight, 0, GL_RGBA, GL_FLOAT, NULL
			);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glFramebufferTexture2D(
				GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texturesPingPong[i], 0
			);
		}
	}

	void GBuffer::setBufferToRead(GB_TEX_TYPE texType) {
		glReadBuffer(GL_COLOR_ATTACHMENT0 + texType);
	}
}