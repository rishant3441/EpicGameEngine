//
// Created by Rishan Thangaraj on 8/23/2022.
//
// Copyright (c) 2022, Rishan Thangaraj <rishanthangaraj@gmail.com> All rights reserved.
// SPDX-License-Identifier: MIT
//

#pragma once


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
        void RenderAt(glm::vec3 position, float intensity = 1);

    private:
        GPU_Target* lightingTarget;
        GPU_Image* lightingTexture;
        std::string vertexShader;
        std::string fragmentShader;
        uint32_t shader;
        uint32_t vShader;
        uint32_t shaderObj;
    };
}

