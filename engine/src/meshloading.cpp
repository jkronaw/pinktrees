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

	Mesh::Mesh(std::vector<Vector3> vertices, std::vector<Vector2> texcoords, std::vector<Vector3> normals)
	{
		this->vertices = vertices;
		this->texcoords = texcoords;
		this->normals = normals;
	}

	void Mesh::calculateTangents()
	{
		if (normals.empty() || texcoords.empty())
		{
			throw Exception("Normals and texcoords must be available to calculate tangent information.");
		}

		// clear any existing tangent information
		tangents.clear();

		for (unsigned int i = 0; i < vertices.size(); i += 3)
		{
			Vector3 edge1 = vertices[i + 1] - vertices[i];
			Vector3 edge2 = vertices[i + 2] - vertices[i];

			Vector2 deltaUV1 = texcoords[i + 1] - texcoords[i];
			Vector2 deltaUV2 = texcoords[i + 2] - texcoords[i];

			// float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
			// multiplication by f is omitted to gain performance
			// it is not required since tangent is normalized later anyway
			Vector3 tangent = deltaUV2.y * edge1 - deltaUV1.y * edge2;

			// use same tangent for each of a face's three vertices
			tangents.push_back(tangent);
			tangents.push_back(tangent);
			tangents.push_back(tangent);

			// bitangent is computed in vertex shader to save memory
		}

		for (unsigned int i = 0; i < vertices.size(); i += 3)
		{
			Vector3 normal = normals[i].normalized();
			Vector3 tangent = tangents[i].normalized();

			// re-orthogonalize tangent vector using Gram-Schmidt
			tangents[i] = (tangent - tangent.dot(normal) * normal).normalized();
		}
	}

	void Mesh::setup()
	{
		GLuint vboIds[4];

		glGenVertexArrays(1, &vaoId);
		glBindVertexArray(vaoId);
		{
			glGenBuffers(4, vboIds);
			glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vector3), &vertices[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(VERTICES);
			glVertexAttribPointer(VERTICES, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), 0);

			if (texcoords.size() > 0)
			{
				glBindBuffer(GL_ARRAY_BUFFER, vboIds[1]);
				glBufferData(GL_ARRAY_BUFFER, texcoords.size() * sizeof(Vector2), &texcoords[0], GL_STATIC_DRAW);
				glEnableVertexAttribArray(TEXCOORDS);
				glVertexAttribPointer(TEXCOORDS, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2), 0);
			}
			if (normals.size() > 0)
			{
				glBindBuffer(GL_ARRAY_BUFFER, vboIds[2]);
				glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(Vector3), &normals[0], GL_STATIC_DRAW);
				glEnableVertexAttribArray(NORMALS);
				glVertexAttribPointer(NORMALS, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), 0);
			}
			if (tangents.size() > 0)
			{
				glBindBuffer(GL_ARRAY_BUFFER, vboIds[3]);
				glBufferData(GL_ARRAY_BUFFER, tangents.size() * sizeof(Vector3), &tangents[0], GL_STATIC_DRAW);
				glEnableVertexAttribArray(TANGENTS);
				glVertexAttribPointer(TANGENTS, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), 0);
			}
		}

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDeleteBuffers(4, vboIds);
	}

	void Mesh::draw()
	{
		glBindVertexArray(vaoId);
		glDrawArrays(GL_TRIANGLES, 0, (GLsizei)vertices.size());
		glBindVertexArray(0);
	}
}
