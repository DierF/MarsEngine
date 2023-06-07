#pragma once

#include "MarsEngine.h"

namespace MarsEngine
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();

		~EditorLayer() = default;

		virtual void onAttach() override;

		virtual void onDetach() override;

		virtual void onUpdate(Timestep ts) override;

		virtual void onImGuiRender() override;

		virtual void onEvent(Event& e) override;

	private:
		OrthographicCameraController m_cameraController;

		Ref<VertexArray> m_squareVA;
		Ref<Shader> m_flatColorShader;
		Ref<Framebuffer> m_framebuffer;

		Ref<Scene> m_activeScene;
		Entity m_squareEntity;

		Ref<Texture2D> m_checkerboardTexture;

		bool m_viewportFocused = false, m_viewportHovered = false;

		glm::vec2 m_viewportSize = { 0.0f, 0.0f };

		glm::vec4 m_squareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
	};
}