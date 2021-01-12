#pragma once

#include <fstream>
#include <vector>
#include <sstream>

#include "mesh.h"
#include "vector.h"

namespace engine
{
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
