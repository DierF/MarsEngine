#include "pch.h"
#include "OpenGLVertexArray.h"

#include "glad/glad.h"

namespace MarsEngine {

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {

		switch (type)
		{
		case MarsEngine::ShaderDataType::Float:
			return GL_FLOAT;
		case MarsEngine::ShaderDataType::Float2:
			return GL_FLOAT;
		case MarsEngine::ShaderDataType::Float3:
			return GL_FLOAT;
		case MarsEngine::ShaderDataType::Float4:
			return GL_FLOAT;
		case MarsEngine::ShaderDataType::Matrix3:
			return GL_FLOAT;
		case MarsEngine::ShaderDataType::Matrix4:
			return GL_FLOAT;
		case MarsEngine::ShaderDataType::Int:
			return GL_INT;
		case MarsEngine::ShaderDataType::Int2:
			return GL_INT;
		case MarsEngine::ShaderDataType::Int3:
			return GL_INT;
		case MarsEngine::ShaderDataType::Int4:
			return GL_INT;
		case MarsEngine::ShaderDataType::Bool:
			return GL_BOOL;
		default:
			break;
		}
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &m_rendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray() {
		
		glDeleteVertexArrays(1, &m_rendererID);
	}

	void OpenGLVertexArray::bind() const
	{
		glBindVertexArray(m_rendererID);
	}

	void OpenGLVertexArray::unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::addVertexBuffer(Ref<VertexBuffer> const& vertexBuffer)
	{
		ME_CORE_ASSERT(vertexBuffer->getLayout().getElement().size(), "VertexBuffer has no layout!");

		glBindVertexArray(m_rendererID);
		vertexBuffer->bind();

		auto const& layout = vertexBuffer->getLayout();
		uint32_t index = 0;
		for (auto const& element : layout) {

			glEnableVertexAttribArray(index);
			glVertexAttribPointer(
				index,
				element.getComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.m_type),
				element.m_normalized ? GL_TRUE : GL_FALSE,
				layout.getStride(),
				(void const*)element.m_offset);
			++index;
		}

		m_vertexBuffer.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::setIndexBuffer(Ref<IndexBuffer> const& indexBuffer)
	{
		glBindVertexArray(m_rendererID);
		indexBuffer->bind();

		m_indexBuffer = indexBuffer;
	}
}