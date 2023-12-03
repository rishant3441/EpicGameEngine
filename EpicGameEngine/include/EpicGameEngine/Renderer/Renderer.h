//
// Created by Rishan Thangaraj on 3/24/2023.
//
// Copyright (c) 2023, Rishan Thangaraj <rishanthangaraj@gmail.com> All rights reserved.
// SPDX-License-Identifier: MIT
//

#pragma once

#include "EpicGameEngine/Renderer/VertexArray.h"
#include "EpicGameEngine/Renderer/RendererAPI.h"
#include <EpicGameEngine/Renderer/Framebuffer.h>

namespace EpicGameEngine
{
    class Renderer
    {
    public:
        static void Init();
        static void Shutdown();

        static void DrawIndicies(const std::shared_ptr<VertexArray>& va, uint32_t indexCount = 0);

        static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
        static void SetClearColor(const glm::vec4& color);
        static void Clear();

    private:
        static std::unique_ptr<RendererAPI> rendererAPI;
    };
}
