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

	m_spriteSheet = MarsEngine::Texture2D::create("assets/textures/Checkerboard.png");

	m_subTexture = MarsEngine::SubTexture2D::createFromCoords(m_spriteSheet, { 2, 1 }, { 128, 128 }, { 1, 2 });

	m_particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	m_particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
	m_particle.SizeBegin = 0.5f, m_particle.SizeVariation = 0.3f, m_particle.SizeEnd = 0.0f;
	m_particle.LifeTime = 1.0f;
	m_particle.Velocity = { 0.0f, 0.0f };
	m_particle.VelocityVariation = { 3.0f, 1.0f };
	m_particle.Position = { 0.0f, 0.0f };
}

void Sandbox2D::onDetach()
{
	ME_PROFILE_FUNCTION();
}

void Sandbox2D::onUpdate(MarsEngine::Timestep ts)
{
	ME_PROFILE_FUNCTION();

	m_cameraController.onUpdate(ts);

	MarsEngine::Renderer2D::resetStats();
	{
		ME_PROFILE_SCOPE("Renderer Prep");
		MarsEngine::RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		MarsEngine::RenderCommand::clear();
	}

	{
		static float rotation = 0.0f;
		rotation += ts * 50.0f;

		ME_PROFILE_SCOPE("Renderer Draw");
		MarsEngine::Renderer2D::beginScene(m_cameraController.getCamera());
		MarsEngine::Renderer2D::drawRotatedQuad({ 1.0f, 0.0f }, { 0.8f, 0.8f }, glm::radians(-45.0f), { 0.8f, 0.2f, 0.3f, 1.0f });
		MarsEngine::Renderer2D::drawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		MarsEngine::Renderer2D::drawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
		MarsEngine::Renderer2D::drawQuad({ 0.0f, 0.0f, -0.1f }, { 20.0f, 20.0f }, m_checkerboardTexture, 10.0f);
		MarsEngine::Renderer2D::drawRotatedQuad({ -2.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, glm::radians(rotation), m_checkerboardTexture, 20.0f);
		MarsEngine::Renderer2D::endScene();

		MarsEngine::Renderer2D::beginScene(m_cameraController.getCamera());
		for (float y = -5.0f; y < 5.0f; y += 0.5f)
		{
			for (float x = -5.0f; x < 5.0f; x += 0.5f)
			{
				glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 0.5f) / 10.0f, 0.7f };
				MarsEngine::Renderer2D::drawQuad({ x, y }, { 0.45f, 0.45f }, color);
			}
		}
		MarsEngine::Renderer2D::endScene();
	}

	if (MarsEngine::Input::isMouseButtonPressed(ME_MOUSE_BUTTON_LEFT))
	{
		auto [x, y] = MarsEngine::Input::getMousePosition();
		auto width = MarsEngine::Application::get().getWindow().getWidth();
		auto height = MarsEngine::Application::get().getWindow().getHeight();

		auto bounds = m_cameraController.getBounds();
		auto pos = m_cameraController.getCamera().getPosition();
		x = (x / width) * bounds.getWidth() - bounds.getWidth() * 0.5f;
		y = bounds.getHeight() * 0.5f - (y / height) * bounds.getHeight();
		m_particle.Position = { x + pos.x, y + pos.y };
		for (int i = 0; i < 5; i++)
			m_particleSystem.Emit(m_particle);
	}

	m_particleSystem.OnUpdate(ts);
	m_particleSystem.OnRender(m_cameraController.getCamera());

	MarsEngine::Renderer2D::beginScene(m_cameraController.getCamera());
	MarsEngine::Renderer2D::drawQuad({ 3.0f, 2.0f, 0.5f }, { 1.0f, 2.0f }, m_subTexture);
	MarsEngine::Renderer2D::endScene();
}

void Sandbox2D::onImGuiRender()
{
	ME_PROFILE_FUNCTION();
	ImGui::Begin("Settings");

	auto stats = MarsEngine::Renderer2D::getStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls      : %d", stats.drawCalls);
	ImGui::Text("Quads           : %d", stats.quadCount);
	ImGui::Text("Vertices        : %d", stats.getTotalVertexCount());
	ImGui::Text("Indices         : %d", stats.getTotalIndexCount());

	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_squareColor));
	ImGui::End();
}

void Sandbox2D::onEvent(MarsEngine::Event& e)
{
	m_cameraController.onEvent(e);
}
