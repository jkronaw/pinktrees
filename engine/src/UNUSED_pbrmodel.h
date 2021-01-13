#pragma once

#include <vector>

#include "drawable.h"
#include "mesh.h"
#include "texture.h"

namespace engine {
	class PBRModel : public IDrawable
	{
	public:
		TextureInfo* activeTextures[5];

		PBRModel();
		~PBRModel();

		void load(std::string directoryName);
		void useLoadedTextures();
		void draw(ShaderProgram* program) const override;
	private:
		Mesh* mesh;
		Texture2D* loadedTextures[5];
		Sampler* sampler;
	};
}


