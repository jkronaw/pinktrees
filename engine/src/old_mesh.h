//#pragma once
//
//#include <vector>
//#include <map>
//#include <GL/glew.h>
//#include <assimp/scene.h>
//
//#include "vector.h"
//#include "texture.h"
//#include "exceptions.h"
//
//namespace engine
//{
//	struct Vertex;
//	enum TextureType;
//	class Mesh;
//
//	struct Vertex
//	{
//		Vertex() = default;
//		Vertex(Vector3 position, Vector2 texcoords, Vector3 normal);
//		Vector3 position;
//		Vector2 texcoords;
//		Vector3 normal;
//		Vector3 tangent;
//	};
//
//	enum TextureType
//	{
//		ALBEDO,
//		NORMAL,
//		ROUGHNESS,
//		METALLIC,
//		AO,
//		NR_TEXTURE_TYPES
//	};
//
//	class Mesh
//	{
//	public:
//		Mesh() = default;
//		Mesh(std::vector<Vertex> vertices);
//		Mesh(aiMesh* mesh, const aiScene* scene, const std::map<std::string, Texture2D*>& loadedTextures);
//		~Mesh();
//
//		void calculateTangents();
//		void setup();
//		void draw(ShaderProgram* program = nullptr);
//
//		// vertex attributes
//		static const GLuint VERTICES = 0;
//		static const GLuint TEXCOORDS = 1;
//		static const GLuint NORMALS = 2;
//		static const GLuint TANGENTS = 3;
//	private:
//		std::vector<Vertex> vertices;
//		std::vector<GLuint> indices;
//
//		Texture2D* albedoMap;
//		Texture2D* normalMap;
//		Texture2D* metallicMap;
//		Texture2D* roughnessMap;
//		Texture2D* aoMap;
//
//		TextureInfo* textureInfos[5] = { nullptr };
//
//		GLuint vaoId = 0;
//		GLuint vboId = 0;
//		GLuint eboId = 0;
//	};
//
//	aiTextureType toAiTextureType(TextureType textureType);
//}
//
