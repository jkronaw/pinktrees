#pragma once

#include <vector>

#include "meshloading.h"
#include "texture.h"

namespace engine {
	class Model
	{
	public:
		Mesh* mesh;

		TextureInfo* textures[5];

		TextureInfo* albedo;
		TextureInfo* normal;
		TextureInfo* roughness;
		TextureInfo* metallic;
		TextureInfo* ao;

		void load(std::string directoryName);
		void draw(ShaderProgram* program);
	};
}


