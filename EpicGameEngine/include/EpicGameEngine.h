#pragma once

// For use by Epic Game Engine Applications

#include <EpicGameEngine/Application.h>
#include "SDL.h"
#include <SDL_gpu.h>

// WINDOW:
#include <EpicGameEngine/Window/Window.h>
#ifdef EGE_PLATFORM_WINDOWS
	#include <EpicGameEngine/Window/WindowsWindow.h>
	#include <EpicGameEngine/Input/Input.h>
#endif

// LOGGING:
#include <spdlog/spdlog.h>

// EVENTS:
#include <EpicGameEngine/Events/Event.h>

// INPUT:
#include <EpicGameEngine/Input/Input.h>
#include <EpicGameEngine/Input/KeyCodes.h>
#include <EpicGameEngine/Input/MouseCodes.h>

// LAYERS:
#include <EpicGameEngine/Layers/Layer.h>
#include <EpicGameEngine/Layers/LayerArray.h>

// GAMEOBJECTS:
#include <EpicGameEngine/GameObjects/GameObject.h>
#include <EpicGameEngine/GameObjects/Rectangle.h>