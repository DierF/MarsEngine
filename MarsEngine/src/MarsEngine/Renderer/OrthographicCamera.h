#pragma once

#include "glm/glm.hpp"

namespace MarsEngine {

	class OrthographicCamera {

	public:
		OrthographicCamera(float left, float right, float bottom, float top);

		auto const& getPosition() const { return m_position; }

		void setPosition(glm::vec3 const& position) {
			m_position = position;
			recalculateViewMartrix();
		}

		auto getRotation() const { return m_rotation; }

		void setRotation(float rotation) {
			m_rotation = rotation;
			recalculateViewMartrix();
		}

		auto const& getProjectionMatrix() const { return m_projectionMatrix; }

		auto const& getViewMatrix() const { return m_viewMatrix; }

		auto const& getViewProjectionMatrix() const { return m_viewProjectionMatrix; }

	private:
		void recalculateViewMartrix();

	private:
		glm::mat4 m_projectionMatrix;
		glm::mat4 m_viewMatrix;
		glm::mat4 m_viewProjectionMatrix;

		glm::vec3 m_position = { 0.0f, 0.0f, 0.0f };
		float m_rotation = 0.0f;
	};
}