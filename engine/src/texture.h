#pragma once

#include <string>

#include "shader.h"

namespace engine {

	class Texture;
	class Texture2D;

	struct TextureInfo;

	class Sampler;

	class Texture
	{
	protected:
		GLuint id = -1;
	public:
		Texture() = default;
		~Texture();
		virtual void bind() = 0;
		virtual void unbind() = 0;
	};

	class Texture2D : public Texture
	{
	public:
		void bind() override;
		void unbind() override;
		void load(const std::string& filename);
	};

	struct TextureInfo
	{
		GLenum unit;
		GLuint index;
		std::string uniformName;
		Texture* texture = nullptr;
		Sampler* sampler = nullptr;
	
		TextureInfo(GLenum unit, GLuint index, const std::string& uniformName, Texture* texture, Sampler* sampler);
		void updateShader(ShaderProgram* program);
	};

	class Sampler
	{
	protected:
		GLuint id = -1;
	public:
		Sampler();
		~Sampler();
		virtual void create() = 0;
		virtual void bind(GLuint unit);
		virtual void unbind(GLuint unit);
	};

	class NearestSampler : public Sampler { public: void create() override; };
	class LinearSampler : public Sampler { public: void create() override; };
	class NearestMipmapNearestSampler : public Sampler { public: void create() override; };
	class NearestMipmapLinearSampler : public Sampler { public: void create() override; };
	class LinearMipmapNearestSampler : public Sampler { public: void create() override; };
	class LinearMipmapLinearSampler : public Sampler { public: void create() override; };
	class LinearAnisotropicSampler : public Sampler { public: void create() override; };
	class MaxAnisotropicSampler : public Sampler { public: void create() override; };
}

