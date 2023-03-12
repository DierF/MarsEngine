#include "pch.h"

#include "OpenGLRendererAPI.h"
#include "glad/glad.h"

namespace MarsEngine {

	void OpenGLRendererAPI::setClearColor(glm::vec4 const& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::drawIndexed(std::shared_ptr<VertexArray> const& vertexArray)
	{
		glDrawElements(GL_TRIANGLES, vertexArray->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
	}
}