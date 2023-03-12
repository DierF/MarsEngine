#pragma once

#include "MarsEngine/Renderer/RendererAPI.h"

namespace MarsEngine {

	class OpenGLRendererAPI : public RendererAPI {

	public:
		virtual void setClearColor(glm::vec4 const& color) override;

		virtual void clear() override;

		virtual void drawIndexed(std::shared_ptr<VertexArray> const& vertexArray) override;
	};
}