#include "texture.h"

#define _USE_MATH_DEFINES
#include <math.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "meshfactory.h"

namespace engine
{
	/* Texture */
	Texture::Texture() { glGenTextures(1, &id); }
	Texture::~Texture() { glDeleteTextures(1, &id); }

	/* Texture2D */
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

	void Texture2D::bind() const { glBindTexture(GL_TEXTURE_2D, id); }
	void Texture2D::unbind() const { glBindTexture(GL_TEXTURE_2D, 0); }
	void Texture2D::loadFromDisk(const std::string& filename) const
	{
		int width, height;
		GLenum format;
		unsigned char* data = loadImage(filename, &width, &height, &format);

		glBindTexture(GL_TEXTURE_2D, id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// the following two settings are used as fallback if no sampler is attached to the current unit
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);
		stbi_image_free(data);
	}
	void Texture2D::loadFromDiskHDR(const std::string& filename) const
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(true);
		float* data = stbi_loadf(filename.c_str(), &width, &height, &channels, 0);

		if (data == nullptr)
		{
			throw FileCouldNotBeOpenedException(filename.c_str());
		}

		glBindTexture(GL_TEXTURE_2D, id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);

		glBindTexture(GL_TEXTURE_2D, 0);
		stbi_image_free(data);
	}
	void Texture2D::createFromColorGrayscale(float color) const
	{
		bind();

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		// load a 1x1 texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 1, 1, 0, GL_RED, GL_FLOAT, &color);

		unbind();
	}
	void Texture2D::createFromColorRGB(const Vector3& color) const
	{
		bind();

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		// load a 1x1 texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_FLOAT, &color);

		unbind();
	}
	void Texture2D::createBRDFLookupTexture() const
	{
		const unsigned int INTERNAL_SIZE = 512;

		bind();

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, INTERNAL_SIZE, INTERNAL_SIZE, 0, GL_RG, GL_FLOAT, 0);

		unbind();

		unsigned int captureFbo, captureRbo;
		glGenFramebuffers(1, &captureFbo);
		glGenRenderbuffers(1, &captureRbo);

		glBindFramebuffer(GL_FRAMEBUFFER, captureFbo);
		glBindRenderbuffer(GL_RENDERBUFFER, captureRbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, INTERNAL_SIZE, INTERNAL_SIZE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, id, 0);

		ShaderProgram* program = new ShaderProgram();
		program->init("shaders/general/quad2D.vert", "shaders/preprocessing/brdfLUT.frag");
		program->link();

		program->use();

		int oldViewport[4];
		glGetIntegerv(GL_VIEWPORT, oldViewport);
		glViewport(0, 0, INTERNAL_SIZE, INTERNAL_SIZE);

		Mesh* quad = MeshFactory::createQuad();

		glBindFramebuffer(GL_FRAMEBUFFER, captureFbo);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		quad->draw();
		program->unuse();

		glViewport(0, 0, oldViewport[2], oldViewport[3]);

		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDeleteFramebuffers(1, &captureFbo);
		glDeleteRenderbuffers(1, &captureRbo);

		delete quad;
		delete program;
	}

	/* TextureCubemap */
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
	const Matrix4 CUBEMAP_CAPTURE_VIEW_MATRICES[] =
	{
	   Matrix4::CreateLookAt(Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f,  0.0f,  0.0f), Vector3(0.0f, -1.0f,  0.0f)),
	   Matrix4::CreateLookAt(Vector3(0.0f, 0.0f, 0.0f), Vector3(-1.0f, 0.0f,  0.0f), Vector3(0.0f, -1.0f,  0.0f)),
	   Matrix4::CreateLookAt(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f,  1.0f,  0.0f), Vector3(0.0f,  0.0f,  1.0f)),
	   Matrix4::CreateLookAt(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, -1.0f,  0.0f), Vector3(0.0f,  0.0f, -1.0f)),
	   Matrix4::CreateLookAt(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f,  0.0f,  1.0f), Vector3(0.0f, -1.0f,  0.0f)),
	   Matrix4::CreateLookAt(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f,  0.0f, -1.0f), Vector3(0.0f, -1.0f,  0.0f))
	};

	void TextureCubemap::bind() const { glBindTexture(GL_TEXTURE_CUBE_MAP, id); }
	void TextureCubemap::unbind() const { glBindTexture(GL_TEXTURE_CUBE_MAP, id); }
	void TextureCubemap::loadFromDiskSingleFiles(const std::string& directoryName) const
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, id);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

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
	void TextureCubemap::loadFromDiskHDR(const std::string& filename) const
	{
		const unsigned int INTERNAL_CUBEMAP_SIDELENGTH = 1024;

		Texture2D* hdr = new Texture2D();
		hdr->loadFromDiskHDR(filename);
		TextureInfo* hdrInfo = new TextureInfo(GL_TEXTURE0, "equirectangularMap", hdr, nullptr);

		setupEmpty(INTERNAL_CUBEMAP_SIDELENGTH);

		unsigned int captureFbo, captureRbo;
		glGenFramebuffers(1, &captureFbo);
		glGenRenderbuffers(1, &captureRbo);

		glBindFramebuffer(GL_FRAMEBUFFER, captureFbo);
		glBindRenderbuffer(GL_RENDERBUFFER, captureRbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, INTERNAL_CUBEMAP_SIDELENGTH, INTERNAL_CUBEMAP_SIDELENGTH);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRbo);

		ShaderProgram* program = new ShaderProgram();
		program->init("shaders/preprocessing/cubemap.vert", "shaders/preprocessing/equirectToCubemap.frag");
		program->link();

		program->use();

		hdrInfo->updateShader(program);

		Matrix4 projectionMatrix = Matrix4::CreatePerspectiveProjection(M_PI / 2, 1.0f, 0.1f, 10.0f);
		program->setUniform("ProjectionMatrix", projectionMatrix);

		// change global OpenGL settings
		int oldViewport[4];
		glGetIntegerv(GL_VIEWPORT, oldViewport);
		glViewport(0, 0, INTERNAL_CUBEMAP_SIDELENGTH, INTERNAL_CUBEMAP_SIDELENGTH);
		glCullFace(GL_FRONT);

		Mesh* cube = MeshFactory::createCube();

		glBindFramebuffer(GL_FRAMEBUFFER, captureFbo);
		for (unsigned int i = 0; i < 6; ++i)
		{
			program->setUniform("ViewMatrix", CUBEMAP_CAPTURE_VIEW_MATRICES[i]);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, id, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			cube->draw();
		}
		
		program->unuse();

		// restore global OpenGL settings
		glViewport(0, 0, oldViewport[2], oldViewport[3]);
		glCullFace(GL_BACK);

		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDeleteFramebuffers(1, &captureFbo);
		glDeleteRenderbuffers(1, &captureRbo);

		delete cube;
		delete hdr;
		delete hdrInfo;
		delete program;
	}
	void TextureCubemap::convoluteIrradianceMapFromCubemap(TextureCubemap* cubemap) const
	{
		const unsigned int INTERNAL_CUBEMAP_SIDELENGTH = 128;

		setupEmpty(INTERNAL_CUBEMAP_SIDELENGTH);

		unsigned int captureFbo, captureRbo;
		glGenFramebuffers(1, &captureFbo);
		glGenRenderbuffers(1, &captureRbo);

		glBindFramebuffer(GL_FRAMEBUFFER, captureFbo);
		glBindRenderbuffer(GL_RENDERBUFFER, captureRbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, INTERNAL_CUBEMAP_SIDELENGTH, INTERNAL_CUBEMAP_SIDELENGTH);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRbo);

		ShaderProgram* program = new ShaderProgram();
		program->init("shaders/preprocessing/cubemap.vert", "shaders/preprocessing/irradianceMapConvolution.frag");
		program->link();

		program->use();

		TextureInfo* cubemapInfo = new TextureInfo(GL_TEXTURE0, "cubemap", cubemap, nullptr);
		cubemapInfo->updateShader(program);

		Matrix4 projectionMatrix = Matrix4::CreatePerspectiveProjection(M_PI / 2, 1.0f, 0.1f, 10.0f);
		program->setUniform("ProjectionMatrix", projectionMatrix);

		// change global OpenGL settings
		int oldViewport[4];
		glGetIntegerv(GL_VIEWPORT, oldViewport);
		glViewport(0, 0, INTERNAL_CUBEMAP_SIDELENGTH, INTERNAL_CUBEMAP_SIDELENGTH);
		glCullFace(GL_FRONT);

		Mesh* cube = MeshFactory::createCube();

		glBindFramebuffer(GL_FRAMEBUFFER, captureFbo);
		for (unsigned int i = 0; i < 6; ++i)
		{
			program->setUniform("ViewMatrix", CUBEMAP_CAPTURE_VIEW_MATRICES[i]);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, id, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			cube->draw();
		}

		program->unuse();

		// restore global OpenGL settings
		glViewport(0, 0, oldViewport[2], oldViewport[3]);
		glCullFace(GL_BACK);

		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDeleteFramebuffers(1, &captureFbo);
		glDeleteRenderbuffers(1, &captureRbo);

		delete cube;
		delete cubemapInfo;
		delete program;
	}
	void TextureCubemap::convolutePrefilterMapFromCubemap(TextureCubemap* cubemap) const
	{
		const unsigned int INTERNAL_CUBEMAP_SIDELENGTH = 128;
		const unsigned int NR_MIPMAP_LEVELS = 5;

		setupEmpty(INTERNAL_CUBEMAP_SIDELENGTH, true);

		unsigned int captureFbo, captureRbo;
		glGenFramebuffers(1, &captureFbo);
		glGenRenderbuffers(1, &captureRbo);

		glBindFramebuffer(GL_FRAMEBUFFER, captureFbo);
		glBindRenderbuffer(GL_RENDERBUFFER, captureRbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, INTERNAL_CUBEMAP_SIDELENGTH, INTERNAL_CUBEMAP_SIDELENGTH);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRbo);

		ShaderProgram* program = new ShaderProgram();
		program->init("shaders/preprocessing/cubemap.vert", "shaders/preprocessing/prefilterMapConvolution.frag");
		program->link();

		program->use();

		TextureInfo* cubemapInfo = new TextureInfo(GL_TEXTURE0, "cubemap", cubemap, nullptr);
		cubemapInfo->updateShader(program);

		Matrix4 projectionMatrix = Matrix4::CreatePerspectiveProjection(M_PI / 2, 1.0f, 0.1f, 10.0f);
		program->setUniform("ProjectionMatrix", projectionMatrix);

		// change global OpenGL settings
		int oldViewport[4];
		glGetIntegerv(GL_VIEWPORT, oldViewport);
		glCullFace(GL_FRONT);

		Mesh* cube = MeshFactory::createCube();

		glBindFramebuffer(GL_FRAMEBUFFER, captureFbo);

		for (unsigned int mipmapLevel = 0; mipmapLevel < NR_MIPMAP_LEVELS; mipmapLevel++)
		{
			GLuint mipmapWidth  = (GLuint)(INTERNAL_CUBEMAP_SIDELENGTH * std::pow(0.5, mipmapLevel));
			GLuint mipmapHeight = (GLuint)(INTERNAL_CUBEMAP_SIDELENGTH * std::pow(0.5, mipmapLevel));

			glBindRenderbuffer(GL_RENDERBUFFER, captureRbo);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipmapWidth, mipmapHeight);
			glViewport(0, 0, mipmapWidth, mipmapHeight);

			float roughness = mipmapLevel / (float)(NR_MIPMAP_LEVELS - 1);
			program->setUniform("roughness", roughness);

			for (unsigned int i = 0; i < 6; ++i)
			{
				program->setUniform("ViewMatrix", CUBEMAP_CAPTURE_VIEW_MATRICES[i]);

				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, id, mipmapLevel);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				cube->draw();
			}
		}

		program->unuse();

		// restore global OpenGL settings
		glViewport(0, 0, oldViewport[2], oldViewport[3]);
		glCullFace(GL_BACK);

		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDeleteFramebuffers(1, &captureFbo);
		glDeleteRenderbuffers(1, &captureRbo);

		delete cube;
		delete cubemapInfo;
		delete program;
	}
	void TextureCubemap::setupEmpty(int cubeSidelength, bool useMipmaps) const
	{
		bind();

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, useMipmaps ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		for (unsigned int i = 0; i < 6; i++)
		{
			glTexImage2D(CUBEMAP_TEXTURES[i], 0, GL_RGB16F, cubeSidelength, cubeSidelength, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
		}

		if (useMipmaps)	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

		unbind();
	}

	/* TextureInfo */
	TextureInfo::TextureInfo(GLenum unit, const std::string& uniformName, Texture* texture, Sampler* sampler) : unit(unit), uniformName(uniformName), texture(texture), sampler(sampler) {}
	
	void TextureInfo::updateShader(ShaderProgram* program) const
	{
		glActiveTexture(unit);
		texture->bind();
		program->setUniform(uniformName.c_str(), int(unit - GL_TEXTURE0));
		if (sampler)
		{
			sampler->bind(unit - GL_TEXTURE0);
		}
	}
	void TextureInfo::unbindSampler() const {
		if (sampler)
		{
			sampler->unbind(unit - GL_TEXTURE0);
		}
	}

	/* Samplers */
	Sampler::Sampler() { glGenSamplers(1, &id); }
	Sampler::~Sampler() { glDeleteSamplers(1, &id); }

	void Sampler::bind(GLuint unit) const { glBindSampler(unit, id); }
	void Sampler::unbind(GLuint unit) const { glBindSampler(unit, 0); }

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