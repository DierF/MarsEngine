#include "SceneHierarchyPanel.h"
#include "MarsEngine/Scene/Entity.h"
#include "MarsEngine/Scene/Component.h"
#include "imgui.h"
#include "imgui_internal.h"
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

	static void drawVec3Control(std::string const& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		if (ImGui::Button("X", buttonSize))
		{
			values.x = resetValue;
		}
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		if (ImGui::Button("Y", buttonSize))
		{
			values.y = resetValue;
		}
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		if (ImGui::Button("Z", buttonSize))
		{
			values.z = resetValue;
		}
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
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
				auto& transformC = entity.getComponent<TransformComponent>();
				drawVec3Control("Translation", transformC.translation);
				glm::vec3 rotation = glm::degrees(transformC.rotation);
				drawVec3Control("Rotation", rotation);
				transformC.rotation = glm::radians(rotation);
				drawVec3Control("Scale", transformC.scale, 1.0f);

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