#include "pch.h"
#include "OpenGLShader.h"
#include <fstream>
#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"

namespace MarsEngine {

	static GLenum ShaderTypeFromString(std::string const& type)
	{
		if (type == "vertex")
		{
			return GL_VERTEX_SHADER;
		}
		else if (type == "fragment" || type == "pixel")
		{
			return GL_FRAGMENT_SHADER;
		}
		else
		{
			ME_CORE_ASSERT(false, "unknown shader type");
			return 0;
		}
	}

	OpenGLShader::OpenGLShader(std::string const& filepath)
	{
		ME_PROFILE_FUNCTION();

		std::string source = readFile(filepath);
		auto shaderSources = preprocess(source);
		compile(shaderSources);

		auto lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filepath.rfind('.');
		auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
		m_name = filepath.substr(lastSlash, count);
	}

	OpenGLShader::OpenGLShader(std::string const& name, std::string const& vertexSrc, std::string const& fragmentSrc)
		: m_name(name)
	{
		ME_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;
		compile(sources);
	}

	OpenGLShader::~OpenGLShader()
	{
		ME_PROFILE_FUNCTION();

		glDeleteProgram(m_rendererID);
	}

	std::string OpenGLShader::readFile(std::string const& filepath)
	{
		ME_PROFILE_FUNCTION();

		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		}
		else
		{
			ME_CORE_ERROR("could not open file '{0}'", filepath);
		}
		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::preprocess(std::string const& source)
	{
		ME_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> shaderSources;

		char const* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			ME_CORE_ASSERT(eol != std::string::npos, "shader syntax error");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			ME_CORE_ASSERT(ShaderTypeFromString(type) != 0, "invalid shader type specified");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);
			shaderSources[ShaderTypeFromString(type)] =
				source.substr(nextLinePos,
					pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
		}
		return shaderSources;
	}

	void OpenGLShader::compile(std::unordered_map<GLenum, std::string> const& shaderSources)
	{
		ME_PROFILE_FUNCTION();

		GLuint program = glCreateProgram();
		ME_CORE_ASSERT(shaderSources.size() != 2, "Only 2 shaders for now");
		std::array<GLenum, 2> glShaderIDs;
		int glShaderIDIndex = 0;
		for (auto& [type, source] : shaderSources)
		{
			GLuint shader = glCreateShader(type);

			// Send the vertex shader source code to GL
			// Note that std::string's .c_str is NULL character terminated.
			const GLchar* sourceCStr = source.c_str();
			glShaderSource(shader, 1, &sourceCStr, 0);

			// Compile the vertex shader
			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				// Use the infoLog as you see fit.
				ME_CORE_ERROR("{0}", infoLog.data());
				ME_CORE_ASSERT(false, "shader compliation failure!");
				break;
			}
			glAttachShader(program, shader);
			glShaderIDs[glShaderIDIndex++] = shader;
		}

		// Read our shaders into the appropriate buffers
		//std::string const& vertexSource = vertexSrc;// Get source code for vertex shader.
		//std::string const& fragmentSource = fragmentSrc;// Get source code for fragment shader.


		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);
			// Don't leak shaders either.
			for (auto id : glShaderIDs)
			{
				glDeleteShader(id);
			}

			// Use the infoLog as you see fit.

			ME_CORE_ERROR("{0}", infoLog.data());
			ME_CORE_ASSERT(false, "shader link failure!");
			return;
		}

		for (auto id : glShaderIDs)
		{
			glDeleteShader(id);
		}

		m_rendererID = program;
	}

	void OpenGLShader::bind() const
	{
		ME_PROFILE_FUNCTION();

		glUseProgram(m_rendererID);
	}

	void OpenGLShader::unbind() const
	{
		ME_PROFILE_FUNCTION();

		glUseProgram(0);
	}

	void OpenGLShader::setInt(std::string const& name, int value)
	{
		ME_PROFILE_FUNCTION();

		uploadUniformInt(name, value);
	}

	void OpenGLShader::setIntArray(std::string const& name, int* values, uint32_t count)
	{
		uploadUniformIntArray(name, values, count);
	}

	void OpenGLShader::setFloat(std::string const& name, float value)
	{
		ME_PROFILE_FUNCTION();

		uploadUniformFloat(name, value);
	}

	void OpenGLShader::setFloat3(std::string const& name, glm::vec3 const& value)
	{
		ME_PROFILE_FUNCTION();

		uploadUniformFloat3(name, value);
	}

	void OpenGLShader::setFloat4(std::string const& name, glm::vec4 const& value)
	{
		ME_PROFILE_FUNCTION();

		uploadUniformFloat4(name, value);
	}

	void OpenGLShader::setMat4(std::string const& name, glm::mat4 const& value)
	{
		ME_PROFILE_FUNCTION();

		uploadUniformMat4(name, value);
	}

	void OpenGLShader::uploadUniformInt(std::string const& name, int value)
	{
		GLint location = glGetUniformLocation(m_rendererID, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShader::uploadUniformIntArray(std::string const& name, int* values, uint32_t count)
	{
		GLint location = glGetUniformLocation(m_rendererID, name.c_str());
		glUniform1iv(location, count, values);
	}

	void OpenGLShader::uploadUniformFloat(std::string const& name, float value)
	{
		GLint location = glGetUniformLocation(m_rendererID, name.c_str());
		glUniform1f(location, value);
	}

	void OpenGLShader::uploadUniformFloat2(std::string const& name, glm::vec2 const& value)
	{
		GLint location = glGetUniformLocation(m_rendererID, name.c_str());
		glUniform2f(location, value.x, value.y);
	}

	void OpenGLShader::uploadUniformFloat3(std::string const& name, glm::vec3 const& value)
	{
		GLint location = glGetUniformLocation(m_rendererID, name.c_str());
		glUniform3f(location, value.x, value.y, value.z);
	}

	void OpenGLShader::uploadUniformFloat4(std::string const& name, glm::vec4 const& value)
	{
		GLint location = glGetUniformLocation(m_rendererID, name.c_str());
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::uploadUniformMat3(std::string const& name, glm::mat3 const& matrix)
	{
		GLint location = glGetUniformLocation(m_rendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::uploadUniformMat4(std::string const& name, glm::mat4 const& matrix)
	{
		GLint location = glGetUniformLocation(m_rendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}
}