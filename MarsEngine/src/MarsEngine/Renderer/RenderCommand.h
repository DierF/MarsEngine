#pragma once

#include "RendererAPI.h"

namespace MarsEngine {

	class RenderCommand {

	public:
		inline static void init()
		{
			s_rendererAPI->init();
		}

		inline static void setClearColor(glm::vec4 const& color) {
			s_rendererAPI->setClearColor(color);
		}

		inline static void clear() {
			s_rendererAPI->clear();
		}

		inline static void drawIndexed(std::shared_ptr<VertexArray> const& vertexArray) {
			s_rendererAPI->drawIndexed(vertexArray);
		}

	private:
		static RendererAPI* s_rendererAPI;
	};
}