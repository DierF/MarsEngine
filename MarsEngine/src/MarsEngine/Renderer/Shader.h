#pragma once

#include <string>

namespace MarsEngine {

	class Shader {

	public:
		Shader(std::string const& vertexSrc, std::string const& fragmentSrc);

		~Shader();

		void bind() const;

		void unbind() const;

	private:
		uint32_t m_rendererID;
	};
}