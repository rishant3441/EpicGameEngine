#include <EpicGameEngine/GameObjects/Rectangle.h>
#include <EpicGameEngine/Renderer/Renderer.h>
#include <EpicGameEngine/ege_pch.h>
#include <spdlog/spdlog.h>

namespace EpicGameEngine
{
    Rectangle::Rectangle(double x, double y, double w, double h, std::string name, SDL_Color color)
    :   name(name), color(color)
    {
        position.x = x;
        position.y = y;
        transform.w = w;
        transform.h = h;

        rect.x = position.x;
        rect.y = position.y;
        rect.w = transform.w;
        rect.h = transform.h;
    }

    Rectangle::~Rectangle()
    {
        Rectangle::Shutdown();
    }

    void Rectangle::Render()
    {
        rect.x = position.x;
        rect.y = position.y;
        rect.w = transform.w;
        rect.h = transform.h;

        GPU_RectangleFilled2(Renderer::target, rect, color);
    }
    
    void Rectangle::Shutdown()
    {
        
    }
}