#pragma once

#include <string>

#include "shader.h"

namespace engine
{
	class Texture;
	class Texture2D;
	class TextureCubemap;
	struct TextureInfo;
	class Sampler;

	class Texture
	{
	protected:
		GLuint id = 0;
	public:
		Texture();
		~Texture();
		virtual void bind() const = 0;
		virtual void unbind() const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		void bind() const override;
		void unbind() const override;
		void loadFromDisk(const std::string& filename) const;
		void loadFromDiskHDR(const std::string& filename) const;
		void createFromColorGrayscale(float color) const;
		void createFromColorRGB(const Vector3& color) const;
		void createBRDFLookupTexture() const;
	};

	class TextureCubemap : public Texture
	{
	public:
		void bind() const override;
		void unbind() const override;
		void loadFromDiskSingleFiles(const std::string& directoryName) const;
		void loadFromDiskHDR(const std::string& filename) const;
		void convoluteIrradianceMapFromCubemap(TextureCubemap* cubemap) const;
		void convolutePrefilterMapFromCubemap(TextureCubemap* cubemap) const;
	private:
		void setupEmpty(int cubeSidelength, bool useMipmaps = false) const;
	};

	struct TextureInfo
	{
		GLenum unit;
		std::string uniformName;
		Texture* texture = nullptr;
		Sampler* sampler = nullptr;

		TextureInfo(GLenum unit, const std::string& uniformName, Texture* texture, Sampler* sampler);
		void updateShader(ShaderProgram* program) const;
		void unbindSampler() const;
	};

	class Sampler
	{
	protected:
		GLuint id = 0;
		Sampler();
	public:
		~Sampler();
		virtual void bind(GLuint unit) const;
		virtual void unbind(GLuint unit) const;
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

