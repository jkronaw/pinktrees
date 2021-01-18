#pragma once

#include <GL/glew.h>
#include <vector>
#include "vector.h"

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

	class ReflectionsBuffer : public PostProcessBuffer {
	public:

		ReflectionsBuffer();
		~ReflectionsBuffer();

		GLuint fbo = 0;
		GLuint texture = 0;

		void initialize(unsigned int windowWidth, unsigned int windowHeight);
		void deleteBufferData();
	};

	class ReflectionsBlendBuffer : public PostProcessBuffer {
	public:

		ReflectionsBlendBuffer();
		~ReflectionsBlendBuffer();

		GLuint fbo = 0;
		GLuint texture = 0;

		void initialize(unsigned int windowWidth, unsigned int windowHeight);
		void deleteBufferData();
	};

	class BlurBuffer : public PostProcessBuffer {
	public:

		BlurBuffer();
		~BlurBuffer();

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

	class SsaoBuffer : public PostProcessBuffer {
	public: 
		SsaoBuffer();
		~SsaoBuffer();

		GLuint fbo = 0;
		GLuint texture = 0;

		GLuint noiseTexture = 0;

		std::vector<Vector3> ssaoKernel;

		void initialize(unsigned int windowWidth, unsigned int windowHeight);
		void deleteBufferData();
		void generateSampleKernel();
		void generateNoiseTexture();
	};
}


