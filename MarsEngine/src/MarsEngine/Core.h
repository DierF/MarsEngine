#pragma once

#ifdef ME_PLATFORM_WINDOWS
	#if ME_DYNAMIC_LINK
		#ifdef ME_BUILD_DLL
			#define ME_API __declspec(dllexport)
		#else
			#define ME_API __declspec(dllimport)
		#endif
	#else
		#define ME_API
	#endif
#else
	#error MarsEngine only support Windows!
#endif

#ifdef ME_ENABLE_ASSERTS
	#define ME_ASSERT(x, ...) {if(!(x)) {ME_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); }}
	#define ME_CORE_ASSERT(x, ...) {if(!(x)) {ME_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); }}
#else
	#define ME_ASSERT(x, ...)
	#define ME_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define ME_BIND_EVENT_FUNC(func) std::bind(&func, this, std::placeholders::_1)

#include <memory> //can't believe I have to do this to enable the following alias template. >_<
namespace MarsEngine
{

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;


}