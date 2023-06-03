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

	MarsEngine::FramebufferSpecification fbSpec;
	fbSpec.width = 1920;
	fbSpec.height = 1080;
	m_framebuffer = MarsEngine::Framebuffer::create(fbSpec);

	m_spriteSheet = MarsEngine::Texture2D::create("assets/textures/Checkerboard.png");

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
		m_framebuffer->bind();
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
		m_framebuffer->unbind();
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
}

void Sandbox2D::onImGuiRender()
{
	ME_PROFILE_FUNCTION();
	//bool show = true;
	//ImGui::ShowDemoWindow(&show);

	static bool dockSpaceOpen = true;
    static bool opt_fullscreen = true;
    static bool opt_padding = false;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    else
    {
        dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
    // and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    if (!opt_padding)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", &dockSpaceOpen, window_flags);
    if (!opt_padding)
        ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // Submit the DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            // Disabling fullscreen would allow the window to be moved to the front of other windows,
            // which we can't undo at the moment without finer window depth/z control.
            ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
            ImGui::MenuItem("Padding", NULL, &opt_padding);
            ImGui::Separator();

            if (ImGui::MenuItem("Exit"))
			{
				MarsEngine::Application::get().close();
			}
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

	ImGui::Begin("Settings");

	auto stats = MarsEngine::Renderer2D::getStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls      : %d", stats.drawCalls);
	ImGui::Text("Quads           : %d", stats.quadCount);
	ImGui::Text("Vertices        : %d", stats.getTotalVertexCount());
	ImGui::Text("Indices         : %d", stats.getTotalIndexCount());

	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_squareColor));

	auto textureID = m_framebuffer->getColorAttachmentRendererID();
	ImGui::Image((void*)textureID, ImVec2{ 1920.0f, 1080.0f }, ImVec2{ 0,1 }, ImVec2{ 1,0 });
	ImGui::End();

    ImGui::End();
}

void Sandbox2D::onEvent(MarsEngine::Event& e)
{
	m_cameraController.onEvent(e);
}
