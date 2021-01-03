#pragma once

#include "shader.h"

namespace engine
{
	class IDrawable {
	public:
		virtual void draw(ShaderProgram* program) const = 0;
	};
}
