#pragma once
#include <EpicGameEngine/ege_pch.h>
#include <EpicGameEngine/Events/Event.h>
#include <EpicGameEngine/GameObjects/GameObject.h>
#include <EpicGameEngine/Timestep.h>

namespace EpicGameEngine
{
    class EPICGAMEENGINE_API Layer
    {
    public:
        Layer(const std::string& name = "Layer")
        : name(name)
        {

        }
        virtual ~Layer() = default;

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate(Timestep time) {}
        virtual void OnImGuiRender();
        virtual void OnEvent(std::shared_ptr<Event> e)  {}
        
        void OnRender();

        const std::string& GetName() const { return name; }

        std::vector<GameObject*> gameObjects{};
    private:
        std::string name; 
    
    };
}