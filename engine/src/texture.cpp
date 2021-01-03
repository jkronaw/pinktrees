#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace engine
{
	Texture::Texture() { glGenTextures(1, &id); }
	Texture::~Texture() { glDeleteTextures(1, &id); }

	void Texture2D::bind() { glBindTexture(GL_TEXTURE_2D, id); }
	void Texture2D::unbind() { glBindTexture(GL_TEXTURE_2D, 0); }

	unsigned char* loadImage(const std::string filename, int* width, int* height, GLenum* format)
	{
		int channels;
		unsigned char* data = stbi_load(filename.c_str(), width, height, &channels, 0);

		if (data == nullptr)
		{
			throw FileCouldNotBeOpenedException(filename.c_str());
		}

		switch (channels)
		{
		case 1:
			*format = GL_RED;
			break;
		case 2:
			*format = GL_RG;
			break;
		case 3:
			*format = GL_RGB;
			break;
		case 4:
			*format = GL_RGBA;
			break;
		default:
			stbi_image_free(data);
			throw Exception("Unsupported number of channels in image file.");
		}

		return data;
	}

	void Texture2D::load(const std::string& filename)
	{
		int width, height;
		GLenum format;
		unsigned char* data = loadImage(filename, &width, &height, &format);

		glBindTexture(GL_TEXTURE_2D, id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// the following two settings are used as fallback if no sampler is attached to the current unit
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);
		stbi_image_free(data);
	}

	void Texture2D::createFromColorGrayscale(float color)
	{
		glBindTexture(GL_TEXTURE_2D, id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		// load a 1x1 texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 1, 1, 0, GL_RED, GL_FLOAT, &color);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture2D::createFromColorRGB(Vector3 color)
	{
		glBindTexture(GL_TEXTURE_2D, id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		// load a 1x1 texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_FLOAT, &color);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void TextureCubeMap::bind() { glBindTexture(GL_TEXTURE_CUBE_MAP, id); }
	void TextureCubeMap::unbind() { glBindTexture(GL_TEXTURE_CUBE_MAP, id); }

	const GLenum CUBEMAP_TEXTURES[6] = {
		GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	};

	const std::string CUBEMAP_FILENAMES[6] = {
		"px", "nx",
		"py", "ny",
		"pz", "nz"
	};

	void TextureCubeMap::load(const std::string& directoryName)
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, id);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		int width, height, channels;
		unsigned char* data;

		for (unsigned int i = 0; i < 6; i++)
		{
			std::string filename = directoryName + "/" + CUBEMAP_FILENAMES[i] + ".png";
			
			int width, height;
			GLenum format;
			unsigned char* data = loadImage(filename, &width, &height, &format);

			glTexImage2D(CUBEMAP_TEXTURES[i], 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

			stbi_image_free(data);
		}

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}

	TextureInfo::TextureInfo(GLenum unit, const std::string& uniformName, Texture* texture, Sampler* sampler) : unit(unit), uniformName(uniformName), texture(texture), sampler(sampler) {}

	void TextureInfo::updateShader(ShaderProgram* program)
	{
		glActiveTexture(unit);
		texture->bind();
		program->setUniform(uniformName.c_str(), int(unit - GL_TEXTURE0));
		if (sampler)
		{
			sampler->bind(unit - GL_TEXTURE0);
		}
	}

	void TextureInfo::unbindSampler() {
		if (sampler)
		{
			sampler->unbind(unit - GL_TEXTURE0);
		}
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