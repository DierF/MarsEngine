#pragma once

#include "glm/glm.hpp"

namespace MarsEngine
{
	namespace Math
	{
		bool decomposeTransform(glm::mat4 const& transform,
			glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale);
	}
}