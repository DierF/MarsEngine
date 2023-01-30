#pragma once

#ifdef ME_PLATFORM_WINDOWS
	#ifdef ME_BUILD_DLL
		#define ME_API __declspec(dllexport)
	#else
		#define ME_API __declspec(dllimport)
	#endif
#else
	#error MarsEngine only support Windows!
#endif

#define BIT(x) (1 << x)