#pragma once

#include "MarsEngine/Renderer/Shader.h"
#include "glm/glm.hpp"

namespace MarsEngine {

	class OpenGLShader : public Shader {

	public:
		OpenGLShader(std::string const& vertexSrc, std::string const& fragmentSrc);

		virtual ~OpenGLShader();

		virtual void bind() const;

		virtual void unbind() const;

		void uploadUniformInt(std::string const& name, int value);

		void uploadUniformFloat(std::string const& name, float value);

		void uploadUniformFloat2(std::string const& name, glm::vec2 const& value);

		void uploadUniformFloat3(std::string const& name, glm::vec3 const& value);

		void uploadUniformFloat4(std::string const& name, glm::vec4 const& value);

		void uploadUniformMat3(std::string const& name, glm::mat3 const& matrix);

		void uploadUniformMat4(std::string const& name, glm::mat4 const& matrix);

	private:
		uint32_t m_rendererID;
	};
}