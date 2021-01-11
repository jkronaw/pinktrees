#include "skybox.h"

#include "meshfactory.h"

namespace engine {
	// static variable
	ShaderProgram* program = nullptr;

	Skybox::Skybox(const Camera* camera) {
		cube = MeshFactory::createCube();

		if (program == nullptr)
		{
			program = new ShaderProgram();
			program->init("shaders/skybox.vert", "shaders/skybox.frag");
			program->link();
			program->setUniformBlockBinding("SharedMatrices", camera->getUboBP());
		}
	}

	Skybox::~Skybox() {
		delete cube;
	}

	void Skybox::enableToneMapping()
	{
		program->use();
		program->setUniform("toneMap", true);
		program->unuse();
	}

	void Skybox::disableToneMapping()
	{
		program->use();
		program->setUniform("toneMap", false);
		program->unuse();
	}

	TextureCubemap* Skybox::getCubemap() const
	{
		return (TextureCubemap*)textureInfo->texture;
	}

	void Skybox::setCubemap(TextureCubemap* cubemap)
	{
		disableToneMapping();

		if (textureInfo != nullptr) delete textureInfo;
		textureInfo = new TextureInfo(GL_TEXTURE0, "cubemap", cubemap, nullptr);
	}

	void Skybox::loadCubemapFromDiskSingleFiles(const std::string& directoryName)
	{
		disableToneMapping();

		TextureCubemap* cubemap = new TextureCubemap();
		cubemap->loadFromDiskSingleFiles(directoryName);
		setCubemap(cubemap);
	}

	void Skybox::loadCubemapFromDiskHDR(const std::string& filename)
	{
		enableToneMapping();

		TextureCubemap* cubemap = new TextureCubemap();
		cubemap->loadFromDiskHDR(filename);
		setCubemap(cubemap);
	}

	void Skybox::draw() const {
		program->use();

		glCullFace(GL_FRONT);
		cube->draw();
		glCullFace(GL_BACK);

		program->unuse();
	}
}