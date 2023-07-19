#pragma once

#include <vector>
#include <string>

namespace MarsEngine {

	enum class ShaderDataType
	{
		None = 0, Float, Float2, Float3, Float4, Matrix3, Matrix4, Int, Int2, Int3, Int4, Bool
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type) {

		switch (type)
		{
		case MarsEngine::ShaderDataType::Float:
			return 4;
		case MarsEngine::ShaderDataType::Float2:
			return 4 * 2;
		case MarsEngine::ShaderDataType::Float3:
			return 4 * 3;
		case MarsEngine::ShaderDataType::Float4:
			return 4 * 4;
		case MarsEngine::ShaderDataType::Matrix3:
			return 4 * 3 * 3;
		case MarsEngine::ShaderDataType::Matrix4:
			return 4 * 4 * 4;
		case MarsEngine::ShaderDataType::Int:
			return 4;
		case MarsEngine::ShaderDataType::Int2:
			return 4 * 2;
		case MarsEngine::ShaderDataType::Int3:
			return 4 * 3;
		case MarsEngine::ShaderDataType::Int4:
			return 4 * 4;
		case MarsEngine::ShaderDataType::Bool:
			return 1;
		default:
			break;
		}

		ME_CORE_ASSERT(false, "Unknown ShaderType!");
		return 0;
	}

	struct BufferElement {

		std::string m_name;
		ShaderDataType m_type;
		uint32_t m_size;
		uint32_t m_offset;
		bool m_normalized;

		BufferElement() {}

		BufferElement(ShaderDataType type, std::string const& name, bool normalized = false)
			: m_name(name), m_type(type), m_size(ShaderDataTypeSize(type)), m_offset(0),
			m_normalized(normalized) {}

		uint32_t getComponentCount() const {

			switch (m_type)
			{
			case MarsEngine::ShaderDataType::Float:
				return 1;
			case MarsEngine::ShaderDataType::Float2:
				return 2;
			case MarsEngine::ShaderDataType::Float3:
				return 3;
			case MarsEngine::ShaderDataType::Float4:
				return 4;
			case MarsEngine::ShaderDataType::Matrix3:
				return 3 * 3;
			case MarsEngine::ShaderDataType::Matrix4:
				return 4 * 4;
			case MarsEngine::ShaderDataType::Int:
				return 1;
			case MarsEngine::ShaderDataType::Int2:
				return 2;
			case MarsEngine::ShaderDataType::Int3:
				return 3;
			case MarsEngine::ShaderDataType::Int4:
				return 4;
			case MarsEngine::ShaderDataType::Bool:
				return 1;
			default:
				break;
			}
			return 0;
		}
	};

	class BufferLayout {

	public:
		BufferLayout() {}

		BufferLayout(std::initializer_list<BufferElement> const& element)
			: m_element(element) {
			calculateOffsetAndStride();
		}

		inline uint32_t getStride() const { return m_stride; }

		inline std::vector<BufferElement> const& getElement() const { return m_element; }

		auto begin() { return m_element.begin(); }

		auto end() { return m_element.end(); }

		auto begin() const { return m_element.begin(); }

		auto end() const { return m_element.end(); }

	private:
		void calculateOffsetAndStride() {
			uint32_t offset = 0;
			m_stride = 0;
			for (auto& element : m_element) {
				element.m_offset = offset;
				offset += element.m_size;
				m_stride += element.m_size;
			}
		}

	private:
		std::vector<BufferElement> m_element;
		uint32_t m_stride = 0;
	};

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {}

		virtual void bind() const = 0;

		virtual void unbind() const = 0;

		virtual void setData(void const* data, uint32_t size) = 0;

		virtual BufferLayout const& getLayout() const = 0;

		virtual void setLayout(BufferLayout const& layout) = 0;
		
		static Ref<VertexBuffer> create(uint32_t size);

		static Ref<VertexBuffer> create(float* vertices, uint32_t size);
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {}

		virtual void bind() const = 0;

		virtual void unbind() const = 0;

		virtual uint32_t getCount() const = 0;

		static Ref<IndexBuffer> create(uint32_t* indices, uint32_t count);
	};
}