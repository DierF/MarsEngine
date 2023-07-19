#pragma once

#include "glm/glm.hpp"

namespace MarsEngine
{
	class Camera
	{
	public:
		Camera() = default;

		Camera(glm::mat4 const& projection)
			: m_projection(projection) {}

		virtual ~Camera() = default;

		glm::mat4 const& getProjection() const { return m_projection; }

	protected:
		glm::mat4 m_projection = glm::mat4(1.0f);
	};
}