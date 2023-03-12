#pragma once

#include "RenderCommand.h"
#include "RendererAPI.h"

namespace MarsEngine {

	class Renderer {

	public:
		static void beginScene();

		static void endScene();

		static void submit(std::shared_ptr<VertexArray> const& vertexArray);

		inline static RendererAPI::API getAPI() { return RendererAPI::getAPI(); }
	};
}