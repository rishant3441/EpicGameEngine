//
// Created by Rishan Thangaraj on 7/7/2022.
//

#pragma once

#include <entt.hpp>
#include <EpicGameEngine/ege_pch.h>
#include <EpicGameEngine/Timestep.h>
#include <EpicGameEngine/UUID.h>
#include <EpicGameEngine/Renderer/Camera.h>

namespace EpicGameEngine
{
    class GameObject;

    class Scene
    {
    public:
        Scene() = default;
        ~Scene() = default;

        GameObject CreateGameObject(const std::string& name = std::string());
        GameObject CreateGameObjectWithUUID(const std::string& name, UUID uuid);
        void DeleteGameObject(GameObject gameObject);

        void OnEditorUpdate(Timestep ts, EditorCamera& camera);
        void OnRuntimeStart();
        void OnRuntimeStop();
        void OnRuntimeUpdate(Timestep ts);
        void OnViewportResize(uint32_t width, uint32_t height);

        GameObject GetPrimaryCamera();
        GameObject GetGameObjectByUUID(UUID uuid);

        entt::registry registry;

        glm::vec2 viewportSize;
    private:
        std::unordered_map<UUID, entt::entity> entityMap;

        friend class GameObject;
        friend class GameObjectsPanel;
        friend class SceneSerializer;

    };
}
