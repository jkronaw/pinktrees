#include "meshloading.h"

#include "exceptions.h"

namespace engine
{
	void WavefrontLoader::loadFile(const char* path)
	{
		std::ifstream ifile(path);

		std::string line;
		while (std::getline(ifile, line))
		{
			std::stringstream sline(line);
			parseLine(sline);
		}

		createObject();
	}

	std::vector<Mesh*> WavefrontLoader::getObjects() { return objects; }

	void WavefrontLoader::parseLine(std::stringstream& sin)
	{
		std::string s;
		sin >> s;

		// if vertexData have already been read, create a new object before moving on to the next object in the file
		if (s.compare("o") == 0 && !vertexData.empty()) createObject();

		else if (s.compare("v") == 0) parseVertex(sin);
		else if (s.compare("vt") == 0) parseTexcoord(sin);
		else if (s.compare("vn") == 0) parseNormal(sin);
		else if (s.compare("f") == 0) parseFace(sin);
	}
	void WavefrontLoader::parseVertex(std::stringstream& sin)
	{
		Vector3 v;
		sin >> v.x >> v.y >> v.z;
		vertexData.push_back(v);
	}
	void WavefrontLoader::parseTexcoord(std::stringstream& sin)
	{
		Vector2 t;
		sin >> t.u >> t.v;
		texcoordData.push_back(t);
	}
	void WavefrontLoader::parseNormal(std::stringstream& sin)
	{
		Vector3 n;
		sin >> n.x >> n.y >> n.z;
		normalData.push_back(n);
	}
	void WavefrontLoader::parseFace(std::stringstream& sin)
	{
		std::string token;
		if (normalData.empty() && texcoordData.empty())
		{
			for (int i = 0; i < 3; i++)
			{
				sin >> token;
				vertexIdx.push_back(std::stoi(token));
			}
		}
		else
		{
			for (int i = 0; i < 3; i++)
			{
				std::getline(sin, token, '/');
				if (token.size() > 0) vertexIdx.push_back(std::stoi(token));
				std::getline(sin, token, '/');
				if (token.size() > 0) texcoordIdx.push_back(std::stoi(token));
				std::getline(sin, token, ' ');
				if (token.size() > 0) normalIdx.push_back(std::stoi(token));
			}
		}
	}
	void WavefrontLoader::createObject()
	{
		std::vector<Vector3> vertices;
		std::vector<Vector2> texcoords;
		std::vector<Vector3> normals;

		for (unsigned int i = 0; i < vertexIdx.size(); i++)
		{
			unsigned int vi = vertexIdx[i];
			vertices.push_back(vertexData[vi - 1]);

			if (!texcoordData.empty())
			{
				unsigned int ti = texcoordIdx[i];
				texcoords.push_back(texcoordData[ti - 1]);
			}
			if (!normalData.empty())
			{
				unsigned int ni = normalIdx[i];
				normals.push_back(normalData[ni - 1]);
			}
		}

		// add a new object to the list
		objects.push_back(new Mesh(vertices, texcoords, normals));

		// clear as the data is not needed anymore
		vertexData.clear();
		texcoordData.clear();
		normalData.clear();
		vertexIdx.clear();
		texcoordIdx.clear();
		normalIdx.clear();
	}
}
