#pragma once

#ifdef EGE_PLATFORM_WINDOWS
    #ifndef NOMINMAX
        #define NOMINMAX
    #endif
#endif

#include <iostream>
#include <memory>
#include <cstdint>
#include <string>
#include <SDL.h>
#include <SDL_gpu.h>