#pragma once

#include "MarsEngine/Renderer/OrthographicCamera.h"
#include "MarsEngine/Core/Timestep.h"
#include "MarsEngine/Event/ApplicationEvent.h"
#include "MarsEngine/Event/MouseEvent.h"

namespace MarsEngine
{
	struct OrthographicCameraBounds
	{
		float left, right;
		float bottom, top;

		float getWidth() { return right - left; }
		float getHeight() { return top - bottom; }
	};

	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);

		void onUpdate(Timestep ts);

		void onEvent(Event& e);

		OrthographicCamera& getCamera() { return m_camera; }

		OrthographicCamera const& getCamera() const { return m_camera; }

		float getZoomLevel() const { return m_zoomLevel; }

		void setZoomLevel(float level) { m_zoomLevel = level; calculateView(); }

		OrthographicCameraBounds const& getBounds() const { return m_bounds; }

	private:
		void calculateView();

		bool onMouseScrolled(MouseScrolledEvent& e);

		bool onWindowResized(WindowResizeEvent& e);

	private:
		float m_aspectRatio;
		float m_zoomLevel = 1.0f;

		OrthographicCameraBounds m_bounds;
		OrthographicCamera m_camera;

		bool m_rotation;
		glm::vec3 m_cameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_cameraRotation = 0.0f;
		float m_cameraTranslationSpeed = 1.0f;
		float m_cameraRotationSpeed = 5.0f;
	};
}