#include "pch.h"
#include "RenderCommand.h"
#include "RendererAPI.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace MarsEngine {

	RendererAPI* RenderCommand::s_rendererAPI = new OpenGLRendererAPI();
}