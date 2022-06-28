#include <EpicGameEngine/Events/EventConversion.h>
#include <EpicGameEngine/ege_pch.h>
#include <EpicGameEngine/Events/Event.h>
#include <EpicGameEngine/Events/InputEvent.h>
#include <EpicGameEngine/Events/WindowEvent.h>

namespace EpicGameEngine
{
    // TODO: Finish implementing this as we write more events
    auto SDL_Event_to_Event(const SDL_Event* event) -> std::shared_ptr<Event>
    {
        switch (event->type)
        {
            case SDL_QUIT:
                return std::make_shared<WindowCloseEvent>();
                break;
            case SDL_WINDOWEVENT:
                switch (event->window.event)
                {
                    case SDL_WINDOWEVENT_RESIZED:
                        return std::make_shared<WindowResizeEvent>(event->window.data1, event->window.data2);
                        break;
                }
                break;
        }
        return nullptr;
    }
}