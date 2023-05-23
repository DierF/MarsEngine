#pragma once

#include "glm/glm.hpp"
#include <memory>
#include "VertexArray.h"

namespace MarsEngine {

	class RendererAPI {

	public:
		enum class API
		{
			None = 0, OpenGL = 1
		};

	public:
		virtual void init() = 0;

		virtual void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

		virtual void setClearColor(glm::vec4 const& color) = 0;

		virtual void clear() = 0;

		virtual void drawIndexed(Ref<VertexArray> const& vertexArray) = 0;

		inline static API getAPI() { return s_API; }

	private:
		static API s_API;
	};
}