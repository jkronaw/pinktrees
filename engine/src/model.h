#pragma once

#include <vector>
#include <map>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "shader.h"
#include "mesh.h"
#include "texture.h"

namespace engine
{
    class Model
    {
    public:
        Model(std::string path)
        {
            loadModel(path);
        }
        void draw(ShaderProgram* program);
        
    //private:
        std::vector<Mesh*> meshes;
        std::map<std::string, Texture2D*> loadedTextures;

        void loadModel(std::string path);
        void processNode(aiNode* node, const aiScene* scene);
    };
}

