#include "material.h"

namespace engine
{
	//TODO: Revise init values for floats (especially check wether normal is correct)
	Material::Material()
		:albedoMap(nullptr), normalMap(nullptr), roughnessMap(nullptr), metallicMap(nullptr), aoMap(nullptr),
		albedo(Vector3(0, 0, 0)), normal(Vector3(0,0,1)), roughness(Vector3(0.5, 0, 0)), metallic(Vector3(0.5, 0, 0)), ao(Vector3(1, 1, 1)) {};

	Material::Material(Texture2D* albedoMap, Texture2D* normalMap, Texture2D* roughnessMap, Texture2D* metallicMap, Texture2D* aoMap)
		:albedoMap(albedoMap), normalMap(normalMap), roughnessMap(roughnessMap), metallicMap(metallicMap), aoMap(aoMap) {};

	Material::~Material() {};
}