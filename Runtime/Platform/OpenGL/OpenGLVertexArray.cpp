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
		ME_PROFILE_FUNCTION();

		glCreateVertexArrays(1, &m_rendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		ME_PROFILE_FUNCTION();

		glDeleteVertexArrays(1, &m_rendererID);
	}

	void OpenGLVertexArray::bind() const
	{
		ME_PROFILE_FUNCTION();

		glBindVertexArray(m_rendererID);
	}

	void OpenGLVertexArray::unbind() const
	{
		ME_PROFILE_FUNCTION();

		glBindVertexArray(0);
	}

	void OpenGLVertexArray::addVertexBuffer(Ref<VertexBuffer> const& vertexBuffer)
	{
		ME_CORE_ASSERT(vertexBuffer->getLayout().getElement().size(), "VertexBuffer has no layout!");

		glBindVertexArray(m_rendererID);
		vertexBuffer->bind();

		auto const& layout = vertexBuffer->getLayout();
		for (auto const& element : layout)
		{
			switch (element.m_type)
			{
			case ShaderDataType::Float:
			case ShaderDataType::Float2:
			case ShaderDataType::Float3:
			case ShaderDataType::Float4:
			{
				glEnableVertexAttribArray(m_vertexBufferIndex);
				glVertexAttribPointer(
					m_vertexBufferIndex,
					element.getComponentCount(),
					ShaderDataTypeToOpenGLBaseType(element.m_type),
					element.m_normalized ? GL_TRUE : GL_FALSE,
					layout.getStride(),
					(void const*)element.m_offset);
				++m_vertexBufferIndex;
				break;
			}
			case ShaderDataType::Int:
			case ShaderDataType::Int2:
			case ShaderDataType::Int3:
			case ShaderDataType::Int4:
			case ShaderDataType::Bool:
			{
				glEnableVertexAttribArray(m_vertexBufferIndex);
				glVertexAttribIPointer(
					m_vertexBufferIndex,
					element.getComponentCount(),
					ShaderDataTypeToOpenGLBaseType(element.m_type),
					layout.getStride(),
					(void const*)element.m_offset);
				++m_vertexBufferIndex;
				break;
			}
			case ShaderDataType::Matrix3:
			case ShaderDataType::Matrix4:
			{
				uint8_t count = element.getComponentCount();
				for (uint8_t i = 0; i < count; ++i)
				{
					glEnableVertexAttribArray(m_vertexBufferIndex);
					glVertexAttribPointer(
						m_vertexBufferIndex,
						count,
						ShaderDataTypeToOpenGLBaseType(element.m_type),
						element.m_normalized ? GL_TRUE : GL_FALSE,
						layout.getStride(),
						(void const*)(element.m_offset + sizeof(float) * count * i));
					glVertexAttribDivisor(m_vertexBufferIndex, 1);
					++m_vertexBufferIndex;
				}
				break;
			}
			default:
				ME_CORE_ASSERT(false, "unknown shaderType");
			}
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