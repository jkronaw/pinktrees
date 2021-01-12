#pragma once

#include <vector>
#include <GL/glew.h>
#include <assimp/scene.h>

#include "vector.h"

namespace engine
{
	struct Vertex;
	class Mesh;

	struct Vertex
	{
		Vector3 position;
		Vector2 texcoords;
		Vector3 normal;
		Vector3 tangent;
	};

	class Mesh
	{
	public:
		Mesh() = default;
		Mesh(std::vector<Vertex> vertices);
		Mesh(std::vector<Vector3> positions, std::vector<Vector2> texcoords, std::vector<Vector3> normals);
		Mesh(aiMesh* mesh, const aiScene* scene);
		~Mesh();

		void calculateTangents();
		void setup();
		void draw();

		// vertex attributes
		static const GLuint VERTICES = 0;
		static const GLuint TEXCOORDS = 1;
		static const GLuint NORMALS = 2;
		static const GLuint TANGENTS = 3;
	private:
		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;

		GLuint vaoId = 0;
		GLuint vboId = 0;
		GLuint eboId = 0;
	};
}

