#include "pch.h"

#include "SceneSerializer.h"
#include "Entity.h"
#include "Component.h"
#include "yaml-cpp/yaml.h"

namespace YAML
{
	template<>
	struct convert<glm::vec3>
	{
		static Node encode(glm::vec3 const& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(Node const& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
			{
				return false;
			}
			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(glm::vec4 const& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}

		static bool decode(Node const& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
			{
				return false;
			}
			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};
}

namespace MarsEngine
{
	YAML::Emitter& operator<<(YAML::Emitter& out, glm::vec3 const& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}
	YAML::Emitter& operator<<(YAML::Emitter& out, glm::vec4 const& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	SceneSerializer::SceneSerializer(Ref<Scene> const& scene)
		: m_scene(scene)
	{
	}

	static void serializeEntity(YAML::Emitter& out, Entity entity)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Entity";
		out << YAML::Value << "1145141919818"; //TODO: Entity ID goes here

		if (entity.hasComponment<TagComponent>())
		{
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap;

			auto& tag = entity.getComponent<TagComponent>().tag;
			out << YAML::Key << "Tag" << YAML::Value << tag;

			out << YAML::EndMap;
		}

		if (entity.hasComponment<TransformComponent>())
		{
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap;

			auto& tc = entity.getComponent<TransformComponent>();
			out << YAML::Key << "Translation" << YAML::Value << tc.translation;
			out << YAML::Key << "Rotation" << YAML::Value << tc.rotation;
			out << YAML::Key << "Scale" << YAML::Value << tc.scale;

			out << YAML::EndMap;
		}

		if (entity.hasComponment<CameraComponent>())
		{
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap;

			auto& cc = entity.getComponent<CameraComponent>();
			auto& camera = cc.camera;

			out << YAML::Key << "Camera" << YAML::Value;
			out << YAML::BeginMap;
			out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.getProjectionType();
			out << YAML::Key << "PerspectiveFOV" << YAML::Value << camera.getPerspectiveVerticalFOV();
			out << YAML::Key << "PerspectiveNear" << YAML::Value << camera.getPerspectiveNearClip();
			out << YAML::Key << "PerspectiveFar" << YAML::Value << camera.getPerspectiveFarClip();
			out << YAML::Key << "OrthographicSize" << YAML::Value << camera.getOrthographicSize();
			out << YAML::Key << "OrthographicNear" << YAML::Value << camera.getOrthographicNearClip();
			out << YAML::Key << "OrthographicFar" << YAML::Value << camera.getOrthographicFarClip();
			out << YAML::EndMap;

			out << YAML::Key << "Primary" << YAML::Value << cc.primary;
			out << YAML::Key << "FixedAspectRatio" << YAML::Value << cc.fixedAspectRatio;

			out << YAML::EndMap;
		}

		if (entity.hasComponment<SpriteRendererComponent>())
		{
			out << YAML::Key << "SpriteRendererComponent";
			out << YAML::BeginMap;

			auto& sc = entity.getComponent<SpriteRendererComponent>();
			out << YAML::Key << "Color" << YAML::Value << sc.color;

			out << YAML::EndMap;
		}

		out << YAML::EndMap;
	}

	void SceneSerializer::serialize(std::string const& filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled";
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		m_scene->m_registry.each([&](auto entityID)
			{
				Entity entity = { entityID, m_scene.get() };
				if (!entity)
				{
					return;
				}
				serializeEntity(out, entity);
			});
		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();
	}

	void SceneSerializer::serializeRuntime(std::string const& filepath)
	{
		ME_CORE_ASSERT(false, "not implemented");
	}

	bool SceneSerializer::deserialize(std::string const& filepath)
	{
		std::ifstream ifs(filepath);
		std::stringstream ss;
		ss << ifs.rdbuf();

		YAML::Node data = YAML::Load(ss.str());
		if (!data["Scene"])
		{
			return false;
		}
		std::string sceneName = data["Scene"].as<std::string>();
		ME_CORE_TRACE("Deserializing scene '{}'", sceneName);

		auto entities = data["Entities"];
		if (entities)
		{
			for (auto entity : entities)
			{
				uint64_t uuid = entity["Entity"].as<uint64_t>();

				std::string name;
				auto tagC = entity["TagComponent"];
				if (tagC)
				{
					name = tagC["Tag"].as<std::string>();
				}
				ME_CORE_TRACE("Deserializing entity with ID = {}, name = {}", uuid, name);

				Entity deserializedEntity = m_scene->createEntity(name);

				auto transformC = entity["TransformComponent"];
				if (transformC)
				{
					auto& tc = deserializedEntity.getComponent<TransformComponent>();
					tc.translation = transformC["Translation"].as<glm::vec3>();
					tc.rotation = transformC["Rotation"].as<glm::vec3>();
					tc.scale = transformC["Scale"].as<glm::vec3>();
				}

				auto cameraC = entity["CameraComponent"];
				if (cameraC)
				{
					auto& cc = deserializedEntity.addComponent<CameraComponent>();

					auto& cameraProps = cameraC["Camera"];
					cc.camera.setProjectionType((SceneCamera::ProjectionType)cameraProps["ProjectionType"].as<int>());
					
					cc.camera.setPerspectiveVerticalFOV(cameraProps["PerspectiveFOV"].as<float>());
					cc.camera.setPerspectiveNearClip(cameraProps["PerspectiveNear"].as<float>());
					cc.camera.setPerspectiveFarClip(cameraProps["PerspectiveFar"].as<float>());

					cc.camera.setOrthographicSize(cameraProps["OrthographicSize"].as<float>());
					cc.camera.setOrthographicNearClip(cameraProps["OrthographicNear"].as<float>());
					cc.camera.setOrthographicFarClip(cameraProps["OrthographicFar"].as<float>());

					cc.primary = cameraC["Primary"].as<bool>();
					cc.fixedAspectRatio = cameraC["FixedAspectRatio"].as<bool>();
				}

				auto spriteRendererC = entity["SpriteRendererComponent"];
				if (spriteRendererC)
				{
					auto& src = deserializedEntity.addComponent<SpriteRendererComponent>();
					src.color = spriteRendererC["Color"].as<glm::vec4>();
				}
			}
		}
		return true;
	}

	bool SceneSerializer::deserializeRuntime(std::string const& filepath)
	{
		ME_CORE_ASSERT(false, "not implemented");
		return false;
	}
}