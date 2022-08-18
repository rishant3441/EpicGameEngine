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
            case SDL_MOUSEBUTTONDOWN:
            //if (event->button.button == SDL_BUTTON_LEFT)
            //    return std::make_shared<MousePressedEvent>(Mouse::ButtonLeft);
            //    break;
                switch(event->button.button)
                {
                    case SDL_BUTTON_LEFT:
                        return std::make_shared<MousePressedEvent>(Mouse::ButtonLeft);
                        break;
                    case SDL_BUTTON_RIGHT:
                        return std::make_shared<MousePressedEvent>(Mouse::ButtonRight);
                        break;
                    case SDL_BUTTON_MIDDLE:
                        return std::make_shared<MousePressedEvent>(Mouse::ButtonMiddle);
                        break;
                }
            case SDL_MOUSEBUTTONUP: 
                switch(event->button.button)
                {
                    case SDL_BUTTON_LEFT:
                        return std::make_shared<MouseReleasedEvent>(Mouse::ButtonLeft);
                        break;
                    case SDL_BUTTON_RIGHT:
                        return std::make_shared<MouseReleasedEvent>(Mouse::ButtonRight);
                        break;
                    case SDL_BUTTON_MIDDLE:
                        return std::make_shared<MouseReleasedEvent>(Mouse::ButtonMiddle);
                }
                break;
            case SDL_MOUSEWHEEL:
                return std::make_shared<MouseScrolledEvent>(event->wheel.x, event->wheel.y);
        }
        return nullptr;
    }
}