#pragma once

// For use by Epic Game Engine Applications

#include <EpicGameEngine/Application.h>
#include "SDL.h"

// -------------ENTRY POINT------------------
#include <EpicGameEngine/EntryPoint.h>
// ------------------------------------------
#include <EpicGameEngine/Window/Window.h>
#ifdef EGE_PLATFORM_WINDOWS
	#include <EpicGameEngine/Window/WindowsWindow.h>
#endif
