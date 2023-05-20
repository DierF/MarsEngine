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

		virtual void addVertexBuffer(Ref<VertexBuffer> const& vertexBuffer) override;

		virtual void setIndexBuffer(Ref<IndexBuffer> const& indexBuffer) override;

		virtual std::vector<Ref<VertexBuffer>> const& getVertexBuffer() const override {
			return m_vertexBuffer;
		}

		virtual Ref<IndexBuffer> const& getIndexBuffer() const override {
			return m_indexBuffer;
		}

	private:
		uint32_t m_rendererID;
		std::vector<Ref<VertexBuffer>> m_vertexBuffer;
		Ref<IndexBuffer> m_indexBuffer;
	};
}