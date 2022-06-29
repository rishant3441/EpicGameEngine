#pragma once
#include <EpicGameEngine/Events/Event.h>
#include <EpicGameEngine/Input/MouseCodes.h>
#include <EpicGameEngine/Input/KeyCodes.h>

// TODO: Add all of the derived input events defined in the EventTypes enum here.
namespace EpicGameEngine
{
    class MousePressedEvent : public Event
    {
    public:
        MousePressedEvent(MouseCode button)
            : button(button)
        {}
        ~MousePressedEvent() override = default;

        [[nodiscard]] auto toString() const -> std::string override 
        {
            return "Mouse Pressed Event";
        }

        MouseCode button; 

        EVENT_CLASS_TYPE(MousePressed);
    };

    class MouseReleasedEvent : public Event
    {
    public:
        MouseReleasedEvent(MouseCode button)
            : button(button)
        {}
        ~MouseReleasedEvent() override = default;

        [[nodiscard]] auto toString() const -> std::string override 
        {
            return "Mouse Released Event";
        }

        MouseCode button; 

        EVENT_CLASS_TYPE(MouseReleased);
    };
}