#include "EditorLayer.h"
#include "imgui.h"
#include "ImGuizmo.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "MarsEngine/Scene/SceneSerializer.h"
#include "MarsEngine/Util/PlatformUtil.h"

namespace MarsEngine
{
	EditorLayer::EditorLayer()
		: Layer("Sandbox2D"), m_cameraController(1920.0f / 1080.0f, true)
	{
	}

	void EditorLayer::onAttach()
	{
		m_checkerboardTexture = Texture2D::create("assets/textures/Checkerboard.png");
		m_playIcon = Texture2D::create("assets/icons/PlayButton.png");
		m_simulateIcon = Texture2D::create("assets/icons/SimulateButton.png");
		m_stopIcon = Texture2D::create("assets/icons/StopButton.png");

		FramebufferSpecification fbSpec;
		fbSpec.attachment = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER,
			FramebufferTextureFormat::Depth };
		fbSpec.width = 1280;
		fbSpec.height = 720;
		m_framebuffer = Framebuffer::create(fbSpec);

		m_editorScene = createRef<Scene>();
		m_activeScene = m_editorScene;

		m_editorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);

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

		Renderer2D::resetStats();
		m_framebuffer->bind();
		RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::clear();

		m_framebuffer->clearAttachment(1, -1);

		switch (m_sceneState)
		{
			case SceneState::Edit:
			{
				if (m_viewportFocused)
				{
					m_cameraController.onUpdate(ts);
				}
				m_editorCamera.onUpdate(ts);
				m_activeScene->onUpdateEditor(ts, m_editorCamera);
				break;
			}
			case SceneState::Simulate:
			{
				m_editorCamera.onUpdate(ts);
				m_activeScene->onUpdateSimulation(ts, m_editorCamera);
				break;
			}
			case SceneState::Play:
			{
				m_activeScene->onUpdateRuntime(ts);
				break;
			}
		}

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
			if (pixelData == -1)
			{
				m_hoveredEntity = {};
			}
			else
			{
				m_hoveredEntity = { (entt::entity)pixelData, m_activeScene.get() };
			}
		}

		onOverlayRender();

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

				if (ImGui::MenuItem("Save", "Ctrl+S"))
				{
					saveScene();
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
		m_contentBrowserPanel.onImGuiRender();

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

		ImGui::Begin("Settings");
		ImGui::Checkbox("Show physics colliders", &m_showPhysicsColliders);
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");

		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		m_viewportFocused = ImGui::IsWindowFocused();
		m_viewportHovered = ImGui::IsWindowHovered();
		m_viewportBounds[0] = { viewportMinRegion.x + viewportOffset.x,
								viewportMinRegion.y + viewportOffset.y };
		m_viewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x,
								viewportMaxRegion.y + viewportOffset.y };

		Application::get().getImGuiLayer()->blockEvents(!m_viewportFocused && !m_viewportHovered);
		
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_viewportSize = { viewportPanelSize.x, viewportPanelSize.y };
		auto textureID = m_framebuffer->getColorAttachmentRendererID();
		ImGui::Image((void*)textureID, ImVec2{ m_viewportSize.x, m_viewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1,0 });
	
		if (ImGui::BeginDragDropTarget())
		{
			if (ImGuiPayload const* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				auto path = (wchar_t const*)payload->Data;
				openScene(path);
			}
			ImGui::EndDragDropTarget();
		}

		Math::Mat4 testMat4;
		std::cout << testMat4[1][2] << std::endl;

		ImGui::End();
		ImGui::PopStyleVar();

		UI_toolbar();

		ImGui::End();
	}

	void EditorLayer::UI_toolbar()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

		auto& colors = ImGui::GetStyle().Colors;
		auto const& buttonHovered = colors[ImGuiCol_ButtonHovered];
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
			ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));

		auto const& buttonActive = colors[ImGuiCol_ButtonActive];
		ImGui::PushStyleColor(ImGuiCol_ButtonActive,
			ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

		
		ImGui::Begin("##toolbar", nullptr, ImGuiWindowFlags_NoDecoration
			| ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		bool toolbarEnabled = (bool)m_activeScene;

		ImVec4 tintColor = ImVec4(1, 1, 1, 1);
		if (!toolbarEnabled)
		{
			tintColor.w = 0.5f;
		}

		auto size = ImGui::GetWindowHeight() - 4.0f;
		ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));

		bool hasPlayButton = m_sceneState == SceneState::Edit || m_sceneState == SceneState::Play;
		bool hasSimulateButton = m_sceneState == SceneState::Edit || m_sceneState == SceneState::Simulate;

		if (hasPlayButton)
		{
			Ref<Texture2D> icon = (m_sceneState == SceneState::Edit || m_sceneState == SceneState::Simulate) ? m_playIcon : m_stopIcon;
			if (ImGui::ImageButton((ImTextureID)(uint64_t)icon->getRendererID(),
				ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0,
				ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor) && toolbarEnabled)
			{
				if (m_sceneState == SceneState::Edit || m_sceneState == SceneState::Simulate)
				{
					onScenePlay();
				}
				else if (m_sceneState == SceneState::Play)
				{
					onSceneStop();
				}
			}
		}
		if (hasSimulateButton)
		{
			if (hasPlayButton)
			{
				ImGui::SameLine();
			}
			Ref<Texture2D> icon = (m_sceneState == SceneState::Edit || m_sceneState == SceneState::Play) ? m_simulateIcon : m_stopIcon;
			if (ImGui::ImageButton((ImTextureID)(uint64_t)icon->getRendererID(),
				ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0,
				ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor) && toolbarEnabled)
			{
				if (m_sceneState == SceneState::Edit || m_sceneState == SceneState::Play)
				{
					onSceneSimulate();
				}
				else if (m_sceneState == SceneState::Simulate)
				{
					onSceneStop();
				}
			}
		}
		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(3);
		ImGui::End();
	}

	void EditorLayer::onEvent(Event& e)
	{
		m_cameraController.onEvent(e);
		m_editorCamera.onEvent(e);

		EventDispatcher dispatcher(e);
		dispatcher.dispatch<KeyPressedEvent>(ME_BIND_EVENT_FUNC(EditorLayer::onKeyPressed));
		dispatcher.dispatch<MouseButtonPressedEvent>(ME_BIND_EVENT_FUNC(EditorLayer::onMouseButtonPressed));
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
				else if (control)
				{
					saveScene();
				}
				break;
			}

			//duplicate
			case ME_KEY_D:
			{
				if (control)
				{
					onDuplicateEntity();
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

	bool EditorLayer::onMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		if (e.getMouseButton() == ME_MOUSE_BUTTON_LEFT)
		{
			if (m_viewportHovered && !ImGuizmo::IsOver() && !Input::isKeyPressed(ME_KEY_LEFT_ALT))
			{
				m_sceneHierarchyPanel.setSelectedEntity(m_hoveredEntity);
			}
		}
		return false;
	}

	void EditorLayer::onOverlayRender()
	{
		if (m_sceneState == SceneState::Play)
		{
			auto camera = m_activeScene->getPrimaryCameraEntity();
			if (!camera)
			{
				return;
			}
			Renderer2D::beginScene(camera.getComponent<CameraComponent>().camera,
				camera.getComponent<TransformComponent>().getTransform());
		}
		else
		{
			Renderer2D::beginScene(m_editorCamera);
		}

		if (m_showPhysicsColliders)
		{
			{
				auto view = m_activeScene->getAllEntitiesWith<TransformComponent, BoxCollider2DComponent>();
				for (auto entity : view)
				{
					auto [tc, bc2dc] = view.get<TransformComponent, BoxCollider2DComponent>(entity);

					auto translation = tc.translation + glm::vec3(bc2dc.offset, 0.001f);
					auto scale = tc.scale * glm::vec3(bc2dc.size * 2.0f, 1.0f);

					auto transform = glm::translate(glm::mat4(1.0f), translation)
						* glm::rotate(glm::mat4(1.0f), tc.rotation.z, glm::vec3(0.0f, 0.0f, 1.0f))
						* glm::scale(glm::mat4(1.0f), scale);

					Renderer2D::drawRect(transform, glm::vec4(0, 1, 0, 1));
				}
			}

			{
				auto view = m_activeScene->getAllEntitiesWith<TransformComponent, CircleCollider2DComponent>();
				for (auto entity : view)
				{
					auto [tc, cc2dc] = view.get<TransformComponent, CircleCollider2DComponent>(entity);

					auto translation = tc.translation + glm::vec3(cc2dc.offset, 0.001f);
					auto scale = tc.scale * glm::vec3(cc2dc.radius * 2.0f);

					auto transform = glm::translate(glm::mat4(1.0f), translation)
						* glm::scale(glm::mat4(1.0f), scale);

					Renderer2D::drawCircle(transform, glm::vec4(0, 1, 0, 1), 0.01f);
				}
			}
		}

		Renderer2D::endScene();
	}

	void EditorLayer::newScene()
	{
		m_activeScene = createRef<Scene>();
		m_activeScene->onViewportResize((uint32_t)m_viewportSize.x, (uint32_t)m_viewportSize.y);
		m_sceneHierarchyPanel.setContext(m_activeScene);
		m_editorScenePath = std::filesystem::path();
	}

	void EditorLayer::openScene()
	{
		std::string filepath = FileDialog::openFile("MarsEngine Scene (*.mars)\0*.mars\0");
		if (!filepath.empty())
		{
			openScene(filepath);
		}
	}

	void EditorLayer::openScene(std::filesystem::path const& path)
	{
		if (m_sceneState != SceneState::Edit)
		{
			onSceneStop();
		}
		if (path.extension().string() != ".mars")
		{
			ME_WARN("Not a scene file", path.filename().string());
			return;
		}

		auto newScene = createRef<Scene>();
		SceneSerializer serializer(newScene);
		if (serializer.deserialize(path.string()))
		{
			m_editorScene = newScene;
			m_editorScene->onViewportResize((uint32_t)m_viewportSize.x, (uint32_t)m_viewportSize.y);
			m_sceneHierarchyPanel.setContext(m_editorScene);
			m_activeScene = m_editorScene;
			m_editorScenePath = path;
		}
	}

	void EditorLayer::saveScene()
	{
		if (m_editorScenePath.empty())
		{
			saveAsScene();
		}
		else
		{
			serializeScene(m_editorScene, m_editorScenePath);
		}
	}

	void EditorLayer::saveAsScene()
	{
		std::string filepath = FileDialog::saveFile("MarsEngine Scene (*.mars)\0*.mars\0");
		if (!filepath.empty())
		{
			serializeScene(m_editorScene, filepath);
			m_editorScenePath = filepath;
		}
	}

	void EditorLayer::serializeScene(Ref<Scene> scene, std::filesystem::path const& path)
	{
		SceneSerializer serializer(scene);
		serializer.serialize(path.string());
	}

	void EditorLayer::onScenePlay()
	{
		if (m_sceneState == SceneState::Simulate)
		{
			onSceneStop();
		}
		m_sceneState = SceneState::Play;
		m_activeScene = Scene::copy(m_editorScene);
		m_activeScene->onRuntimeStart();
		m_sceneHierarchyPanel.setContext(m_activeScene);
	}

	void EditorLayer::onSceneSimulate()
	{
		if (m_sceneState == SceneState::Play)
		{
			onSceneStop();
		}
		m_sceneState = SceneState::Simulate;
		m_activeScene = Scene::copy(m_editorScene);
		m_activeScene->onSimulationStart();
		m_sceneHierarchyPanel.setContext(m_activeScene);
	}

	void EditorLayer::onSceneStop()
	{
		if (m_sceneState == SceneState::Play)
		{
			m_activeScene->onRuntimeStop();
		}
		else // m_sceneState == SceneState::Simulate
		{
			m_activeScene->onSimulationStop();
		}
		m_sceneState = SceneState::Edit;
		m_activeScene = m_editorScene;
		m_sceneHierarchyPanel.setContext(m_activeScene);
	}

	void EditorLayer::onDuplicateEntity()
	{
		if (m_sceneState != SceneState::Edit)
		{
			return;
		}
		auto selectedEntity = m_sceneHierarchyPanel.getSelectedEntity();
		if (selectedEntity)
		{
			m_editorScene->duplicateEntity(selectedEntity);
		}
	}

}