#pragma once

#include "MarsEngine/Renderer/FrameBuffer.h"

namespace MarsEngine
{
	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(FramebufferSpecification const& spec);

		virtual ~OpenGLFramebuffer() override;

		void invalidate();

		virtual void bind() override;

		virtual void unbind() override;

		virtual void resize(uint32_t width, uint32_t height) override;

		virtual uint32_t getColorAttachmentRendererID() const override
		{
			return m_colorAttachment;
		}

		virtual FramebufferSpecification const& getSpecification() const override
		{
			return m_specification;
		}

	private:
		uint32_t m_rendererID = 0;
		uint32_t m_colorAttachment = 0, m_depthAttachment = 0;
		FramebufferSpecification m_specification;
	};
}