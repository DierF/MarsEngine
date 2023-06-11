#include "SceneHierarchyPanel.h"
#include "MarsEngine/Scene/Entity.h"
#include "MarsEngine/Scene/Component.h"
#include "imgui.h"
#include "glm/gtc/type_ptr.hpp"

namespace MarsEngine
{
	SceneHierarchyPanel::SceneHierarchyPanel(Ref<Scene> const& scene)
	{
		setContext(scene);
	}

	void SceneHierarchyPanel::setContext(Ref<Scene> const& scene)
	{
		m_context = scene;
	}

	void SceneHierarchyPanel::onImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		m_context->m_registry.each([&](auto entityID)
			{
				Entity entity{ entityID, m_context.get() };
				drawEntityNode(entity);
			});

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
		{
			m_selectionContext = {};
		}

		ImGui::End();

		ImGui::Begin("Properties");
		if (m_selectionContext)
		{
			drawComponents(m_selectionContext);
		}

		ImGui::End();
	}

	void SceneHierarchyPanel::drawEntityNode(Entity entity)
	{
		auto& tag = entity.getComponent<TagComponent>().tag;

		ImGuiTreeNodeFlags flags = (m_selectionContext == entity ? ImGuiTreeNodeFlags_Selected : 0)
			| ImGuiTreeNodeFlags_OpenOnArrow;
		auto opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			m_selectionContext = entity;
		}

		if (opened)
		{
			ImGui::TreePop();
		}
	}

	void SceneHierarchyPanel::drawComponents(Entity entity)
	{
		if (entity.hasComponment<TagComponent>())
		{
			auto& tag = entity.getComponent<TagComponent>().tag;
			
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());
			if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);

			}
		}

		if (entity.hasComponment<TransformComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(),
				ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
			{
				auto& transform = entity.getComponent<TransformComponent>().transform;
				ImGui::DragFloat3("Position", glm::value_ptr(transform[3]), 0.1f);

				ImGui::TreePop();
			}
		}

		if (entity.hasComponment<CameraComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(),
				ImGuiTreeNodeFlags_DefaultOpen, "Camera"))
			{
				auto& cameraC = entity.getComponent<CameraComponent>();
				auto& camera = cameraC.camera;

				ImGui::Checkbox("Primary", &cameraC.primary);

				char const* projectionTypeStrings[] = { "Perspective", "Orthographic" };
				char const* currentProjectionTypeString = projectionTypeStrings[(int)camera.getProjectionType()];
				if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
				{
					for (int i = 0; i < 2; ++i)
					{
						bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
						if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
						{
							currentProjectionTypeString = projectionTypeStrings[i];
							camera.setProjectionType((SceneCamera::ProjectionType)i);
						}

						if (isSelected)
						{
							ImGui::SetItemDefaultFocus();
						}
					}

					ImGui::EndCombo();
				}
				
				if (camera.getProjectionType() == SceneCamera::ProjectionType::Perspective)
				{
					float fov = glm::degrees(camera.getPerspectiveVerticalFOV());
					if (ImGui::DragFloat("Vertical FOV", &fov))
					{
						camera.setPerspectiveVerticalFOV(glm::radians(fov));
					}

					float near = camera.getPerspectiveNearClip();
					if (ImGui::DragFloat("NearClip", &near))
					{
						camera.setPerspectiveNearClip(near);
					}

					float far = camera.getPerspectiveFarClip();
					if (ImGui::DragFloat("FarClip", &far))
					{
						camera.setPerspectiveFarClip(far);
					}
				}
				if (camera.getProjectionType() == SceneCamera::ProjectionType::Orthographic)
				{
					float Size = camera.getOrthographicSize();
					if (ImGui::DragFloat("Size", &Size))
					{
						camera.setOrthographicSize(Size);
					}

					float near = camera.getOrthographicNearClip();
					if (ImGui::DragFloat("NearClip", &near))
					{
						camera.setOrthographicNearClip(near);
					}

					float far = camera.getOrthographicFarClip();
					if (ImGui::DragFloat("FarClip", &far))
					{
						camera.setOrthographicFarClip(far);
					}

					ImGui::Checkbox("Fixed Aspect Ratio", &cameraC.fixedAspectRatio);
				}
				ImGui::TreePop();
			}
		}

		if (entity.hasComponment<SpriteRendererComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(SpriteRendererComponent).hash_code(),
				ImGuiTreeNodeFlags_DefaultOpen, "Sprite Renderer"))
			{
				auto& spriteRendererC = entity.getComponent<SpriteRendererComponent>();
				ImGui::ColorEdit4("Color", glm::value_ptr(spriteRendererC.color));

				ImGui::TreePop();
			}
		}
	}
}