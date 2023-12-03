//
// Created by Rishan Thangaraj on 3/24/2023.
//
// Copyright (c) 2023, Rishan Thangaraj <rishanthangaraj@gmail.com> All rights reserved.
// SPDX-License-Identifier: MIT
//

#pragma once

#include "EpicGameEngine/Renderer/RendererAPI.h"

namespace EpicGameEngine
{

    class OpenGLRendererAPI : public RendererAPI
    {
    public:
       void Init() override;
       void Shutdown() override;

       void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
       void SetClearColor(const glm::vec4& color) override;

       void Clear() override;

       void DrawIndicies(const std::shared_ptr<VertexArray>& va, uint32_t indexCount = 0) override;
    private:
    };

} // EpicGameEngine