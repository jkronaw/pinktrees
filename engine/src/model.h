#pragma once

#include <vector>
#include <map>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "drawable.h"
#include "shader.h"
#include "mesh.h"
#include "texture.h"

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

        void processNode(aiNode* node, const aiScene* scene);
    };
}

