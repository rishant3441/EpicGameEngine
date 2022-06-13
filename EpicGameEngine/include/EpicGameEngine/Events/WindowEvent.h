#pragma once
#include <EpicGameEngine/Events/Event.h>

// TODO: Write ToString() function potentially. 
namespace EpicGameEngine
{
    class WindowCloseEvent : public Event
    {
    public:
        WindowCloseEvent() {}
        ~WindowCloseEvent() {}

        virtual EventType GetEventType() { return EventType::WindowClose; }
    };

    class WindowResizeEvent : public Event
    {
    public:
        WindowResizeEvent(unsigned int width, unsigned int height)
        : width(width), height(height)
        {

        }

        virtual EventType GetEventType() { return EventType::WindowResize; }

        unsigned int GetNewWidth() { return width; }
        unsigned int GetNewHeight() { return height; }

    private:
        unsigned int width;
        unsigned int height; 
    };
}