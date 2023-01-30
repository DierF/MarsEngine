#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace MarsEngine {

	class ME_API Log {

	public:
		static void init();

		inline static std::shared_ptr<spdlog::logger>& getCoreLogger() {
			return s_CoreLogger;
		}

		inline static std::shared_ptr<spdlog::logger>& getClientLogger() {
			return s_ClientLogger;
		}

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

//Core log macros
#define ME_CORE_TRACE(...)    ::MarsEngine::Log::getCoreLogger()->trace(__VA_ARGS__)
#define ME_CORE_INFO(...)     ::MarsEngine::Log::getCoreLogger()->info(__VA_ARGS__)
#define ME_CORE_WARN(...)     ::MarsEngine::Log::getCoreLogger()->warn(__VA_ARGS__)
#define ME_CORE_ERROR(...)    ::MarsEngine::Log::getCoreLogger()->error(__VA_ARGS__)
#define ME_CORE_FATAL(...)    ::MarsEngine::Log::getCoreLogger()->fatal(__VA_ARGS__)

//Client log macros
#define ME_TRACE(...)         ::MarsEngine::Log::getClientLogger()->trace(__VA_ARGS__)
#define ME_INFO(...)          ::MarsEngine::Log::getClientLogger()->info(__VA_ARGS__)
#define ME_WARN(...)          ::MarsEngine::Log::getClientLogger()->warn(__VA_ARGS__)
#define ME_ERROR(...)         ::MarsEngine::Log::getClientLogger()->error(__VA_ARGS__)
#define ME_FATAL(...)         ::MarsEngine::Log::getClientLogger()->fatal(__VA_ARGS__)

//if dist build
#define ME_CORE_INFO