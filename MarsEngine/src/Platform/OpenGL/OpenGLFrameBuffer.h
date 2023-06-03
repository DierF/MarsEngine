#pragma once

#include "MarsEngine/Renderer/FrameBuffer.h"

namespace MarsEngine
{
	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(FramebufferSpecification const& spec);

		virtual ~OpenGLFramebuffer();

		void invalidate();

		virtual void bind() override;

		virtual void unbind() override;

		virtual uint32_t getColorAttachmentRendererID() const override
		{
			return m_colorAttachment;
		}

		virtual FramebufferSpecification const& getSpecification() const override
		{
			return m_specification;
		}

	private:
		uint32_t m_rendererID;
		uint32_t m_colorAttachment, m_depthAttachment;
		FramebufferSpecification m_specification;
	};
}