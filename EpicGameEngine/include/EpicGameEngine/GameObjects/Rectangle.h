#pragma once
#include <EpicGameEngine/GameObjects/GameObject.h>
#include <EpicGameEngine/ege_pch.h>

namespace EpicGameEngine
{
    struct EPICGAMEENGINE_API transformS
    {
        double w, h;
    };
    class EPICGAMEENGINE_API Rectangle : public GameObject
    {
    public:

        Rectangle(double x, double y, double w, double h, std::string name, SDL_Color color);
        ~Rectangle();

        transformS transform;

        void Render();
        void Shutdown();

        GPU_Rect rect;
        SDL_Color color;

    private:
        std::string name = "Rectangle";
    };
}