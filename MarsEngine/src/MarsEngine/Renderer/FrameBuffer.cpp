#include "pch.h"

#include "FrameBuffer.h"
#include "MarsEngine/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLFrameBuffer.h"

namespace MarsEngine
{
	Ref<Framebuffer> Framebuffer::create(FramebufferSpecification const& spec)
	{
		switch (Renderer::getAPI())
		{
		case RendererAPI::API::None:
			ME_CORE_ASSERT(false, "RendererAPI::None is not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return createRef<OpenGLFramebuffer>(spec);
		default:
			break;
		}
		ME_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
