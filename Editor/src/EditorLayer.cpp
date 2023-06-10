#include "EditorLayer.h"
#include "imgui.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace MarsEngine
{
	EditorLayer::EditorLayer()
		: Layer("Sandbox2D"), m_cameraController(1920.0f / 1080.0f, true)
	{
	}

	void EditorLayer::onAttach()
	{
		m_checkerboardTexture = Texture2D::create("assets/textures/Checkerboard.png");

		FramebufferSpecification fbSpec;
		fbSpec.width = 1280;
		fbSpec.height = 720;
		m_framebuffer = Framebuffer::create(fbSpec);

		m_activeScene = createRef<Scene>();
		auto square = m_activeScene->createEntity("Square");
		square.addComponent<SpriteRendererComponent>(glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f });
		m_squareEntity = square;

		m_cameraEntity = m_activeScene->createEntity("Camera Entity");
		m_cameraEntity.addComponent<CameraComponent>();
		m_secondCameraEntity = m_activeScene->createEntity("Clip-space Camera Entity");
		auto& cc = m_secondCameraEntity.addComponent<CameraComponent>();
		cc.primary = false;

		class CameraController : public ScriptableEntity
		{
		public:
			void onCreate()
			{
				auto& transform = getComponent<TransformComponent>().transform;

			}

			void onDestroy()
			{

			}

			void onUpdate(Timestep ts)
			{
				auto& transform = getComponent<TransformComponent>().transform;
				float speed = 5.0f;

				if (Input::isKeyPressed(ME_KEY_A))
				{
					transform[3][0] -= speed * ts;
				}
				if (Input::isKeyPressed(ME_KEY_D))
				{
					transform[3][0] += speed * ts;
				}
				if (Input::isKeyPressed(ME_KEY_W))
				{
					transform[3][1] += speed * ts;
				}
				if (Input::isKeyPressed(ME_KEY_S))
				{
					transform[3][1] -= speed * ts;
				}
			}
		};
		m_cameraEntity.addComponent<NativeScriptComponent>().bind<CameraController>();
		m_secondCameraEntity.addComponent<NativeScriptComponent>().bind<CameraController>();

		m_sceneHierarchyPanel.setContext(m_activeScene);
	}

	void EditorLayer::onDetach()
	{
	}

	void EditorLayer::onUpdate(Timestep ts)
	{
		FramebufferSpecification spec = m_framebuffer->getSpecification();
		if (m_viewportSize.x > 0.0f && m_viewportSize.y > 0.0f &&
			(spec.width != m_viewportSize.x || spec.height != m_viewportSize.y))
		{
			m_framebuffer->resize((uint32_t)m_viewportSize.x, (uint32_t)m_viewportSize.y);
			m_cameraController.onResize(m_viewportSize.x, m_viewportSize.y);

			m_activeScene->onViewportResize((uint32_t)m_viewportSize.x, (uint32_t)m_viewportSize.y);
		}

		if (m_viewportFocused)
		{
			m_cameraController.onUpdate(ts);
		}

		Renderer2D::resetStats();
		m_framebuffer->bind();
		RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::clear();

		m_activeScene->onUpdate(ts);

		Renderer2D::endScene();

		m_framebuffer->unbind();
	}

	void EditorLayer::onImGuiRender()
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
					Application::get().close();
				}
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		m_sceneHierarchyPanel.onImGuiRender();

		ImGui::Begin("Settings");
		auto stats = Renderer2D::getStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls      : %d", stats.drawCalls);
		ImGui::Text("Quads           : %d", stats.quadCount);
		ImGui::Text("Vertices        : %d", stats.getTotalVertexCount());
		ImGui::Text("Indices         : %d", stats.getTotalIndexCount());

		if (m_squareEntity)
		{
			ImGui::Separator();
			auto& tag = m_squareEntity.getComponent<TagComponent>().tag;
			ImGui::Text("%s", tag.c_str());

			auto& squareColor = m_squareEntity.getComponent<SpriteRendererComponent>().color;
			ImGui::ColorEdit4("Square Color", glm::value_ptr(squareColor));
			ImGui::Separator();
		}
		
		ImGui::DragFloat3("Camera Transform",
			glm::value_ptr(m_cameraEntity.getComponent<TransformComponent>().transform[3]));

		if (ImGui::Checkbox("Camera A", &m_primaryCamera))
		{
			m_cameraEntity.getComponent<CameraComponent>().primary = m_primaryCamera;
			m_secondCameraEntity.getComponent<CameraComponent>().primary = !m_primaryCamera;
		}

		{
			auto& camera = m_secondCameraEntity.getComponent<CameraComponent>().camera;
			float orthoSize = camera.getOrthographicSize();
			if (ImGui::DragFloat("Second Ortho Size", &orthoSize))
			{
				camera.setOrthographicSize(orthoSize);
			}
		}

		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");

		m_viewportFocused = ImGui::IsWindowFocused();
		m_viewportHovered = ImGui::IsWindowHovered();
		Application::get().getImGuiLayer()->blockEvents(!m_viewportFocused || !m_viewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		if (m_viewportSize != *(glm::vec2*)(&viewportPanelSize))
		{
			//m_framebuffer->resize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
			m_viewportSize = { viewportPanelSize.x, viewportPanelSize.y };

			m_cameraController.onResize(viewportPanelSize.x, viewportPanelSize.y);
		}
		auto textureID = m_framebuffer->getColorAttachmentRendererID();
		ImGui::Image((void*)textureID, ImVec2{ m_viewportSize.x, m_viewportSize.y }, ImVec2{ 0,1 }, ImVec2{ 1,0 });
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();
	}

	void EditorLayer::onEvent(Event& e)
	{
		m_cameraController.onEvent(e);
	}
}