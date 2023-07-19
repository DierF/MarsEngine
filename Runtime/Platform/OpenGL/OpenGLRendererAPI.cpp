#include "pch.h"

#include "OpenGLRendererAPI.h"
#include "glad/glad.h"

namespace MarsEngine {
	void OpenGLRendererAPI::init()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LINE_SMOOTH);
	}

	void OpenGLRendererAPI::setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::setClearColor(glm::vec4 const& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::drawIndexed(Ref<VertexArray> const& vertexArray, uint32_t indexCount)
	{
		vertexArray->bind();
		uint32_t count = indexCount != 0 ? indexCount : vertexArray->getIndexBuffer()->getCount();
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}
	void OpenGLRendererAPI::drawLines(Ref<VertexArray> const& vertexArray, uint32_t vertexCount)
	{
		vertexArray->bind();
		glDrawArrays(GL_LINES, 0, vertexCount);
	}

	void OpenGLRendererAPI::setLineWidth(float width)
	{
		glLineWidth(width);
	}
}