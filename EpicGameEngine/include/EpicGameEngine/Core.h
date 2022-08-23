#pragma once

#include <memory>
#include <EpicGameEngine/CoreLib/CoreLib.h>

#ifdef EGE_PLATFORM_WINDOWS
	#ifdef EGE_BUILD_DLL
		#define EPICGAMEENGINE_API __declspec(dllexport)
	#else
		#define EPICGAMEENGINE_API
	#endif
#else
	#error Epic Game Engine only supports Windows!
#endif

namespace EpicGameEngine
{
    template <typename T>
    using Ref = std::shared_ptr<T>;
    template <typename T, typename ... Args>
    constexpr Ref<T> CreateRef(Args&& ... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
}