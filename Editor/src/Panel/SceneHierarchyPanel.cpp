#include "SceneHierarchyPanel.h"
#include "MarsEngine/Scene/Entity.h"
#include "MarsEngine/Scene/Component.h"
#include "MarsEngine/Renderer/Texture.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "glm/gtc/type_ptr.hpp"
#include <filesystem>

namespace MarsEngine
{
	extern std::filesystem::path const s_assetPath;

	SceneHierarchyPanel::SceneHierarchyPanel(Ref<Scene> const& scene)
	{
		setContext(scene);
	}

	void SceneHierarchyPanel::setContext(Ref<Scene> const& scene)
	{
		m_context = scene;
		m_selectionContext = {};
	}

	void SceneHierarchyPanel::onImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		if (m_context)
		{
			m_context->m_registry.each([&](auto entityID)
				{
					Entity entity{ entityID, m_context.get() };
					drawEntityNode(entity);
				});

			if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			{
				m_selectionContext = {};
			}

			if (ImGui::BeginPopupContextWindow(0,
				ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems))
			{
				if (ImGui::MenuItem("Create Empty Entity"))
				{
					m_context->createEntity("Empty Entity");
				}
				ImGui::EndPopup();
			}
		}

		ImGui::End();

		ImGui::Begin("Properties");
		if (m_selectionContext)
		{
			drawComponents(m_selectionContext);
		}

		ImGui::End();
	}

	void SceneHierarchyPanel::setSelectedEntity(Entity entity)
	{
		m_selectionContext = entity;
	}

	void SceneHierarchyPanel::drawEntityNode(Entity entity)
	{
		auto& tag = entity.getComponent<TagComponent>().tag;

		ImGuiTreeNodeFlags flags = (m_selectionContext == entity ? ImGuiTreeNodeFlags_Selected : 0)
			| ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
		auto opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			m_selectionContext = entity;
		}

		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
			{
				entityDeleted = true;
			}
			ImGui::EndPopup();
		}

		if (opened)
		{
			ImGui::TreePop();
		}

		if (entityDeleted)
		{
			m_context->destroyEntity(entity);
			if (m_selectionContext == entity)
			{
				m_selectionContext = {};
			}
		}
	}

	static void drawVec3Control(std::string const& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

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
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
		{
			values.x = resetValue;
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
		{
			values.y = resetValue;
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
		{
			values.z = resetValue;
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
	}

	template<typename T, typename UIFunction>
	static void drawComponent(std::string const& name, Entity entity, UIFunction uifunction)
	{
		ImGuiTreeNodeFlags const treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen
			| ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth
			| ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

		if (entity.hasComponent<T>())
		{
			auto& component = entity.getComponent<T>();

			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImGui::Separator();
			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(),
				treeNodeFlags, name.c_str());
			ImGui::PopStyleVar();
			ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
			if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight }))
			{
				ImGui::OpenPopup("Component Settings");
			}
			bool removeComponent = false;
			if (ImGui::BeginPopup("Component Settings"))
			{
				if (ImGui::MenuItem("Remove Component"))
				{
					removeComponent = true;
				}
				ImGui::EndPopup();
			}
			if (open)
			{
				uifunction(component);
				ImGui::TreePop();
			}
			if (removeComponent)
			{
				entity.removeComponent<T>();
			}
		}
	}

	void SceneHierarchyPanel::drawComponents(Entity entity)
	{
		if (entity.hasComponent<TagComponent>())
		{
			auto& tag = entity.getComponent<TagComponent>().tag;
			
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());
			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);

			}
		}

		ImGui::SameLine();
		ImGui::PushItemWidth(-1);

		if (ImGui::Button("Add Component"))
		{
			ImGui::OpenPopup("AddComponent");
		}
		if (ImGui::BeginPopup("AddComponent"))
		{
			if (!m_selectionContext.hasComponent<CameraComponent>())
			{
				if (ImGui::MenuItem("Camera"))
				{
					m_selectionContext.addComponent<CameraComponent>();
					ImGui::CloseCurrentPopup();
				}
			}
			if (!m_selectionContext.hasComponent<SpriteRendererComponent>())
			{
				if (ImGui::MenuItem("Sprite Renderer"))
				{
					m_selectionContext.addComponent<SpriteRendererComponent>();
					ImGui::CloseCurrentPopup();
				}
			}
			if (!m_selectionContext.hasComponent<CircleRendererComponent>())
			{
				if (ImGui::MenuItem("Circle Renderer"))
				{
					m_selectionContext.addComponent<CircleRendererComponent>();
					ImGui::CloseCurrentPopup();
				}
			}
			if (!m_selectionContext.hasComponent<Rigidbody2DComponent>())
			{
				if (ImGui::MenuItem("Rigidbody 2D"))
				{
					m_selectionContext.addComponent<Rigidbody2DComponent>();
					ImGui::CloseCurrentPopup();
				}
			}
			if (!m_selectionContext.hasComponent<BoxCollider2DComponent>())
			{
				if (ImGui::MenuItem("Box Collider 2D"))
				{
					m_selectionContext.addComponent<BoxCollider2DComponent>();
					ImGui::CloseCurrentPopup();
				}
			}
			if (!m_selectionContext.hasComponent<CircleCollider2DComponent>())
			{
				if (ImGui::MenuItem("Circle Collider 2D"))
				{
					m_selectionContext.addComponent<CircleCollider2DComponent>();
					ImGui::CloseCurrentPopup();
				}
			}

			ImGui::EndPopup();
		}

		ImGui::PopItemWidth();

		drawComponent<TransformComponent>("Transform", entity,
			[](TransformComponent& component)
			{
				drawVec3Control("Translation", component.translation);
				glm::vec3 rotation = glm::degrees(component.rotation);
				drawVec3Control("Rotation", rotation);
				component.rotation = glm::radians(rotation);
				drawVec3Control("Scale", component.scale, 1.0f);
			});

		drawComponent<CameraComponent>("Camera", entity,
			[](CameraComponent& component)
			{
				auto& camera = component.camera;

				ImGui::Checkbox("Primary", &component.primary);

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

					float nearC = camera.getPerspectiveNearClip();
					if (ImGui::DragFloat("NearClip", &nearC))
					{
						camera.setPerspectiveNearClip(nearC);
					}

					float farC = camera.getPerspectiveFarClip();
					if (ImGui::DragFloat("FarClip", &farC))
					{
						camera.setPerspectiveFarClip(farC);
					}
				}
				if (camera.getProjectionType() == SceneCamera::ProjectionType::Orthographic)
				{
					float Size = camera.getOrthographicSize();
					if (ImGui::DragFloat("Size", &Size))
					{
						camera.setOrthographicSize(Size);
					}

					float nearC = camera.getOrthographicNearClip();
					if (ImGui::DragFloat("NearClip", &nearC))
					{
						camera.setOrthographicNearClip(nearC);
					}

					float farC = camera.getOrthographicFarClip();
					if (ImGui::DragFloat("FarClip", &farC))
					{
						camera.setOrthographicFarClip(farC);
					}

					ImGui::Checkbox("Fixed Aspect Ratio", &component.fixedAspectRatio);
				}
			});

		drawComponent<SpriteRendererComponent>("Sprite Renderer", entity,
			[](SpriteRendererComponent& component)
			{
				ImGui::ColorEdit4("Color", glm::value_ptr(component.color));

				ImGui::Button("Texture", ImVec2(100.0f, 0.0f));

				if (ImGui::BeginDragDropTarget())
				{
					if (auto payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
					{
						auto path = (wchar_t const*)payload->Data;
						auto texturePath = std::filesystem::path(path);
						ME_CORE_TRACE("{}", texturePath);
						component.texture = Texture2D::create(texturePath.string());
					}
					ImGui::EndDragDropTarget();
				}

				ImGui::DragFloat("Tiling Factor", &component.tilingFactor, 0.1f, 0.0f, 100.0f);
			});

		drawComponent<CircleRendererComponent>("Circle Renderer", entity,
			[](CircleRendererComponent& component)
			{
				ImGui::ColorEdit4("Color", glm::value_ptr(component.color));
				ImGui::DragFloat("Thickness", &component.thickness, 0.025f, 0.0f, 1.0f);
				ImGui::DragFloat("Fade", &component.fade, 0.00025f, 0.0f, 1.0f);
			});

		drawComponent<Rigidbody2DComponent>("Rigidbody 2D", entity,
			[](Rigidbody2DComponent& component)
			{
				char const* bodyTypeStrings[] = { "Static", "Dynamic", "Kinematic"};
				char const* currentBodyTypeString = bodyTypeStrings[(int)component.type];
				if (ImGui::BeginCombo("Projection", currentBodyTypeString))
				{
					for (int i = 0; i < 2; ++i)
					{
						bool isSelected = currentBodyTypeString == bodyTypeStrings[i];
						if (ImGui::Selectable(bodyTypeStrings[i], isSelected))
						{
							currentBodyTypeString = bodyTypeStrings[i];
							component.type = (Rigidbody2DComponent::BodyType)i;
						}

						if (isSelected)
						{
							ImGui::SetItemDefaultFocus();
						}
					}

					ImGui::EndCombo();
				}

				ImGui::Checkbox("Fixed Rotation", &component.fixedRotation);
			});

		drawComponent<BoxCollider2DComponent>("Box Collider 2D", entity,
			[](BoxCollider2DComponent& component)
			{
				ImGui::DragFloat2("Offset", glm::value_ptr(component.offset));
				ImGui::DragFloat2("Size", glm::value_ptr(component.size));
				ImGui::DragFloat("Density", &component.density, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Friction", &component.friction, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Restitution", &component.restitution, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Restitution Threshold", &component.restitutionThreshold, 0.01f, 0.0f);
			});
		
		drawComponent<CircleCollider2DComponent>("Circle Collider 2D", entity,
			[](CircleCollider2DComponent& component)
			{
				ImGui::DragFloat2("Offset", glm::value_ptr(component.offset));
				ImGui::DragFloat("Radius", &component.radius);
				ImGui::DragFloat("Density", &component.density, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Friction", &component.friction, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Restitution", &component.restitution, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Restitution Threshold", &component.restitutionThreshold, 0.01f, 0.0f);
			});
	}
}