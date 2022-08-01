//
// Created by Rishan Thangaraj on 7/7/2022.
//

#pragma once

#include <entt.hpp>
#include <EpicGameEngine/ege_pch.h>
#include <EpicGameEngine/Timestep.h>

namespace EpicGameEngine
{
    class GameObject;

    class Scene
    {
    public:
        Scene() = default;
        ~Scene() = default;

        GameObject CreateGameObject(const std::string& name = std::string());
        void DeleteGameObject(GameObject gameObject);

        void OnUpdate(Timestep ts);
        void OnViewportResize(uint32_t width, uint32_t height);

        entt::registry registry;

        glm::vec2 viewportSize;
    private:
        friend class GameObject;
        friend class GameObjectsPanel;
        friend class SceneSerializer;
    };
}
