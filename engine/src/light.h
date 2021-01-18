#pragma once

#include "vector.h"

namespace engine
{
	struct Light
	{
		Light(Vector3 position, Vector3 color, float brightness);
		Vector3 position;
		Vector3 color;
		float brightness = 1.f;
	};
}