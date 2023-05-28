#include "Sandbox2D.h"
#include "imgui.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_cameraController(1920.0f / 1080.0f, true)
{
}

void Sandbox2D::onAttach()
{
	ME_PROFILE_FUNCTION();

	m_checkerboardTexture = MarsEngine::Texture2D::create("assets/textures/Checkerboard.png");
}

void Sandbox2D::onDetach()
{
	ME_PROFILE_FUNCTION();
}

void Sandbox2D::onUpdate(MarsEngine::Timestep ts)
{
	ME_PROFILE_FUNCTION();

	m_cameraController.onUpdate(ts);

	{
		ME_PROFILE_SCOPE("Renderer Prep");
		MarsEngine::RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		MarsEngine::RenderCommand::clear();
	}

	{
		ME_PROFILE_SCOPE("Renderer Draw");
		MarsEngine::Renderer2D::beginScene(m_cameraController.getCamera());
		MarsEngine::Renderer2D::drawRotatedQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, glm::radians(-45.0f), { 0.8f, 0.2f, 0.3f, 1.0f });
		MarsEngine::Renderer2D::drawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
		MarsEngine::Renderer2D::drawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, m_checkerboardTexture, 10.0f);
		MarsEngine::Renderer2D::endScene();
	}
}

void Sandbox2D::onImGuiRender()
{
	ME_PROFILE_FUNCTION();
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_squareColor));
	ImGui::End();
}

void Sandbox2D::onEvent(MarsEngine::Event& e)
{
	m_cameraController.onEvent(e);
}
