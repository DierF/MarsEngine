#pragma once

#include <string>

namespace MarsEngine {

	class Shader {

	public:

		virtual ~Shader() = default;

		virtual void bind() const = 0;

		virtual void unbind() const = 0;

		static Shader* create(std::string const& vertexSrc, std::string const& fragmentSrc);
	};
}