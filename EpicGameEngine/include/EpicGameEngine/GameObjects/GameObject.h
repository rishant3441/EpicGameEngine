#pragma once
#include <EpicGameEngine/ege_pch.h>
#include <entt.hpp>
#include "Scene.h"

namespace EpicGameEngine
{
    class EPICGAMEENGINE_API GameObject
    {
    public:
        GameObject() = default;
        GameObject(entt::entity handle, Scene* scene);
        GameObject(const GameObject&) = default;

        virtual void Shutdown() {}
        [[nodiscard]] auto GetName() const -> std::string { return name; }
         

        // doesn't actually delete class
        static void Destroy(GameObject& gameObject) { gameObject.Shutdown(); }

        template<typename T, typename... Args>
        T& AddComponent(Args&&... args)
        {
            assert(!HasComponent<T>() && "Entity already has this component!");
            return scene->registry.emplace<T>(entityHandle, std::forward<Args>(args)...);
        }

        template<typename T>
        T& GetComponent()
        {
            assert(HasComponent<T>() && "Entity does not have this component!");
            return scene->registry.get<T>(entityHandle);
        }

        template<typename T>
        bool HasComponent()
        {
            return scene->registry.all_of<T>(entityHandle);
        }

        template<typename T>
        void RemoveComponent()
        {
            assert(HasComponent<T>() && "Entity does not have this component!");
            scene->registry.remove<T>(entityHandle);
        }

        operator entt::entity() { return entityHandle; }
        operator bool() const { return entityHandle != entt::null; }

    private:
        std::string name;
        entt::entity entityHandle{ entt::null };
        Scene* scene = nullptr;
    };
}