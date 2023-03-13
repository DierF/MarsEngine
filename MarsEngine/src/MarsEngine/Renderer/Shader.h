#pragma once

#include <string>

#include "glm/glm.hpp"

namespace MarsEngine {

	class Shader {

	public:
		Shader(std::string const& vertexSrc, std::string const& fragmentSrc);

		~Shader();

		void bind() const;

		void unbind() const;

		void uploadUniformMat4(std::string const& name, glm::mat4 const& matrix);

	private:
		uint32_t m_rendererID;
	};
}