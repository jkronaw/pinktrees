#include "meshfactory.h"

#include <vector>

namespace engine {

	#pragma region Cube
	const Vector3 CUBE_VERTICES[36] = {
		// back face
		Vector3(-1.0f, -1.0f, -1.0f), // bottom-left
		Vector3( 1.0f,  1.0f, -1.0f), // top-right
		Vector3( 1.0f, -1.0f, -1.0f), // bottom-right
		Vector3( 1.0f,  1.0f, -1.0f), // top-right
		Vector3(-1.0f, -1.0f, -1.0f), // bottom-left
		Vector3(-1.0f,  1.0f, -1.0f), // top-left
		// front face
		Vector3(-1.0f, -1.0f,  1.0f), // bottom-left
		Vector3( 1.0f, -1.0f,  1.0f), // bottom-right
		Vector3( 1.0f,  1.0f,  1.0f), // top-right
		Vector3( 1.0f,  1.0f,  1.0f), // top-right
		Vector3(-1.0f,  1.0f,  1.0f), // top-left
		Vector3(-1.0f, -1.0f,  1.0f), // bottom-left
		// left face
		Vector3(-1.0f,  1.0f,  1.0f), // top-right
		Vector3(-1.0f,  1.0f, -1.0f), // top-left
		Vector3(-1.0f, -1.0f, -1.0f), // bottom-left
		Vector3(-1.0f, -1.0f, -1.0f), // bottom-left
		Vector3(-1.0f, -1.0f,  1.0f), // bottom-right
		Vector3(-1.0f,  1.0f,  1.0f), // top-right
		// right face
		Vector3( 1.0f,  1.0f,  1.0f), // top-left
		Vector3( 1.0f, -1.0f, -1.0f), // bottom-right
		Vector3( 1.0f,  1.0f, -1.0f), // top-right
		Vector3( 1.0f, -1.0f, -1.0f), // bottom-right
		Vector3( 1.0f,  1.0f,  1.0f), // top-left
		Vector3( 1.0f, -1.0f,  1.0f), // bottom-left
		// bottom face
		Vector3(-1.0f, -1.0f, -1.0f), // top-right
		Vector3( 1.0f, -1.0f, -1.0f), // top-left
		Vector3( 1.0f, -1.0f,  1.0f), // bottom-left
		Vector3( 1.0f, -1.0f,  1.0f), // bottom-left
		Vector3(-1.0f, -1.0f,  1.0f), // bottom-right
		Vector3(-1.0f, -1.0f, -1.0f), // top-right
		// top face
		Vector3(-1.0f,  1.0f, -1.0f), // top-left
		Vector3( 1.0f,  1.0f,  1.0f), // bottom-right
		Vector3( 1.0f,  1.0f, -1.0f), // top-right
		Vector3( 1.0f,  1.0f,  1.0f), // bottom-right
		Vector3(-1.0f,  1.0f, -1.0f), // top-left
		Vector3(-1.0f,  1.0f,  1.0f)  // bottom-left
	};

	const Vector2 CUBE_TEXCOORDS[36] = {
		// back face
		Vector2(0.0f, 0.0f), // bottom-left
		Vector2(1.0f, 1.0f), // top-right
		Vector2(1.0f, 0.0f), // bottom-right
		Vector2(1.0f, 1.0f), // top-right
		Vector2(0.0f, 0.0f), // bottom-left
		Vector2(0.0f, 1.0f), // top-left
		// front face
		Vector2(0.0f, 0.0f), // bottom-left
		Vector2(1.0f, 0.0f), // bottom-right
		Vector2(1.0f, 1.0f), // top-right
		Vector2(1.0f, 1.0f), // top-right
		Vector2(0.0f, 1.0f), // top-left
		Vector2(0.0f, 0.0f), // bottom-left
		// left face
		Vector2(1.0f, 0.0f), // top-right
		Vector2(1.0f, 1.0f), // top-left
		Vector2(0.0f, 1.0f), // bottom-left
		Vector2(0.0f, 1.0f), // bottom-left
		Vector2(0.0f, 0.0f), // bottom-right
		Vector2(1.0f, 0.0f), // top-right
		// right face
		Vector2(1.0f, 0.0f), // top-left
		Vector2(0.0f, 1.0f), // bottom-right
		Vector2(1.0f, 1.0f), // top-right
		Vector2(0.0f, 1.0f), // bottom-right
		Vector2(1.0f, 0.0f), // top-left
		Vector2(0.0f, 0.0f), // bottom-left
		// bottom face
		Vector2(0.0f, 1.0f), // top-right
		Vector2(1.0f, 1.0f), // top-left
		Vector2(1.0f, 0.0f), // bottom-left
		Vector2(1.0f, 0.0f), // bottom-left
		Vector2(0.0f, 0.0f), // bottom-right
		Vector2(0.0f, 1.0f), // top-right
		// top face
		Vector2(0.0f, 1.0f), // top-left
		Vector2(1.0f, 0.0f), // bottom-right
		Vector2(1.0f, 1.0f), // top-right
		Vector2(1.0f, 0.0f), // bottom-right
		Vector2(0.0f, 1.0f), // top-left
		Vector2(0.0f, 0.0f)	 // bottom-left
	};

	const Vector3 CUBE_NORMALS[36] = {
		// back face
		Vector3(0.0f,  0.0f, -1.0f),
		Vector3(0.0f,  0.0f, -1.0f),
		Vector3(0.0f,  0.0f, -1.0f),
		Vector3(0.0f,  0.0f, -1.0f),
		Vector3(0.0f,  0.0f, -1.0f),
		Vector3(0.0f,  0.0f, -1.0f),
		// front face
		Vector3(0.0f,  0.0f,  1.0f),
		Vector3(0.0f,  0.0f,  1.0f),
		Vector3(0.0f,  0.0f,  1.0f),
		Vector3(0.0f,  0.0f,  1.0f),
		Vector3(0.0f,  0.0f,  1.0f),
		Vector3(0.0f,  0.0f,  1.0f),
		// left face
		Vector3(-1.0f, 0.0f,  0.0f),
		Vector3(-1.0f, 0.0f,  0.0f),
		Vector3(-1.0f, 0.0f,  0.0f),
		Vector3(-1.0f, 0.0f,  0.0f),
		Vector3(-1.0f, 0.0f,  0.0f),
		Vector3(-1.0f, 0.0f,  0.0f),
		// right face
		Vector3(1.0f,  0.0f,  0.0f),
		Vector3(1.0f,  0.0f,  0.0f),
		Vector3(1.0f,  0.0f,  0.0f),
		Vector3(1.0f,  0.0f,  0.0f),
		Vector3(1.0f,  0.0f,  0.0f),
		Vector3(1.0f,  0.0f,  0.0f),
		// bottom face
		Vector3(0.0f, -1.0f,  0.0f),
		Vector3(0.0f, -1.0f,  0.0f),
		Vector3(0.0f, -1.0f,  0.0f),
		Vector3(0.0f, -1.0f,  0.0f),
		Vector3(0.0f, -1.0f,  0.0f),
		Vector3(0.0f, -1.0f,  0.0f),
		// top face
		Vector3(0.0f,  1.0f,  0.0f),
		Vector3(0.0f,  1.0f,  0.0f),
		Vector3(0.0f,  1.0f,  0.0f),
		Vector3(0.0f,  1.0f,  0.0f),
		Vector3(0.0f,  1.0f,  0.0f),
		Vector3(0.0f,  1.0f,  0.0f)
	};

	Mesh* MeshFactory::createCube()
	{
		Mesh* mesh = new Mesh(
			std::vector<Vector3>(CUBE_VERTICES, CUBE_VERTICES + 36),
			std::vector<Vector2>(CUBE_TEXCOORDS, CUBE_TEXCOORDS + 36),
			std::vector<Vector3>(CUBE_NORMALS, CUBE_NORMALS + 36)
		);
		mesh->setup();
		return mesh;
	}
	#pragma endregion

	#pragma region Quad
	const Vector3 QUAD_VERTICES[6] = {
		Vector3( 1.0f, -1.0f, 0.0f),
		Vector3( 1.0f,  1.0f, 0.0f),
		Vector3(-1.0f, -1.0f, 0.0f),
		Vector3(-1.0f, -1.0f, 0.0f),
		Vector3( 1.0f,  1.0f, 0.0f),
		Vector3(-1.0f,  1.0f, 0.0f)
	};

	Mesh* MeshFactory::createQuad()
	{
		Mesh* mesh = new Mesh(
			std::vector<Vector3>(QUAD_VERTICES, QUAD_VERTICES + 6),
			std::vector<Vector2>(),
			std::vector<Vector3>()
		);
		mesh->setup();
		return mesh;
	}
	#pragma endregion
}
