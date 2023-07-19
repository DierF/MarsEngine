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

		virtual void addVertexBuffer(Ref<VertexBuffer> const& vertexBuffer) = 0;

		virtual void setIndexBuffer(Ref<IndexBuffer> const& indexBuffer) = 0;

		virtual std::vector<Ref<VertexBuffer>> const& getVertexBuffer() const = 0;

		virtual Ref<IndexBuffer> const& getIndexBuffer() const = 0;

		static Ref<VertexArray> create();

	};
}