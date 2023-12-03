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
#include <vector>
#include <functional>
#include <fstream>
#include <cassert>
#include <thread>
#include <atomic>
#include <stdexcept>
#include <strstream>
#include <sstream>
#include <algorithm>
#include <execution>
#include <random>
#include <initializer_list>
#include <SDL.h>
#include <glew.h>
#include <SDL_opengl.h>
#include <glm/glm.hpp>
#include "Core.h"