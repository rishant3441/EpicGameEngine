#pragma once
#include <EpicGameEngine/ege_pch.h>
#include <EpicGameEngine/Events/Event.h>
#include <EpicGameEngine/GameObjects/GameObject.h>
#include <EpicGameEngine/Timestep.h>
#include <EpicGameEngine/GameObjects/Scene.h>

#include <utility>

namespace EpicGameEngine
{
    class EPICGAMEENGINE_API Layer
    {
    public:
        Layer(std::string name = "Layer")
        : name(std::move(name))
        {

        }
        virtual ~Layer() = default;

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate(Timestep time) {}
        virtual void OnImGuiRender();
        virtual void OnEvent(std::shared_ptr<Event> e)  {}
        
        void OnRender();

        [[nodiscard]] const std::string& GetName() const { return name; }

        std::vector<GameObject*> gameObjects{};
    private:
        std::string name; 
    
    };
}