#pragma once
#include <EpicGameEngine/Events/Event.h>

namespace EpicGameEngine
{
	auto SDL_Event_to_Event(const SDL_Event* event) -> std::shared_ptr<Event>;
}