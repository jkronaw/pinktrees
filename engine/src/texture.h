#pragma once

#include <string>

#include "shader.h"

namespace engine
{

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
		std::string uniformName;
		Texture* texture = nullptr;
		Sampler* sampler = nullptr;

		TextureInfo(GLenum unit, const std::string& uniformName, Texture* texture, Sampler* sampler);
		void updateShader(ShaderProgram* program);
	};

	class Sampler
	{
	protected:
		GLuint id = -1;
		Sampler();
	public:
		~Sampler();
		virtual void bind(GLuint unit);
		virtual void unbind(GLuint unit);
	};

	class NearestSampler : public Sampler { public: NearestSampler(); };
	class LinearSampler : public Sampler { public: LinearSampler(); };
	class NearestMipmapNearestSampler : public Sampler { public: NearestMipmapNearestSampler(); };
	class NearestMipmapLinearSampler : public Sampler { public: NearestMipmapLinearSampler(); };
	class LinearMipmapNearestSampler : public Sampler { public:	LinearMipmapNearestSampler(); };
	class LinearMipmapLinearSampler : public Sampler { public: LinearMipmapLinearSampler(); };
	class LinearAnisotropicSampler : public Sampler { public: LinearAnisotropicSampler(); };
	class MaxAnisotropicSampler : public Sampler { public: MaxAnisotropicSampler(); };
}

