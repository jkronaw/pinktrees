#include "light.h"

namespace engine
{
	Light::Light(Vector3 position, Vector3 color, float brightness)
	{
		this->position = position;
		this->color = color;
		this->brightness = brightness;
	}
}