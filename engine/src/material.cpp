#include "material.h"

namespace engine
{
	Material::Material(Texture2D* albedoMap, Texture2D* normalMap, Texture2D* roughnessMap, Texture2D* metallicMap, Texture2D* aoMap) : albedoMap(albedoMap), normalMap(normalMap), roughnessMap(roughnessMap), metallicMap(metallicMap), aoMap(aoMap) {};

	void Material::bind(ShaderProgram* program)
	{
		// set material values
		program->setUniform("albedo", albedo);
		program->setUniform("normal", normal);
		program->setUniform("metallic", metallic);
		program->setUniform("roughness", roughness);
		program->setUniform("ao", ao);

		// set textures if existent
		if (useAlbedoMap && albedoMap)
		{
			glActiveTexture(GL_TEXTURE0);
			albedoMap->bind();
			program->setUniform("texAlbedo", 0);
		}

		if (useNormalMap && normalMap)
		{
			glActiveTexture(GL_TEXTURE1);
			normalMap->bind();
			program->setUniform("texNormal", 1);
		}

		if (useMetallicMap && metallicMap)
		{
			glActiveTexture(GL_TEXTURE2);
			metallicMap->bind();
			program->setUniform("texMetallic", 2);
		}

		if (useRoughnessMap && roughnessMap)
		{
			glActiveTexture(GL_TEXTURE3);
			roughnessMap->bind();
			program->setUniform("texRoughness", 3);
		}

		if (useAoMap && aoMap)
		{
			glActiveTexture(GL_TEXTURE4);
			aoMap->bind();
			program->setUniform("texAO", 4);
		}

		// set texture booleans
		program->setUniform("useAlbedoTex", useAlbedoMap && albedoMap);
		program->setUniform("useNormalTex", useNormalMap && normalMap);
		program->setUniform("useMetallicTex", useMetallicMap && metallicMap);
		program->setUniform("useRoughnessTex", useRoughnessMap && roughnessMap);
		program->setUniform("useAoTex", useAoMap && aoMap);
	};
}