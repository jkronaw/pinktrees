#include "meshfactory.h"

#include <vector>

namespace engine
{
#pragma region Cube
	const Vertex CUBE_VERTICES[36] = {
		// back face
		Vertex(Vector3(-1.0f, -1.0f, -1.0f), Vector2(0.0f, 0.0f), Vector3(0.0f,  0.0f, -1.0f)), // bottom-left
		Vertex(Vector3( 1.0f,  1.0f, -1.0f), Vector2(1.0f, 1.0f), Vector3(0.0f,  0.0f, -1.0f)), // top-right
		Vertex(Vector3( 1.0f, -1.0f, -1.0f), Vector2(1.0f, 0.0f), Vector3(0.0f,  0.0f, -1.0f)), // bottom-right
		Vertex(Vector3( 1.0f,  1.0f, -1.0f), Vector2(1.0f, 1.0f), Vector3(0.0f,  0.0f, -1.0f)), // top-right
		Vertex(Vector3(-1.0f, -1.0f, -1.0f), Vector2(0.0f, 0.0f), Vector3(0.0f,  0.0f, -1.0f)), // bottom-left
		Vertex(Vector3(-1.0f,  1.0f, -1.0f), Vector2(0.0f, 1.0f), Vector3(0.0f,  0.0f, -1.0f)), // top-left
		// front face
		Vertex(Vector3(-1.0f, -1.0f,  1.0f), Vector2(0.0f, 0.0f), Vector3(0.0f,  0.0f,  1.0f)), // bottom-left
		Vertex(Vector3( 1.0f, -1.0f,  1.0f), Vector2(1.0f, 0.0f), Vector3(0.0f,  0.0f,  1.0f)), // bottom-right
		Vertex(Vector3( 1.0f,  1.0f,  1.0f), Vector2(1.0f, 1.0f), Vector3(0.0f,  0.0f,  1.0f)), // top-right
		Vertex(Vector3( 1.0f,  1.0f,  1.0f), Vector2(1.0f, 1.0f), Vector3(0.0f,  0.0f,  1.0f)), // top-right
		Vertex(Vector3(-1.0f,  1.0f,  1.0f), Vector2(0.0f, 1.0f), Vector3(0.0f,  0.0f,  1.0f)), // top-left
		Vertex(Vector3(-1.0f, -1.0f,  1.0f), Vector2(0.0f, 0.0f), Vector3(0.0f,  0.0f,  1.0f)), // bottom-left
		// left face
		Vertex(Vector3(-1.0f,  1.0f,  1.0f), Vector2(1.0f, 0.0f), Vector3(-1.0f, 0.0f,  0.0f)), // top-right
		Vertex(Vector3(-1.0f,  1.0f, -1.0f), Vector2(1.0f, 1.0f), Vector3(-1.0f, 0.0f,  0.0f)), // top-left
		Vertex(Vector3(-1.0f, -1.0f, -1.0f), Vector2(0.0f, 1.0f), Vector3(-1.0f, 0.0f,  0.0f)), // bottom-left
		Vertex(Vector3(-1.0f, -1.0f, -1.0f), Vector2(0.0f, 1.0f), Vector3(-1.0f, 0.0f,  0.0f)), // bottom-left
		Vertex(Vector3(-1.0f, -1.0f,  1.0f), Vector2(0.0f, 0.0f), Vector3(-1.0f, 0.0f,  0.0f)), // bottom-right
		Vertex(Vector3(-1.0f,  1.0f,  1.0f), Vector2(1.0f, 0.0f), Vector3(-1.0f, 0.0f,  0.0f)), // top-right
		// right face
		Vertex(Vector3( 1.0f,  1.0f,  1.0f), Vector2(1.0f, 0.0f), Vector3(1.0f,  0.0f,  0.0f)), // top-left
		Vertex(Vector3( 1.0f, -1.0f, -1.0f), Vector2(0.0f, 1.0f), Vector3(1.0f,  0.0f,  0.0f)), // bottom-right
		Vertex(Vector3( 1.0f,  1.0f, -1.0f), Vector2(1.0f, 1.0f), Vector3(1.0f,  0.0f,  0.0f)), // top-right
		Vertex(Vector3( 1.0f, -1.0f, -1.0f), Vector2(0.0f, 1.0f), Vector3(1.0f,  0.0f,  0.0f)), // bottom-right
		Vertex(Vector3( 1.0f,  1.0f,  1.0f), Vector2(1.0f, 0.0f), Vector3(1.0f,  0.0f,  0.0f)), // top-left
		Vertex(Vector3( 1.0f, -1.0f,  1.0f), Vector2(0.0f, 0.0f), Vector3(1.0f,  0.0f,  0.0f)), // bottom-left
		// bottom face
		Vertex(Vector3(-1.0f, -1.0f, -1.0f), Vector2(0.0f, 1.0f), Vector3(0.0f, -1.0f,  0.0f)), // top-right
		Vertex(Vector3( 1.0f, -1.0f, -1.0f), Vector2(1.0f, 1.0f), Vector3(0.0f, -1.0f,  0.0f)), // top-left
		Vertex(Vector3( 1.0f, -1.0f,  1.0f), Vector2(1.0f, 0.0f), Vector3(0.0f, -1.0f,  0.0f)), // bottom-left
		Vertex(Vector3( 1.0f, -1.0f,  1.0f), Vector2(1.0f, 0.0f), Vector3(0.0f, -1.0f,  0.0f)), // bottom-left
		Vertex(Vector3(-1.0f, -1.0f,  1.0f), Vector2(0.0f, 0.0f), Vector3(0.0f, -1.0f,  0.0f)), // bottom-right
		Vertex(Vector3(-1.0f, -1.0f, -1.0f), Vector2(0.0f, 1.0f), Vector3(0.0f, -1.0f,  0.0f)), // top-right
		// top face
		Vertex(Vector3(-1.0f,  1.0f, -1.0f), Vector2(0.0f, 1.0f), Vector3(0.0f,  1.0f,  0.0f)), // top-left
		Vertex(Vector3( 1.0f,  1.0f,  1.0f), Vector2(1.0f, 0.0f), Vector3(0.0f,  1.0f,  0.0f)), // bottom-right
		Vertex(Vector3( 1.0f,  1.0f, -1.0f), Vector2(1.0f, 1.0f), Vector3(0.0f,  1.0f,  0.0f)), // top-right
		Vertex(Vector3( 1.0f,  1.0f,  1.0f), Vector2(1.0f, 0.0f), Vector3(0.0f,  1.0f,  0.0f)), // bottom-right
		Vertex(Vector3(-1.0f,  1.0f, -1.0f), Vector2(0.0f, 1.0f), Vector3(0.0f,  1.0f,  0.0f)), // top-left
		Vertex(Vector3(-1.0f,  1.0f,  1.0f), Vector2(0.0f, 0.0f), Vector3(0.0f,  1.0f,  0.0f))  // bottom-left
	};

	Mesh* MeshFactory::createCube()
	{
		Mesh* mesh = new Mesh(std::vector<Vertex>(CUBE_VERTICES, CUBE_VERTICES + 36));
		mesh->setup();
		return mesh;
	}
#pragma endregion

#pragma region Quad
	const Vertex QUAD_VERTICES[6] = {
		Vertex(Vector3( 1.0f, -1.0f, 0.0f), Vector2(), Vector3()),
		Vertex(Vector3( 1.0f,  1.0f, 0.0f), Vector2(), Vector3()),
		Vertex(Vector3(-1.0f, -1.0f, 0.0f), Vector2(), Vector3()),
		Vertex(Vector3(-1.0f, -1.0f, 0.0f), Vector2(), Vector3()),
		Vertex(Vector3( 1.0f,  1.0f, 0.0f), Vector2(), Vector3()),
		Vertex(Vector3(-1.0f,  1.0f, 0.0f), Vector2(), Vector3())
	};

	Mesh* MeshFactory::createQuad()
	{
		Mesh* mesh = new Mesh(std::vector<Vertex>(QUAD_VERTICES, QUAD_VERTICES + 6));
		mesh->setup();
		return mesh;
	}
#pragma endregion
}
