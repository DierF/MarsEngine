#pragma once

#include "MarsEngine/Renderer/Camera.h"

namespace MarsEngine
{
	class SceneCamera : public Camera
	{
	public:
		enum ProjectionType
		{
			Perspective = 0,
			Orthographic = 1
		};

	public:
		SceneCamera();

		virtual ~SceneCamera() = default;

		void setOrthographic(float size, float nearClip, float farClip);

		void setPerspective(float verticalFOV, float nearClip, float farClip);

		void setViewportSize(uint32_t width, uint32_t height);

		//---------------------------------------------------------------------
		//Perspecive

		float getPerspectiveVerticalFOV() const { return m_perspectiveFOV; }

		void setPerspectiveVerticalFOV(float verticalFOV)
		{
			m_perspectiveFOV = verticalFOV;
			recalculatePrjection();
		}

		float getPerspectiveNearClip() const { return m_perspectiveNear; }

		void setPerspectiveNearClip(float nearClip)
		{
			m_perspectiveNear = nearClip;
			recalculatePrjection();
		}

		float getPerspectiveFarClip() const { return m_perspectiveFar; }

		void setPerspectiveFarClip(float farClip)
		{
			m_perspectiveFar = farClip;
			recalculatePrjection();
		}

		//---------------------------------------------------------------------
		//Orthographic

		float getOrthographicSize() const { return m_orthographicSize; }

		void setOrthographicSize(float size)
		{
			m_orthographicSize = size;
			recalculatePrjection();
		}

		float getOrthographicNearClip() const { return m_orthographicNear; }

		void setOrthographicNearClip(float nearClip)
		{
			m_orthographicNear = nearClip;
			recalculatePrjection();
		}

		float getOrthographicFarClip() const { return m_orthographicFar; }

		void setOrthographicFarClip(float farClip)
		{
			m_orthographicFar = farClip;
			recalculatePrjection();
		}
		
		//---------------------------------------------------------------------

		ProjectionType getProjectionType() const{ return m_projectionType; }

		void setProjectionType(ProjectionType type)
		{
			m_projectionType = type;
			recalculatePrjection();
		}

	private:
		void recalculatePrjection();

	private:
		ProjectionType m_projectionType = ProjectionType::Orthographic;

		float m_perspectiveFOV = glm::radians(45.0f);
		float m_perspectiveNear = 0.01f, m_perspectiveFar = 1000.0f;

		float m_orthographicSize = 10.0f;
		float m_orthographicNear = -1.0f, m_orthographicFar = 1.0f;

		float m_aspectRatio = 1.0f;
	};
}