#pragma once

#include "MarsEngine/Renderer/RendererAPI.h"

namespace MarsEngine {

	class OpenGLRendererAPI : public RendererAPI {

	public:
		virtual void init() override;

		virtual void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

		virtual void setClearColor(glm::vec4 const& color) override;

		virtual void clear() override;

		virtual void drawIndexed(Ref<VertexArray> const& vertexArray, uint32_t indexCount = 0) override;
	};
}