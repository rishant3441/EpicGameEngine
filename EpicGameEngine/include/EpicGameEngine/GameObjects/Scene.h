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

        void OnRender();

        entt::registry registry;
    private:
        friend class GameObject;
    };
}
