#pragma once

#include <vector>
#include <map>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "drawable.h"
#include "shader.h"
#include "Mesh.h"
#include "texture.h"
#include "material.h"

namespace engine
{
    class Model : public IDrawable
    {
    public:
        Model(const std::string& path);
        void draw(ShaderProgram* program) const override;

    private:
        std::vector<Mesh*> meshes;
        std::map<std::string, Texture2D*> loadedTextures;
        std::map<int, Material*> materials;

        void processNode(aiNode* node, const aiScene* scene);
    };

    aiTextureType toAiTextureType(TextureType textureType);
}

