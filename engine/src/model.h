#pragma once

#include <vector>

#include "meshloading.h"
#include "texture.h"

namespace engine {
	class Model
	{
	public:
		TextureInfo* activeTextures[5];

		Model();
		~Model();

		void load(std::string directoryName);
		void useLoadedTextures();
		void draw(ShaderProgram* program);
	private:
		Mesh* mesh;
		Texture2D* loadedTextures[5];
		Sampler* sampler;

	};
}


