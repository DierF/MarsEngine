#pragma once

#include "MarsEngine/Renderer/Buffer.h"
#include <vector>
#include <memory>

namespace MarsEngine {

	class VertexArray {

	public:
		virtual ~VertexArray() {}

		virtual void bind() const = 0;

		virtual void unbind() const = 0;

		virtual void addVertexBuffer(std::shared_ptr<VertexBuffer> const& vertexBuffer) = 0;

		virtual void setIndexBuffer(std::shared_ptr<IndexBuffer> const& indexBuffer) = 0;

		virtual std::vector<std::shared_ptr<VertexBuffer>> const& getVertexBuffer() const = 0;

		virtual std::shared_ptr<IndexBuffer> const& getIndexBuffer() const = 0;

		static VertexArray* create();

	};
}