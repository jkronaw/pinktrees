#include "Model.h"

namespace engine
{
	Model::Model(const std::string& path)
	{
		Assimp::Importer import;
		const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
			return;
		}

		std::string directory = path.substr(0, path.find_last_of('/') + 1);

		for (unsigned int i = 0; i < scene->mNumMaterials; i++)
		{
			Material* mat = new Material();
			aiMaterial* material = scene->mMaterials[i];
			if (std::strcmp(material->GetName().C_Str(), "DefaultMaterial") == 0) continue;

			// Load textures into material object and loadedTexture table
			aiString aiStr;
			for (int j = 0; j < NR_TEXTURE_TYPES; j++)
			{
				aiTextureType aiTextureType = toAiTextureType(static_cast<TextureType>(j));

				if (material->GetTextureCount(aiTextureType) > 0)
				{
					material->GetTexture(aiTextureType, 0, &aiStr);
					std::string path(aiStr.C_Str());

					Texture2D* texture = new Texture2D();
					loadedTextures.insert(std::pair<std::string, Texture2D*>(path, texture));
					path = directory + path;
					texture->loadFromDisk(path);

					switch (j)
					{
					case ALBEDO:
						mat->albedoMap = texture;
						break;
					case NORMAL:
						mat->normalMap = texture;
						break;
					case METALLIC:
						mat->metallicMap = texture;
						break;
					case ROUGHNESS:
						mat->roughnessMap = texture;
						break;
					case AO:
						mat->aoMap = texture;
						break;
					default:
						throw Exception("Undefined aiTextureType");
					}
				}
			}

			// Load Material values
			aiColor3D color(0.f, 0.f, 0.f);

			if(material->Get(AI_MATKEY_COLOR_DIFFUSE, color) == 0)
				mat->albedo = Vector3(color.r, color.b, color.g);
			if(material->Get(AI_MATKEY_COLOR_SPECULAR, color) == 0)			
				mat->metallic = color.r;
			if (material->Get(AI_MATKEY_SHININESS, color) == 0)
				mat->roughness = color.r;

			materials.insert(std::pair<int, Material*>(i, mat));
		}

		processNode(scene->mRootNode, scene);
	}

	void Model::processNode(aiNode* node, const aiScene* scene)
	{
		// process all the node's meshes (if any)
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* tMesh = scene->mMeshes[node->mMeshes[i]];
			Mesh* mesh = new Mesh(tMesh, scene, materials.at(tMesh->mMaterialIndex));
			mesh->calculateTangents();
			mesh->setup();
			meshes.push_back(mesh);
		}

		// then do the same for each of its children
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}
	}

	std::vector<Mesh*> Model::getMeshes()
	{
		return meshes;
	}

	void Model::draw(ShaderProgram* program) const
	{
		for (Mesh* mesh : meshes)
		{
			mesh->draw(program);
		}
	}

	aiTextureType toAiTextureType(TextureType textureType)
	{
		switch (textureType)
		{
		case ALBEDO:
			return aiTextureType_DIFFUSE;
		case NORMAL:
			return aiTextureType_HEIGHT;
		case METALLIC:
			return aiTextureType_SPECULAR;
		case ROUGHNESS:
			return aiTextureType_SHININESS;
		case AO:
			return aiTextureType_AMBIENT;
		default:
			throw Exception("Should not happen");
		}
	}
}