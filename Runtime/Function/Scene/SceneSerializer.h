#pragma once

#include "Scene.h"

namespace MarsEngine
{
	class SceneSerializer
	{
	public:
		SceneSerializer(Ref<Scene> const& scene);

		void serialize(std::string const& filepath);

		void serializeRuntime(std::string const& filepath);

		bool deserialize(std::string const& filepath);

		bool deserializeRuntime(std::string const& filepath);

	private:
		Ref<Scene> m_scene;
	};
}