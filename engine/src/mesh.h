#pragma once

#include <vector>
#include <GL/glew.h>

#include "vector.h"

namespace engine
{
	class Mesh
	{
	public:
		Mesh() = default;
		Mesh(std::vector<Vector3> vertices, std::vector<Vector2> texcoords, std::vector<Vector3> normals);

		void calculateTangents();
		void setup();
		void draw();

		// vertex shader layout
		static const GLuint VERTICES = 0;
		static const GLuint TEXCOORDS = 1;
		static const GLuint NORMALS = 2;
		static const GLuint TANGENTS = 3;
	private:
		std::vector<Vector3> vertices;
		std::vector<Vector2> texcoords;
		std::vector<Vector3> normals;
		std::vector<Vector3> tangents;

		GLuint vaoId = -1;
	};
}

