#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace engine
{
	TextureInfo::TextureInfo(GLenum unit, const std::string& uniformName, Texture* texture, Sampler* sampler) : unit(unit), uniformName(uniformName), texture(texture), sampler(sampler) {}

	void TextureInfo::updateShader(ShaderProgram* program)
	{
		glActiveTexture(unit);
		texture->bind();
		program->setUniform(uniformName.c_str(), unit - GL_TEXTURE0);
		if (sampler)
		{
			sampler->bind(unit - GL_TEXTURE0);
		}
	}

	Texture::~Texture()
	{
		if (glIsTexture(id))
		{
			glDeleteTextures(1, &id);
		}
	}

	void Texture2D::bind()
	{
		glBindTexture(GL_TEXTURE_2D, id);
	}

	void Texture2D::unbind()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture2D::load(const std::string& filename)
	{
		int width, height, channels;
		unsigned char* data = stbi_load(filename.c_str(), &width, &height, &channels, 0);

		if (data == nullptr)
		{
			throw FileCouldNotBeOpenedException(filename.c_str());
		}

		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// the following two settings are used as fallback if no sampler is attached to the current unit
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		GLenum format;
		switch (channels)
		{
		case 1:
			format = GL_RED;
			break;
		case 2:
			format = GL_RG;
			break;
		case 3:
			format = GL_RGB;
			break;
		case 4:
			format = GL_RGBA;
			break;
		default:
			throw Exception("Unsupported number of channels in image file.");
		}

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);
		stbi_image_free(data);
	}

	Sampler::Sampler() { glGenSamplers(1, &id); }
	Sampler::~Sampler() { glDeleteSamplers(1, &id); }

	void Sampler::bind(GLuint unit) { glBindSampler(unit, id); }
	void Sampler::unbind(GLuint unit) { glBindSampler(unit, 0); }

	NearestSampler::NearestSampler()
	{
		glSamplerParameteri(id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glSamplerParameteri(id, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}

	LinearSampler::LinearSampler()
	{
		glSamplerParameteri(id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glSamplerParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}

	NearestMipmapNearestSampler::NearestMipmapNearestSampler()
	{
		glSamplerParameteri(id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glSamplerParameteri(id, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	}

	NearestMipmapLinearSampler::NearestMipmapLinearSampler()
	{
		glSamplerParameteri(id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glSamplerParameteri(id, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	}

	LinearMipmapNearestSampler::LinearMipmapNearestSampler()
	{
		glSamplerParameteri(id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glSamplerParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	}

	LinearMipmapLinearSampler::LinearMipmapLinearSampler()
	{
		glSamplerParameteri(id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glSamplerParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}

	LinearAnisotropicSampler::LinearAnisotropicSampler()
	{
		glSamplerParameteri(id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glSamplerParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glSamplerParameterf(id, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4.0f);
	}

	MaxAnisotropicSampler::MaxAnisotropicSampler()
	{
		glSamplerParameteri(id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glSamplerParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		GLfloat max;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max);
		glSamplerParameterf(id, GL_TEXTURE_MAX_ANISOTROPY_EXT, max);
	}
}