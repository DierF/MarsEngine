#pragma once

#include "MarsEngine/Renderer/VertexArray.h"
#include <vector>
#include <memory>

namespace MarsEngine {

	class OpenGLVertexArray : public VertexArray {

	public:
		OpenGLVertexArray();

		virtual ~OpenGLVertexArray();

		virtual void bind() const override;

		virtual void unbind() const override;

		virtual void addVertexBuffer(std::shared_ptr<VertexBuffer> const& vertexBuffer) override;

		virtual void setIndexBuffer(std::shared_ptr<IndexBuffer> const& indexBuffer) override;

		virtual std::vector<std::shared_ptr<VertexBuffer>> const& getVertexBuffer() const override {
			return m_vertexBuffer;
		}

		virtual std::shared_ptr<IndexBuffer> const& getIndexBuffer() const override {
			return m_indexBuffer;
		}

	private:
		uint32_t m_rendererID;
		std::vector<std::shared_ptr<VertexBuffer>> m_vertexBuffer;
		std::shared_ptr<IndexBuffer> m_indexBuffer;
	};
}