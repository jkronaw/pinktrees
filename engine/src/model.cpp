#include "model.h"

namespace engine {

	void Model::load(std::string directoryName)
	{
		WavefrontLoader loaderGround;
		loaderGround.loadFile((directoryName + std::string("/mesh.obj")).c_str());

		mesh = loaderGround.getObjects()[0];
		mesh->calculateTangents();
		mesh->setup();

		Texture2D* albedo = new Texture2D();
		albedo->load(directoryName + "/albedo.png");

		Texture2D* normal = new Texture2D();
		normal->load(directoryName + "/normal.png");

		Texture2D* roughness = new Texture2D();
		roughness->load(directoryName + "/roughness.png");

		Texture2D* metallic = new Texture2D();
		metallic->load(directoryName + "/metallic.png");

		Texture2D* ao = new Texture2D();
		ao->load(directoryName + "/ao.png");

		Sampler* s = new LinearMipmapLinearSampler();
		textures[0] = new TextureInfo(GL_TEXTURE0, "texAlbedo", albedo, s);
		textures[1] = new TextureInfo(GL_TEXTURE1, "texNormal", normal, s);
		textures[2] = new TextureInfo(GL_TEXTURE2, "texRoughness", roughness, s);
		textures[3] = new TextureInfo(GL_TEXTURE3, "texMetallic", metallic, s);
		textures[4] = new TextureInfo(GL_TEXTURE4, "texAO", ao, s);
	}

	void Model::draw(ShaderProgram* program)
	{
		for (TextureInfo* tInfo : textures)
		{
			tInfo->updateShader(program);
		}

		mesh->draw();

		for (TextureInfo* tInfo : textures) {
			tInfo->unbindSampler();
		}	
	}
}