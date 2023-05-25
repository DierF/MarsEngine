#pragma once

#include "MarsEngine/Renderer/Shader.h"
#include "glm/glm.hpp"

typedef unsigned int GLenum;

namespace MarsEngine {

	class OpenGLShader : public Shader {

	public:
		OpenGLShader(std::string const& filepath);

		OpenGLShader(std::string const& name, std::string const& vertexSrc, std::string const& fragmentSrc);

		virtual ~OpenGLShader();

		virtual void bind() const;

		virtual void unbind() const;

		virtual void setInt(std::string const& name, int value) override;

		virtual void setFloat3(std::string const& name, glm::vec3 const& value) override;

		virtual void setFloat4(std::string const& name, glm::vec4 const& value) override;

		virtual void setMat4(std::string const& name, glm::mat4 const& value) override;

		virtual std::string const& getName() const override { return m_name; }

		void uploadUniformInt(std::string const& name, int value);

		void uploadUniformFloat(std::string const& name, float value);

		void uploadUniformFloat2(std::string const& name, glm::vec2 const& value);

		void uploadUniformFloat3(std::string const& name, glm::vec3 const& value);

		void uploadUniformFloat4(std::string const& name, glm::vec4 const& value);

		void uploadUniformMat3(std::string const& name, glm::mat3 const& matrix);

		void uploadUniformMat4(std::string const& name, glm::mat4 const& matrix);

	private:
		std::string readFile(std::string const& filepath);

		std::unordered_map<GLenum, std::string> preprocess(std::string const& source);

		void compile(std::unordered_map<GLenum, std::string> const& shaderSources);

	private:
		uint32_t m_rendererID;

		std::string m_name;
	};
}