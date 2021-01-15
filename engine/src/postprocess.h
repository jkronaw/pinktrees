#pragma once

#include <GL/glew.h>

namespace engine {

	class PostProcessBuffer {
	public:
		virtual void deleteBufferData() = 0;
		virtual void initialize(unsigned int windowWidth, unsigned int windowHeight) = 0;
	};

	class ShadedBuffer : public PostProcessBuffer {
	public:

		ShadedBuffer();
		~ShadedBuffer();

		GLuint fbo = 0;
		GLuint texture = 0;
		GLuint depthTexture = 0;

		void initialize(unsigned int windowWidth, unsigned int windowHeight);
		void deleteBufferData();
	};

	class BloomBuffer : public PostProcessBuffer {
	public:

		BloomBuffer();
		~BloomBuffer();

		GLuint fbo = 0;
		GLuint texture = 0;

		void initialize(unsigned int windowWidth, unsigned int windowHeight);
		void deleteBufferData();
	};

	class PingPongBuffer : public PostProcessBuffer {
	public:

		PingPongBuffer();
		~PingPongBuffer();

		GLuint fbo[2];
		GLuint texture[2];

		void initialize(unsigned int windowWidth, unsigned int windowHeight);
		void deleteBufferData();
	};
}


