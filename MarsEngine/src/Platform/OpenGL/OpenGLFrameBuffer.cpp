#include "pch.h"

#include "OpenGLFrameBuffer.h"

#include "glad/glad.h"

namespace MarsEngine
{
	static uint32_t const s_maxFramebufferSize = 8192;

	namespace Util
	{
		static GLenum textureTarget(bool multisampled)
		{
			return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
		}

		static void createTextures(bool multisampled, uint32_t* outID, uint32_t count)
		{
			glCreateTextures(textureTarget(multisampled), count, outID);
		}

		static void bindTexture(bool multisampled, uint32_t id)
		{
			glBindTexture(textureTarget(multisampled), id);
		}

		static void attachColorTexture(uint32_t id, int samples, GLenum internalFormat, GLenum format, uint32_t width, uint32_t height, int index)
		{
			bool multisampled = samples > 1;
			if (multisampled)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat,
					width, height, GL_FALSE);
			}
			else
			{
				glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format,
					GL_UNSIGNED_BYTE, nullptr);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index,
				textureTarget(multisampled), id, 0);
		}
		
		static void attachDepthTexture(uint32_t id, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height)
		{
			bool multisampled = samples > 1;
			if (multisampled)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
			}
			else
			{
				glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, textureTarget(multisampled), id, 0);
		}

		static bool isDepthFormat(FramebufferTextureFormat format)
		{
			switch (format)
			{
			case FramebufferTextureFormat::DEPTH24STENCIL8: return true;
			}
			return false;
		}
	}

	OpenGLFramebuffer::OpenGLFramebuffer(FramebufferSpecification const& spec)
		: m_specification(spec)
	{
		for (auto spec : m_specification.attachment.attachment)
		{
			if (!Util::isDepthFormat(spec.textureFormat))
			{
				m_colorAttachmentSpecifications.emplace_back(spec);
			}
			else
			{
				m_depthAttachmentSpecification = spec;
			}
		}

		invalidate();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		glDeleteFramebuffers(1, &m_rendererID);
		glDeleteTextures(m_colorAttachments.size(), m_colorAttachments.data());
		glDeleteTextures(1, &m_depthAttachment);
	}

	void OpenGLFramebuffer::invalidate()
	{
		if (m_rendererID)
		{
			glDeleteFramebuffers(1, &m_rendererID);
			glDeleteTextures(m_colorAttachments.size(), m_colorAttachments.data());
			glDeleteTextures(1, &m_depthAttachment);

			m_colorAttachments.clear();
			m_depthAttachment = 0;
		}

		glCreateFramebuffers(1, &m_rendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID);

		bool multisample = m_specification.samples > 1;

		if (!m_colorAttachmentSpecifications.empty())
		{
			m_colorAttachments.resize(m_colorAttachmentSpecifications.size());
			Util::createTextures(multisample, m_colorAttachments.data(), m_colorAttachments.size());

			for (size_t i = 0; i < m_colorAttachmentSpecifications.size(); ++i)
			{
				Util::bindTexture(multisample, m_colorAttachments[i]);
				switch (m_colorAttachmentSpecifications[i].textureFormat)
				{
				case FramebufferTextureFormat::RGBA8:
				{
					Util::attachColorTexture(m_colorAttachments[i], m_specification.samples,
						GL_RGB8, GL_RGBA, m_specification.width, m_specification.height, i);
					break;
				}
				case FramebufferTextureFormat::RED_INTEGER:
				{
					Util::attachColorTexture(m_colorAttachments[i], m_specification.samples,
						GL_R32I, GL_RED_INTEGER, m_specification.width, m_specification.height, i);
					break;
				}
				}
			}
		}
		if (m_depthAttachmentSpecification.textureFormat != FramebufferTextureFormat::None)
		{
			Util::createTextures(multisample, &m_depthAttachment, 1);
			Util::bindTexture(multisample, m_depthAttachment);
			switch (m_depthAttachmentSpecification.textureFormat)
			{
			case FramebufferTextureFormat::DEPTH24STENCIL8:
			{
				Util::attachDepthTexture(m_depthAttachment, m_specification.samples,
					GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_specification.width,
					m_specification.height);
				break;
			}
			}
		}
		if (m_colorAttachments.size() > 1)
		{
			ME_CORE_ASSERT(m_colorAttachments.size() <= 4);
			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,
				GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers(m_colorAttachments.size(), buffers);
		}
		else if (m_colorAttachments.empty())
		{
			glDrawBuffer(GL_NONE);
		}
		
		ME_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE,
			"Framebuffer is incomplete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID);
		glViewport(0, 0, m_specification.width, m_specification.height);
	}

	void OpenGLFramebuffer::unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::resize(uint32_t width, uint32_t height)
	{
		if (width == 0 || height == 0 || width > s_maxFramebufferSize || height > s_maxFramebufferSize)
		{
			ME_CORE_WARN("Attempt to resize framebuffer to {}, {}", width, height);
			return;
		}

		m_specification.width = width;
		m_specification.height = height;

		invalidate();
	}

	int OpenGLFramebuffer::readPixel(uint32_t attachmentIndex, int x, int y)
	{
		ME_CORE_ASSERT(attachmentIndex < m_colorAttachments.size());
		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
		int pixelData;
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
		return pixelData;
	}
}