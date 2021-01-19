#pragma once

#include "vector.h"
#include "texture.h"

namespace engine
{
	struct Material
	{
		Vector3 albedo = Vector3(0.5f, 1.f, 1.f);
		Vector3 normal = Vector3(0.f, 0.f, 1.f);
		float metallic = 0.5;
		float roughness = 0.5;
		float ao = 0.5;

		Texture2D* albedoMap = nullptr;
		Texture2D* normalMap = nullptr;
		Texture2D* metallicMap = nullptr;
		Texture2D* roughnessMap = nullptr;
		Texture2D* aoMap = nullptr;

		bool useAlbedoMap = true;
		bool useNormalMap = true;
		bool useMetallicMap = true;
		bool useRoughnessMap = true;
		bool useAoMap = true;

		std::string name = "unnamed";

		Material() = default;
		Material(Texture2D* albedoMap, Texture2D* normalMap, Texture2D* roughnessMap, Texture2D* metallicMap, Texture2D* aoMap);
		~Material() = default;

		void bind(ShaderProgram* program);
	};
}
