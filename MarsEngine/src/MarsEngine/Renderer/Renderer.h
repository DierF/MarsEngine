#pragma once

#include "RenderCommand.h"
#include "RendererAPI.h"
#include "OrthographicCamera.h"
#include "Shader.h"

namespace MarsEngine {

	class Renderer {

	public:
		static void beginScene(OrthographicCamera& camera);

		static void endScene();

		static void submit(std::shared_ptr<Shader> const& shader, std::shared_ptr<VertexArray> const& vertexArray);

		inline static RendererAPI::API getAPI() { return RendererAPI::getAPI(); }

	private:
		struct SceneData {

			glm::mat4 ViewProjectionMatrix;

		};

		static SceneData* m_sceneData;
	};
}