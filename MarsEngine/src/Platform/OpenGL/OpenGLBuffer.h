#pragma once

#include "MarsEngine/Renderer/Buffer.h"

namespace MarsEngine {

	class OpenGLVertexBuffer : public VertexBuffer {

	public:
		OpenGLVertexBuffer(float* vertices, uint32_t size);

		virtual ~OpenGLVertexBuffer();

		virtual void bind() const override;

		virtual void unbind() const override;

	private:
		uint32_t m_rendererID;
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