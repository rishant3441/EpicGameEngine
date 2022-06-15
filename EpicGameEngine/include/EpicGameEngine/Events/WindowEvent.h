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

        std::string toString() const override 
        {
            return "Window Close Event";
        }

        EVENT_CLASS_TYPE(WindowClose);
    };

    class WindowResizeEvent : public Event
    {
    public:
        WindowResizeEvent(unsigned int width, unsigned int height)
        : width(width), height(height)
        {

        }
        ~WindowResizeEvent() {}

        std::string toString() const override 
        {
            return "Window Resize Event";
        }

        unsigned int GetNewWidth() { return width; }
        unsigned int GetNewHeight() { return height; }

        EVENT_CLASS_TYPE(WindowResize);

    private:
        unsigned int width;
        unsigned int height; 
    };
}