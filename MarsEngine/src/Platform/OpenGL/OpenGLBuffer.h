#pragma once

#include "MarsEngine/Renderer/Buffer.h"

namespace MarsEngine {

	class OpenGLVertexBuffer : public VertexBuffer {

	public:
		OpenGLVertexBuffer(uint32_t size);

		OpenGLVertexBuffer(float* vertices, uint32_t size);

		virtual ~OpenGLVertexBuffer();

		virtual void bind() const override;

		virtual void unbind() const override;

		virtual void setData(void const* data, uint32_t size) override;

		virtual BufferLayout const& getLayout() const override { return m_layout; }

		virtual void setLayout(BufferLayout const& layout) override { m_layout = layout; }

	private:
		uint32_t m_rendererID;
		BufferLayout m_layout;
	};

	class OpenGLIndexBuffer : public IndexBuffer {

	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);

		virtual ~OpenGLIndexBuffer();

		virtual void bind() const override;

		virtual void unbind() const override;

		virtual uint32_t getCount() const override { return m_count; }

	private:
		uint32_t m_rendererID;
		uint32_t m_count;
	};
}