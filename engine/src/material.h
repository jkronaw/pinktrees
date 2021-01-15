#pragma once

#include "vector.h"
#include "texture.h"

namespace engine
{
	class Material
	{
	public:
		Vector3 albedo;
		Vector3 normal;
		Vector3 metallic;
		Vector3 roughness;
		Vector3 ao;

		Texture2D* albedoMap;
		Texture2D* normalMap;
		Texture2D* metallicMap;
		Texture2D* roughnessMap;
		Texture2D* aoMap;

	public:
		Material();
		Material(Texture2D* albedoMap, Texture2D* normalMap, Texture2D* roughnessMap, Texture2D* metallicMap, Texture2D* aoMap);
		~Material();

	};
}
