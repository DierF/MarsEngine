#pragma once

#include <string>
#include <unordered_map>

namespace MarsEngine {

	class Shader {

	public:

		virtual ~Shader() = default;

		virtual void bind() const = 0;

		virtual void unbind() const = 0;

		virtual std::string const& getName() const = 0;

		static Ref<Shader> create(std::string const& filepath);

		static Ref<Shader> create(std::string const& name, std::string const& vertexSrc, std::string const& fragmentSrc);
	};

	class ShaderLibrary
	{
	public:
		void add(std::string const& name, Ref<Shader> const& shader);

		void add(Ref<Shader> const& shader);

		Ref<Shader> load(std::string const& filepath);

		Ref<Shader> load(std::string const& name, std::string const& filepath);

		Ref<Shader> get(std::string const& name);

		bool exists(std::string const& name);

	private:
		std::unordered_map<std::string, Ref<Shader>> m_shaders;
	};
}