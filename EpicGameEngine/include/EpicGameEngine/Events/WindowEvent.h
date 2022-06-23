#pragma once
#include <EpicGameEngine/Events/Event.h>

// TODO: Write ToString() function potentially. 
namespace EpicGameEngine
{
    class WindowCloseEvent : public Event
    {
    public:
        WindowCloseEvent() = default;
        ~WindowCloseEvent() override = default;

        [[nodiscard]] auto toString() const -> std::string override 
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
        ~WindowResizeEvent() override = default;

        [[nodiscard]] auto toString() const -> std::string override 
        {
            return "Window Resize Event";
        }

        auto GetNewWidth() -> unsigned int { return width; }
        auto GetNewHeight() -> unsigned int { return height; }

        EVENT_CLASS_TYPE(WindowResize);

    private:
        unsigned int width;
        unsigned int height; 
    };
}