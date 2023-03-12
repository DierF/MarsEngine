#include "pch.h"

#include "VertexArray.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace MarsEngine {

	VertexArray* VertexArray::create()
	{
		switch (Renderer::getAPI())
		{
		case RendererAPI::API::None:
			ME_CORE_ASSERT(false, "RendererAPI:None!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return new OpenGLVertexArray();
		default:
			break;
		}

		ME_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}