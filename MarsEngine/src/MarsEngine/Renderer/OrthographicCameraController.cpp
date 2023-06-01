#include "pch.h"

#include "OrthographicCameraController.h"
#include "MarsEngine/Core/Input.h"
#include "MarsEngine/Core/KeyCodes.h"

namespace MarsEngine
{
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		: m_aspectRatio(aspectRatio),
		m_bounds({ -m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel }),
		m_camera(m_bounds.left, m_bounds.right, m_bounds.bottom, m_bounds.top),
		m_rotation(rotation)
	{
	}

	void OrthographicCameraController::onUpdate(Timestep ts)
	{
		ME_PROFILE_FUNCTION();

		if (Input::isKeyPressed(ME_KEY_A)) {
			m_cameraPosition.x -= m_cameraTranslationSpeed * ts;
		}
		else if (Input::isKeyPressed(ME_KEY_D)) {
			m_cameraPosition.x += m_cameraTranslationSpeed * ts;
		}
		if (Input::isKeyPressed(ME_KEY_S)) {
			m_cameraPosition.y -= m_cameraTranslationSpeed * ts;
		}
		else if (Input::isKeyPressed(ME_KEY_W)) {
			m_cameraPosition.y += m_cameraTranslationSpeed * ts;
		}

		if (m_rotation)
		{
			if (Input::isKeyPressed(ME_KEY_Q)) {
				m_cameraRotation += m_cameraRotationSpeed * ts;
			}
			else if (Input::isKeyPressed(ME_KEY_E)) {
				m_cameraRotation -= m_cameraRotationSpeed * ts;
			}
			m_camera.setRotation(m_cameraRotation);
		}
		m_camera.setPosition(m_cameraPosition);
	}

	void OrthographicCameraController::onEvent(Event& e)
	{
		ME_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.dispatch<MouseScrolledEvent>(ME_BIND_EVENT_FUNC(OrthographicCameraController::onMouseScrolled));
		dispatcher.dispatch<WindowResizeEvent>(ME_BIND_EVENT_FUNC(OrthographicCameraController::onWindowResized));
	}

	bool OrthographicCameraController::onMouseScrolled(MouseScrolledEvent& e)
	{
		ME_PROFILE_FUNCTION();

		m_zoomLevel -= e.getOffsetY() * 0.25f;
		m_zoomLevel = std::max(m_zoomLevel, 0.25f);
		m_bounds = { -m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel };
		m_camera.setProjection(m_bounds.left, m_bounds.right, m_bounds.bottom, m_bounds.top);
		return false;
	}

	bool OrthographicCameraController::onWindowResized(WindowResizeEvent& e)
	{
		ME_PROFILE_FUNCTION();

		m_aspectRatio = (float)e.getWidth() / (float)e.getHeight();
		m_bounds = { -m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel };
		m_camera.setProjection(m_bounds.left, m_bounds.right, m_bounds.bottom, m_bounds.top);
		return false;
	}
}