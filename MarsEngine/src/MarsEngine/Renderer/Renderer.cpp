#include "pch.h"

#include "Renderer.h"
#include "RenderCommand.h"

namespace MarsEngine {

	void Renderer::beginScene()
	{
	}

	void Renderer::endScene()
	{
	}

	void Renderer::submit(std::shared_ptr<VertexArray> const& vertexArray)
	{
		vertexArray->bind();
		RenderCommand::drawIndexed(vertexArray);
	}
}