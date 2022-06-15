#pragma once
#include <EpicGameEngine/Events/Event.h>

namespace EpicGameEngine
{
	std::shared_ptr<Event> SDL_Event_to_Event(const SDL_Event* event);
}