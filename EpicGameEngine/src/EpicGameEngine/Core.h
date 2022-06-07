#pragma once

#ifdef EGE_PLATFORM_WINDOWS
	#ifdef EGE_BUILD_DLL
		#define EPICGAMEENGINE_API __declspec(dllexport)
	#else
		#define EPICGAMEENGINE_API __declspec(dllimport)
	#endif
#else
	#error Epic Game Engine only supports Windows!
#endif

