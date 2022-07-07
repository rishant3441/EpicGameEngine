//
// Created by Rishan Thangaraj on 7/7/2022.
//

#pragma once

#include <entt.hpp>
#include <EpicGameEngine/ege_pch.h>

namespace EpicGameEngine
{
    class Scene
    {
    public:
        Scene() = default;
        ~Scene() = default;
    private:
        entt::registry registry;
    };
}
