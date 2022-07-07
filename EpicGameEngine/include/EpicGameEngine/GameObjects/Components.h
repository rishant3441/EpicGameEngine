//
// Created by Rishan Thangaraj on 7/7/2022.
//
// Copyright (c) 2022, Rishan Thangaraj <rishanthangaraj@gmail.com> All rights reserved.
// SPDX-License-Identifier: MIT
//

#pragma once
#include <EpicGameEngine/ege_pch.h>

namespace EpicGameEngine
{
    struct TransformComponent
    {
        glm::vec2 Position = { 0.0f, 0.0f };
        glm::vec2 Rotation = { 0.0f, 0.0f };
        glm::vec2 Scale = { 1.0f, 1.0f };

        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const glm::vec2& position)
            : Position(position)
        {}
    };

    struct SpriteRendererComponent
    {
        SDL_Color Color{255, 255, 255, 255};
        std::shared_ptr<Texture> Texture;

        SpriteRendererComponent() = default;
        SpriteRendererComponent(const SpriteRendererComponent&) = default;
        SpriteRendererComponent(const SDL_Color& color)
            : Color(color)
        {}
    };
}