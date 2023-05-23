#include "pch.h"

#include "Shader.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace MarsEngine {
	Ref<Shader> Shader::create(std::string const& filepath)
	{
		switch (Renderer::getAPI())
		{
		case RendererAPI::API::None:
			ME_CORE_ASSERT(false, "RendererAPI:None!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLShader>(filepath);
		default:
			break;
		}

		ME_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Shader> Shader::create(std::string const& name, std::string const& vertexSrc, std::string const& fragmentSrc)
	{
		switch (Renderer::getAPI())
		{
		case RendererAPI::API::None:
			ME_CORE_ASSERT(false, "RendererAPI:None!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLShader>(name, vertexSrc, fragmentSrc);
		default:
			break;
		}

		ME_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	void ShaderLibrary::add(std::string const& name, Ref<Shader> const& shader)
	{
		ME_CORE_ASSERT(!exists(name), "Shader already exists!");
		m_shaders[name] = shader;
	}

	void ShaderLibrary::add(Ref<Shader> const& shader)
	{
		auto& name = shader->getName();
		add(name, shader);
	}

	Ref<Shader> ShaderLibrary::load(std::string const& filepath)
	{
		auto shader = Shader::create(filepath);
		add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::load(std::string const& name, std::string const& filepath)
	{
		auto shader = Shader::create(filepath);
		add(name, shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::get(std::string const& name)
	{
		ME_CORE_ASSERT(exists(name), "Shader not found!");
		return m_shaders[name];
	}
	bool ShaderLibrary::exists(std::string const& name)
	{
		return m_shaders.find(name) != m_shaders.end();
	}
}