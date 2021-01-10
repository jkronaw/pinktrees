#pragma once

#include "meshloading.h"

namespace engine {
	class MeshFactory
	{
	public:
		static Mesh* createCube();
	private:
		MeshFactory();
	};
}
