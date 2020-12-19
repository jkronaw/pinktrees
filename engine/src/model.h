#pragma once

#include <vector>

#include "meshloading.h"
#include "texture.h"

namespace engine {
	class Model
	{
	public:
		Model();
		~Model();

		void load(std::string directoryName);

		void setAlbedo(Texture2D* texture);
		void setNormal(Texture2D* texture);
		void setRoughness(Texture2D* texture);
		void setMetallic(Texture2D* texture);
		void setAO(Texture2D* texture);

		void draw(ShaderProgram* program);
	private:
		Mesh* mesh;

		Texture2D* loadedTextures[5];
		TextureInfo* activeTextures[5];

		Sampler* sampler;

	};
}


