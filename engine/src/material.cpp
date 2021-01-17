#include "material.h"

namespace engine
{
	//TODO: Revise init values for floats (especially check wether normal is correct)
	Material::Material()
		:albedoMap(nullptr), normalMap(nullptr), roughnessMap(nullptr), metallicMap(nullptr), aoMap(nullptr),
		albedo(Vector3(1, 0, 0)), normal(Vector3(0,0,1)), roughness(0.8f), metallic(0.5f), ao(1.0f) {};

	Material::Material(Texture2D* albedoMap, Texture2D* normalMap, Texture2D* roughnessMap, Texture2D* metallicMap, Texture2D* aoMap)
		:albedoMap(albedoMap), normalMap(normalMap), roughnessMap(roughnessMap), metallicMap(metallicMap), aoMap(aoMap),
		albedo(Vector3(0, 0, 0)), normal(Vector3(0, 0, 1)), roughness(0.5f), metallic(0.5f), ao(1.0f) {};

	Material::~Material() {};

	void Material::bind(ShaderProgram* program) {
		if (albedoMap) {
			glActiveTexture(GL_TEXTURE0);
			albedoMap->bind();
			program->setUniform("texAlbedo", 0);
			program->setUniform("useAlbedoTex", 1);
		}
		else 
			program->setUniform("useAlbedoTex", 0);
		

		if (normalMap) {
			glActiveTexture(GL_TEXTURE1);
			normalMap->bind();
			program->setUniform("texNormal", 1);
			program->setUniform("useNormalTex", 1);
		}
		else
			program->setUniform("useNormalTex", 0);

		if (metallicMap) {
			glActiveTexture(GL_TEXTURE2);
			metallicMap->bind();
			program->setUniform("texMetallic", 2);
			program->setUniform("useMetallicTex", 1);
		}
		else 
			program->setUniform("useMetallicTex", 0);
		

		if (roughnessMap) {
			glActiveTexture(GL_TEXTURE3);
			roughnessMap->bind();
			program->setUniform("texRoughness", 3);
			program->setUniform("useRoughnessTex", 1);
		}
		else 
			program->setUniform("useRoughnessTex", 0);

			
		if (aoMap) {
			glActiveTexture(GL_TEXTURE4);
			aoMap->bind();
			program->setUniform("texAO", 4);
			program->setUniform("useAoTex", 1);
		}
		else
			program->setUniform("useAoTex", 0);

		program->setUniform("albedo", albedo);
		program->setUniform("normal", normal);
		program->setUniform("metallic", metallic);
		program->setUniform("roughness", roughness);
		program->setUniform("ao", ao);
	};
}