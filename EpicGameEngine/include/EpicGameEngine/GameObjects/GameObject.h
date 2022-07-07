#pragma once
#include <EpicGameEngine/ege_pch.h>

namespace EpicGameEngine
{
    class EPICGAMEENGINE_API GameObject
    {
    public:
        GameObject() = default;
        virtual ~GameObject() = default;

        virtual void Render() = 0;
        virtual void Shutdown() = 0;
        [[nodiscard]] auto GetName() const -> std::string { return name; }
         

        // doesn't actually delete class
        static void Destroy(GameObject& gameObject) { gameObject.Shutdown(); }
    private:
        std::string name; 
    };
}