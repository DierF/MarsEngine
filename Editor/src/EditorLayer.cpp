#include "EditorLayer.h"
#include "imgui.h"
#include "ImGuizmo.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "MarsEngine/Scene/SceneSerializer.h"
#include "MarsEngine/Util/PlatformUtil.h"
#include "MarsEngine/Math/Math.h"

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
		fbSpec.attachment = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER,
			FramebufferTextureFormat::Depth };
		fbSpec.width = 1280;
		fbSpec.height = 720;
		m_framebuffer = Framebuffer::create(fbSpec);

		m_activeScene = createRef<Scene>();

		m_editorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);

#if 0
		auto square = m_activeScene->createEntity("Green Square");
		square.addComponent<SpriteRendererComponent>(glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f });
		auto redSquare = m_activeScene->createEntity("Red Square");
		redSquare.addComponent<SpriteRendererComponent>(glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f });
		
		m_squareEntity = square;

		m_cameraEntity = m_activeScene->createEntity("Camera A");
		m_cameraEntity.addComponent<CameraComponent>();
		m_secondCameraEntity = m_activeScene->createEntity("Camera B");
		auto& cc = m_secondCameraEntity.addComponent<CameraComponent>();
		cc.primary = false;

		class CameraController : public ScriptableEntity
		{
		public:
			void onCreate()
			{
				auto& translation = getComponent<TransformComponent>().translation;
				translation.x = rand() % 10 - 5.0f;
			}

			void onDestroy()
			{
			}

			void onUpdate(Timestep ts)
			{
				auto& translation = getComponent<TransformComponent>().translation;
				float speed = 5.0f;

				if (Input::isKeyPressed(ME_KEY_A))
				{
					translation.x -= speed * ts;
				}
				if (Input::isKeyPressed(ME_KEY_D))
				{
					translation.x += speed * ts;
				}
				if (Input::isKeyPressed(ME_KEY_W))
				{
					translation.y += speed * ts;
				}
				if (Input::isKeyPressed(ME_KEY_S))
				{
					translation.y -= speed * ts;
				}
			}
		};
		m_cameraEntity.addComponent<NativeScriptComponent>().bind<CameraController>();
		m_secondCameraEntity.addComponent<NativeScriptComponent>().bind<CameraController>();
#endif

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
			m_editorCamera.setViewportSize(m_viewportSize.x, m_viewportSize.y);
			m_activeScene->onViewportResize((uint32_t)m_viewportSize.x, (uint32_t)m_viewportSize.y);
		}

		if (m_viewportFocused)
		{
			m_cameraController.onUpdate(ts);
			m_editorCamera.onUpdate(ts);
		}


		Renderer2D::resetStats();
		m_framebuffer->bind();
		RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::clear();

		m_framebuffer->clearAttachment(1, -1);

		m_activeScene->onUpdateEditor(ts, m_editorCamera);

		auto [mx, my] = ImGui::GetMousePos();
		mx -= m_viewportBounds[0].x;
		my -= m_viewportBounds[0].y;
		glm::vec2 viewportSize = m_viewportBounds[1] - m_viewportBounds[0];
		my = viewportSize.y - my;
		int mouseX = (int)mx;
		int mouseY = (int)my;

		if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
		{
			auto pixelData = m_framebuffer->readPixel(1, mouseX, mouseY);
			if (pixelData < 0)
			{
				m_hoveredEntity = {};
			}
			else
			{
				m_hoveredEntity = { (entt::entity)pixelData, m_activeScene.get() };
			}
		}

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
		ImGuiStyle& style = ImGui::GetStyle();
		float minWinSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 400.0f;
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
		style.WindowMinSize.x = minWinSizeX;

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows,
				// which we can't undo at the moment without finer window depth/z control.
				//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);

				if (ImGui::MenuItem("New", "Ctrl+N"))
				{
					newScene();
				}

				if (ImGui::MenuItem("Open...", "Ctrl+O"))
				{
					openScene();
				}

				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
				{
					saveAsScene();
				}

				if (ImGui::MenuItem("Exit"))
				{
					Application::get().close();
				}
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		m_sceneHierarchyPanel.onImGuiRender();

		ImGui::Begin("Stats");

		std::string name("None");
		if (m_hoveredEntity)
		{
			name = m_hoveredEntity.getComponent<TagComponent>().tag;
		}
		ImGui::Text("Hovered Entity: %s", name.c_str());

		auto stats = Renderer2D::getStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls      : %d", stats.drawCalls);
		ImGui::Text("Quads           : %d", stats.quadCount);
		ImGui::Text("Vertices        : %d", stats.getTotalVertexCount());
		ImGui::Text("Indices         : %d", stats.getTotalIndexCount());

		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");
		auto viewportOffset = ImGui::GetCursorPos();

		m_viewportFocused = ImGui::IsWindowFocused();
		m_viewportHovered = ImGui::IsWindowHovered();
		Application::get().getImGuiLayer()->blockEvents(!m_viewportFocused && !m_viewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_viewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		auto textureID = m_framebuffer->getColorAttachmentRendererID();
		ImGui::Image((void*)textureID, ImVec2{ m_viewportSize.x, m_viewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1,0 });
		
		auto windowSize = ImGui::GetWindowSize();
		auto minBound = ImGui::GetWindowPos();
		minBound.x += viewportOffset.x;
		minBound.y += viewportOffset.y;

		ImVec2 maxBound = { minBound.x + windowSize.x, minBound.y + windowSize.y };
		m_viewportBounds[0] = { minBound.x, minBound.y };
		m_viewportBounds[1] = { maxBound.x, maxBound.y };

		//Gizmos
		Entity selectedEntity = m_sceneHierarchyPanel.getSelectedEntity();
		if (selectedEntity && m_gizmoType != -1)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();
			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y,
				ImGui::GetWindowWidth(), ImGui::GetWindowHeight());

			//auto cameraEntity = m_activeScene->getPrimaryCameraEntity();
			//auto const& camera = cameraEntity.getComponent<CameraComponent>().camera;
			//glm::mat4 const& cameraProjection = camera.getProjection();
			//glm::mat4 cameraView = glm::inverse(cameraEntity.getComponent<TransformComponent>().getTransform());

			glm::mat4 const& cameraProjection = m_editorCamera.getProjection();
			glm::mat4 cameraView = m_editorCamera.getViewMatrix();

			auto& tc = selectedEntity.getComponent<TransformComponent>();
			glm::mat4 transform = tc.getTransform();

			bool snap = Input::isKeyPressed(ME_KEY_LEFT_CONTROL);
			float snapValue = 0.5f;
			if (m_gizmoType == ImGuizmo::OPERATION::ROTATE)
			{
				snapValue = 45.0f;
			}
			float snapValues[3] = { snapValue, snapValue, snapValue };

			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
				(ImGuizmo::OPERATION)m_gizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
				nullptr, snap ? snapValues : nullptr);

			if (ImGuizmo::IsUsing())
			{
				glm::vec3 translation, rotation, scale;
				Math::decomposeTransform(transform, translation, rotation, scale);

				glm::vec3 deltaRotation = rotation - tc.rotation;
				tc.translation = translation;
				tc.rotation += deltaRotation;
				tc.scale = scale;
			}
		}

		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();
	}

	void EditorLayer::onEvent(Event& e)
	{
		m_cameraController.onEvent(e);
		m_editorCamera.onEvent(e);

		EventDispatcher dispatcher(e);
		dispatcher.dispatch<KeyPressedEvent>(ME_BIND_EVENT_FUNC(EditorLayer::onKeyPressed));
	}

	bool EditorLayer::onKeyPressed(KeyPressedEvent& e)
	{
		if (e.getRepeatCount() > 0)
		{
			return false;
		}
		bool control = Input::isKeyPressed(ME_KEY_LEFT_CONTROL)
			|| Input::isKeyPressed(ME_KEY_RIGHT_CONTROL);
		bool shift = Input::isKeyPressed(ME_KEY_LEFT_SHIFT)
			|| Input::isKeyPressed(ME_KEY_RIGHT_SHIFT);
		switch (e.getKeyCode())
		{
		case ME_KEY_N:
		{
			if (control)
			{
				newScene();
			}
			break;
		}
		case ME_KEY_O:
		{
			if (control)
			{
				openScene();
			}
			break;
		}
		case ME_KEY_S:
		{
			if (control && shift)
			{
				saveAsScene();
			}
			break;
		}

		//Gizmos
		case ME_KEY_Q:
		{
			m_gizmoType = -1;
			break;
		}
		case ME_KEY_W:
		{
			m_gizmoType = ImGuizmo::OPERATION::TRANSLATE;
			break;
		}
		case ME_KEY_E:
		{
			m_gizmoType = ImGuizmo::OPERATION::ROTATE;
			break;
		}
		case ME_KEY_R:
		{
			m_gizmoType = ImGuizmo::OPERATION::SCALE;
			break;
		}
		}
	}

	void EditorLayer::newScene()
	{
		m_activeScene = createRef<Scene>();
		m_activeScene->onViewportResize((uint32_t)m_viewportSize.x, (uint32_t)m_viewportSize.y);
		m_sceneHierarchyPanel.setContext(m_activeScene);
	}

	void EditorLayer::openScene()
	{
		std::string filepath = FileDialog::openFile("MarsEngine Scene (*.mars)\0*.mars\0");
		if (!filepath.empty())
		{
			m_activeScene = createRef<Scene>();
			m_activeScene->onViewportResize((uint32_t)m_viewportSize.x, (uint32_t)m_viewportSize.y);
			m_sceneHierarchyPanel.setContext(m_activeScene);

			SceneSerializer serializer(m_activeScene);
			serializer.deserialize(filepath);
		}
	}

	void EditorLayer::saveAsScene()
	{
		std::string filepath = FileDialog::saveFile("MarsEngine Scene (*.mars)\0*.mars\0");
		if (!filepath.empty())
		{
			SceneSerializer serializer(m_activeScene);
			serializer.serialize(filepath);
		}
	}
}