#pragma once

#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "shader.h"
#include "mesh.h"

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
        // model data
        std::vector<Mesh*> meshes;
        std::string directory;

        void loadModel(std::string path);
        void processNode(aiNode* node, const aiScene* scene);
        //std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
    };
}

