//
// Created by Rishan Thangaraj on 3/24/2023.
//
// Copyright (c) 2023, Rishan Thangaraj <rishanthangaraj@gmail.com> All rights reserved.
// SPDX-License-Identifier: MIT
//

#pragma once

#include <EpicGameEngine/ege_pch.h>
#include "VertexArray.h"

namespace EpicGameEngine
{
    class RendererAPI
    {
    public:
        enum class Backend
        {
            None = 0,
            OpenGL = 1
        };
        virtual ~RendererAPI() = default;

        virtual void Init() = 0;
        virtual void Shutdown() = 0;

        // Config
        virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
        virtual void SetClearColor(const glm::vec4& color) = 0;

        // Draw Commands
        virtual void Clear() = 0;
        virtual void DrawIndicies(const std::shared_ptr<VertexArray>& va, uint32_t indexCount = 0) = 0;

        static Backend GetBackend() { return backend; }

        static std::unique_ptr<RendererAPI> Create();

    private:
        static Backend backend; // Current backend selected
    };
}

