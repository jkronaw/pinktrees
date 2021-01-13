#include "model.h"

namespace engine
{
	void Model::loadModel(std::string path)
	{
		Assimp::Importer import;
		const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
			return;
		}

		std::string directory = path.substr(0, path.find_last_of('/'));

		for (int i = 0; i < scene->mNumMaterials; i++)
		{
			aiMaterial* material = scene->mMaterials[i];
			aiString aiStr;

			for (int i = 0; i < NR_TEXTURE_TYPES; i++)
			{
				aiTextureType aiTextureType = toAiTextureType(static_cast<TextureType>(i));
				for (int j = 0; j < material->GetTextureCount(aiTextureType); j++)
				{
					material->GetTexture(aiTextureType, j, &aiStr);

					std::string path(aiStr.C_Str());

					Texture2D* texture = new Texture2D();
					loadedTextures.insert(std::pair<std::string, Texture2D*>(path, texture));

					path = directory + path;
					texture->loadFromDisk(path);
				}
			}

			processNode(scene->mRootNode, scene);
		}
	}

	void Model::processNode(aiNode* node, const aiScene* scene)
	{
		// process all the node's meshes (if any)
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(new Mesh(mesh, scene, loadedTextures));
		}

		// then do the same for each of its children
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}
	}

	void Model::draw(ShaderProgram* program)
	{
		for (Mesh* mesh : meshes)
		{
			mesh->draw();		
		}
	}
}