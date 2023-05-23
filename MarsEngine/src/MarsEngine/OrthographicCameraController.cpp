#include "pch.h"

#include "OrthographicCameraController.h"
#include "MarsEngine/Input.h"
#include "MarsEngine/KeyCodes.h"

namespace MarsEngine
{
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		: m_aspectRatio(aspectRatio),
		m_camera(-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel),
		m_rotation(rotation)
	{
	}

	void OrthographicCameraController::onUpdate(Timestep ts)
	{
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
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<MouseScrolledEvent>(ME_BIND_EVENT_FUNC(OrthographicCameraController::onMouseScrolled));
		dispatcher.dispatch<WindowResizeEvent>(ME_BIND_EVENT_FUNC(OrthographicCameraController::onWindowResized));
	}

	bool OrthographicCameraController::onMouseScrolled(MouseScrolledEvent& e)
	{
		m_zoomLevel -= e.getOffsetY() * 0.25f;
		m_zoomLevel = std::max(m_zoomLevel, 0.25f);
		ME_CORE_TRACE("{0}, {1}", e.getOffsetY() * 0.25f, m_zoomLevel);
		m_camera.setProjection(-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel);
		return false;
	}

	bool OrthographicCameraController::onWindowResized(WindowResizeEvent& e)
	{
		m_zoomLevel = (float)e.getWidth() / (float)e.getHeight();
		m_camera.setProjection(-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel);
		return false;
	}
}