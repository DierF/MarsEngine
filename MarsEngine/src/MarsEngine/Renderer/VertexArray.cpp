#include "pch.h"

#include "VertexArray.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace MarsEngine {

	Ref<VertexArray> VertexArray::create()
	{
		switch (Renderer::getAPI())
		{
		case RendererAPI::API::None:
			ME_CORE_ASSERT(false, "RendererAPI:None!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLVertexArray>();
		default:
			break;
		}

		ME_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}