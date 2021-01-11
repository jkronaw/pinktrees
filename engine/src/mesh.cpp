#include "mesh.h"

#include "exceptions.h"

namespace engine
{
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