#pragma once

#include "RendererAPI.h"

namespace MarsEngine {

	class RenderCommand {

	public:
		inline static void init()
		{
			s_rendererAPI->init();
		}

		inline static void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_rendererAPI->setViewport(x, y, width, height);
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