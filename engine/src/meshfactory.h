#pragma once

#include "mesh.h"

namespace engine {
	class MeshFactory
	{
	public:
		static Mesh* createCube();
		static Mesh* createQuad();
	private:
		MeshFactory();
	};
}
