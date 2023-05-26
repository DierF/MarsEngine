#include "pch.h"

#include "Texture.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace MarsEngine
{
	Ref<Texture2D> Texture2D::create(uint32_t width, uint32_t height)
	{
		switch (Renderer::getAPI())
		{
		case RendererAPI::API::None:
			ME_CORE_ASSERT(false, "RendererAPI:None!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return createRef<OpenGLTexture2D>(width, height);
		default:
			break;
		}

		ME_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::create(std::string const& path)
	{
		switch (Renderer::getAPI())
		{
		case RendererAPI::API::None:
			ME_CORE_ASSERT(false, "RendererAPI:None!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return createRef<OpenGLTexture2D>(path);
		default:
			break;
		}

		ME_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}