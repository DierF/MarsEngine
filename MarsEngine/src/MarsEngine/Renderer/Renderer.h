#pragma once

#include "RenderCommand.h"
#include "RendererAPI.h"
#include "OrthographicCamera.h"
#include "Shader.h"

namespace MarsEngine {

	class Renderer {

	public:
		static void init();

		static void beginScene(OrthographicCamera& camera);

		static void endScene();

		static void submit(Ref<Shader> const& shader, Ref<VertexArray> const& vertexArray, glm::mat4 const& transform = glm::mat4(1.0f));

		inline static RendererAPI::API getAPI() { return RendererAPI::getAPI(); }

	private:
		struct SceneData {

			glm::mat4 ViewProjectionMatrix;

		};

		static SceneData* m_sceneData;
	};
}