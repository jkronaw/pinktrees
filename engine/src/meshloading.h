#pragma once

#include <fstream>
#include <vector>
#include <sstream>

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


	class WavefrontLoader
	{
	public:
		void loadFile(const char* path);
		std::vector<Mesh*> getObjects();
	private:
		void parseLine(std::stringstream&);
		void parseVertex(std::stringstream&);
		void parseTexcoord(std::stringstream&);
		void parseNormal(std::stringstream&);
		void parseFace(std::stringstream&);
		void createObject();

		std::vector<Mesh*> objects;

		std::vector<Vector3> vertexData;
		std::vector<Vector2> texcoordData;
		std::vector<Vector3> normalData;

		std::vector<unsigned int> vertexIdx;
		std::vector<unsigned int> texcoordIdx;
		std::vector<unsigned int> normalIdx;
	};
}
