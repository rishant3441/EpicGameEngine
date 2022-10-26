//
// Created by Rishan Thangaraj on 8/23/2022.
//
// Copyright (c) 2022, Rishan Thangaraj <rishanthangaraj@gmail.com> All rights reserved.
// SPDX-License-Identifier: MIT
//

#pragma once

#include <EpicGameEngine/ege_pch.h>
#include <EpicGameEngine/Renderer/ShaderProgram.h>

#include <glm/glm.hpp>
#include <SDL_gpu.h>

// TODO: Move shader specific code to a shader class in the future
namespace EpicGameEngine
{
    class Lighting
    {
    public:
        Lighting() = default;
        ~Lighting() = default;

        void Init();
        void Shutdown();

        void Render();
        // Renders a light at a specific coordinate with a specific intensity.
        // TODO: Add more options in the future
        void RenderAt(glm::vec3 position, float intensity = 1);

    private:
        // Renderer Targets
        GPU_Target* lightingTarget;
        GPU_Image* lightingTexture;

        Ref<ShaderProgram> shader;
    };
}

