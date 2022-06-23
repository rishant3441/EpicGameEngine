#pragma once
#include <EpicGameEngine/ege_pch.h>

namespace EpicGameEngine
{
    struct EPICGAMEENGINE_API Vector2
    {
        double x, y;
    };
    class EPICGAMEENGINE_API GameObject
    {
    public:
        GameObject() {}
        virtual ~GameObject() = 0 {}

        Vector2 position;

        virtual void Render() = 0;
        virtual void Shutdown() = 0;
        std::string GetName() const { return name; }
         

        // doesn't actually delete class
        static void Destroy(GameObject& gameObject) { gameObject.Shutdown(); }
    private:
        std::string name; 
    };
}